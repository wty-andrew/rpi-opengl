#include "input.h"

#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#include <cstdio>
#include <cstring>

#include <iostream>
#include <string>
#include <vector>
#include <queue>

namespace {

std::vector<int> fds;
std::vector<bool> keyboard_state(KeyCode::Count, false);

const int MAX_QUEUE_SIZE = 64;
std::queue<Event> event_queue;

void open_devices() {
  std::string input_device_path("/dev/input");

  struct dirent* entry;
  DIR* dir = opendir(input_device_path.c_str());

  if (dir == NULL) return;

  while ((entry = readdir(dir)) != NULL) {
    std::string name{ entry->d_name };

    if (name.rfind("event", 0) != 0) continue;

    std::string fullpath = input_device_path + "/" + name;
    int fd = open(fullpath.c_str(), O_RDONLY | O_NONBLOCK);
    if (fd <= 0) {
      std::cerr << "Error opening " << fullpath << ": " << strerror(errno) << std::endl;
      continue;
    }
    // TODO: better filter out non keyboard and mouse devices
    fds.push_back(fd);
  }

  closedir(dir);
}

void close_devices() {
  for (int fd : fds) close(fd);
}

void add_event(Event& event) {
  if (event_queue.size() >= MAX_QUEUE_SIZE) event_queue.pop();
  event_queue.push(event);
}

bool get_event(Event& event) {
  if (event_queue.empty()) return false;

  event = event_queue.front();
  event_queue.pop();
  return true;
}

void process_events() {
  for (auto fd : fds) {
    struct input_event raw_event;

    while (read(fd, &raw_event, sizeof(struct input_event)) > 0) {
      Event event;
      if (to_keyboard_event(raw_event, event)) {
        keyboard_state[event.key.code] = event.key.type != KeyboardEvent::Type::Released;
        add_event(event);
        continue;
      }

      // TODO: update global mouse state
      if (to_mouse_motion_event(raw_event, event)) {
        add_event(event);
        continue;
      }

      if (to_mouse_click_event(raw_event, event)) {
        add_event(event);
        continue;
      }

      if (to_mouse_wheel_event(raw_event, event)) {
        add_event(event);
        continue;
      }
    }
  }
}

}  // namespace

void Input::init() { open_devices(); }

void Input::terminate() { close_devices(); }

bool Input::is_key_pressed(KeyCode key) {
  if (key < 0 || key >= keyboard_state.size()) return false;
  return keyboard_state.at(key);
}

bool Input::poll_event(Event& event) {
  if (event_queue.empty()) process_events();

  return get_event(event);
}
