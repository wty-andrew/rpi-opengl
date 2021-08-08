#include "event.h"

KeyCode remap_key(int key) {
  switch (key) {
    case KEY_LEFT:
      return KeyCode::Left;
    case KEY_RIGHT:
      return KeyCode::Right;
    case KEY_UP:
      return KeyCode::Up;
    case KEY_DOWN:
      return KeyCode::Down;
    case KEY_SPACE:
      return KeyCode::Space;
    case KEY_ESC:
      return KeyCode::Escape;
    default:
      return KeyCode::Count;
  }
}

bool to_keyboard_event(const input_event& raw_event, Event& event) {
  if (raw_event.type != EV_KEY) return false;
  auto code = remap_key(raw_event.code);
  if (code == KeyCode::Count) return false;

  KeyboardEvent::Type type;
  switch (raw_event.value) {
    case 0:
      type = KeyboardEvent::Type::Released;
      break;
    case 1:
      type = KeyboardEvent::Type::Pressed;
      break;
    case 2:
      type = KeyboardEvent::Type::Repeated;
      break;
    default:
      return false;
  }

  event.type = Event::Type::Keyboard;
  event.key.code = code;
  event.key.type = type;
  return true;
}

bool to_mouse_motion_event(const input_event& raw_event, Event& event) {
  // FIXME: keep track of mouse position and fill the 0 fields
  if (raw_event.type == EV_REL && (raw_event.code == REL_X || raw_event.code == REL_Y)) {
    event.type = Event::Type::MouseMotion;
    event.motion.x = 0;
    event.motion.y = 0;
    event.motion.dx = raw_event.code == REL_X ? raw_event.value : 0;
    event.motion.dy = raw_event.code == REL_Y ? raw_event.value : 0;
    return true;
  }

  if (raw_event.type == EV_ABS && (raw_event.code == ABS_X || raw_event.code == ABS_Y)) {
    event.type = Event::Type::MouseMotion;
    event.motion.x = raw_event.code == ABS_X ? raw_event.value : 0;
    event.motion.y = raw_event.code == ABS_Y ? raw_event.value : 0;
    event.motion.dx = 0;
    event.motion.dy = 0;
    return true;
  }

  return false;
}

bool to_mouse_click_event(const input_event& raw_event, Event& event) {
  if (raw_event.type != EV_KEY) return false;

  MouseClickEvent::Button button;
  switch (raw_event.code) {
    case BTN_LEFT:
      button = MouseClickEvent::Button::Left;
      break;
    case BTN_RIGHT:
      button = MouseClickEvent::Button::Right;
      break;
    case BTN_MIDDLE:
      button = MouseClickEvent::Button::Middle;
      break;
    default:
      return false;
  }
  MouseClickEvent::Type type =
    raw_event.value ? MouseClickEvent::Type::Down : MouseClickEvent::Type::Up;

  event.type = Event::Type::MouseClick;
  event.click.button = button;
  event.click.type = type;
  return true;
}

bool to_mouse_wheel_event(const input_event& raw_event, Event& event) {
  if (raw_event.type != EV_REL || raw_event.code != REL_WHEEL) return false;

  event.type = Event::Type::MouseWheel;
  event.wheel.y = raw_event.value;
  return true;
}
