#ifndef INPUT_H
#define INPUT_H

#include "event.h"

class Input {
public:
  static void init();

  static void terminate();

  static bool is_key_pressed(KeyCode key);

  static bool poll_event(Event& event);
};

#endif  // INPUT_H
