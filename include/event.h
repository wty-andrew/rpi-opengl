#ifndef EVENT_H
#define EVENT_H

#include <linux/input.h>

enum KeyCode {
  // TODO: fill all common keys
  Left,
  Right,
  Up,
  Down,
  Escape,
  Space,

  Count,
};

struct KeyboardEvent {
  enum class Type {
    Pressed,
    Released,
    Repeated,
  };

  Type type;
  KeyCode code;
};

struct MouseMotionEvent {
  int x;
  int y;
  int dx;
  int dy;
};

struct MouseClickEvent {
  enum class Type {
    Down,
    Up,
  };

  enum class Button {
    Left,
    Middle,
    Right,
  };

  Type type;
  Button button;
};

struct MouseWheelEvent {
  int y;
};

struct Event {
  enum class Type {
    Keyboard,
    MouseMotion,
    MouseClick,
    MouseWheel,
  };

  Type type;

  union {
    MouseMotionEvent motion;
    MouseClickEvent click;
    MouseWheelEvent wheel;
    KeyboardEvent key;
  };
};

/* event conversions */
KeyCode remap_key(int key);

bool to_keyboard_event(const input_event& raw_event, Event& event);

bool to_mouse_motion_event(const input_event& raw_event, Event& event);

bool to_mouse_click_event(const input_event& raw_event, Event& event);

bool to_mouse_wheel_event(const input_event& raw_event, Event& event);

#endif  // EVENT_H
