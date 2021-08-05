#include "window.h"
#include "shader.h"

#include <GLES2/gl2.h>

#include <string>
#include <chrono>
#include <thread>

int main(int argc, char** argv) {
  Window window;
  window.init();

  std::string vertex_shader_code = R"(
attribute vec4 a_position;
attribute vec4 a_color;

varying vec4 v_color;

void main() {
  gl_Position = a_position;
  v_color = a_color;
}
)";

  std::string fragment_shader_code = R"(
precision mediump float;
varying vec4 v_color;

void main() {
  gl_FragColor = v_color;
}
)";

  // clang-format off
  GLfloat position_vertices[] = {
     0.0f,  0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f
  };

  GLfloat color_vertices[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
  };
  // clang-format on

  Shader shader;
  shader.load(vertex_shader_code, fragment_shader_code);

  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  shader.use();

  GLuint position_loc = glGetAttribLocation(shader.get_program(), "a_position");
  glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 0, position_vertices);
  glEnableVertexAttribArray(position_loc);

  GLuint color_loc = glGetAttribLocation(shader.get_program(), "a_color");
  glVertexAttribPointer(color_loc, 3, GL_FLOAT, GL_FALSE, 0, color_vertices);
  glEnableVertexAttribArray(color_loc);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  window.update();

  std::this_thread::sleep_for(std::chrono::seconds(3));

  return 0;
}
