#include "window.h"
#include "shader.h"
#include "input.h"

#include <GLES2/gl2.h>

#include <string>
#include <chrono>
#include <thread>

GLuint create_texture_2d(int width, int height, GLubyte* data) {
  GLuint texture_id;
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  return texture_id;
}

int main(int argc, char** argv) {
  Window window;
  window.init();
  Input::init();
  atexit(Input::terminate);

  std::string vertex_shader_code = R"(
attribute vec4 a_position;
attribute vec2 a_tex_coord;

varying vec2 v_tex_coord;

void main() {
  gl_Position = a_position;
  v_tex_coord = a_tex_coord;
}
)";

  std::string fragment_shader_code = R"(
precision mediump float;
varying vec2 v_tex_coord;

uniform sampler2D u_texture;

void main() {
  gl_FragColor = texture2D(u_texture, v_tex_coord);
}
)";

  // clang-format off
  GLfloat vertices[] = {
    // position         // tex coord
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // top left
  };

  GLushort indices[] = {
    0, 1, 2,
    0, 2, 3,
  };

  // textures are loaded left to right, bottom to top
  GLubyte pixels[] = {
    255,   0,   0, // Red (bottom left)
      0, 255,   0, // Green (bottom right)
      0,   0, 255, // Blue (top left)
    255, 255,   0, // Yellow (top right)
  };
  // clang-format on

  Shader shader;
  shader.load(vertex_shader_code, fragment_shader_code);

  GLuint position_loc = glGetAttribLocation(shader.get_program(), "a_position");
  GLuint tex_coord_loc = glGetAttribLocation(shader.get_program(), "a_tex_coord");

  GLuint texture_id = create_texture_2d(2, 2, pixels);

  bool running = true;
  while (running) {
    Event event;
    while (Input::poll_event(event)) {
      switch (event.type) {
        case Event::Type::Keyboard: {
          if (event.key.code == KeyCode::Escape) running = false;
          break;
        }

        default:
          break;
      }
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    glVertexAttribPointer(position_loc, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vertices);
    glEnableVertexAttribArray(position_loc);

    glVertexAttribPointer(tex_coord_loc, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &vertices[3]);
    glEnableVertexAttribArray(tex_coord_loc);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    shader.set_uniform("u_texture", 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);

    window.update();

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  return 0;
}
