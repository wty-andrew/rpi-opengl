#include "window.h"

#include <GLES2/gl2.h>

#include <chrono>
#include <thread>

int main(int argc, char** argv) {
  Window window;
  window.init();

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  window.update();

  std::this_thread::sleep_for(std::chrono::seconds(3));

  return 0;
}
