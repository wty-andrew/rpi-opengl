#include "shader.h"

#include <iostream>
#include <stdexcept>

GLuint compile_shader(GLuint type, const std::string& source) {
  GLuint shader = glCreateShader(type);

  const char* content = source.c_str();
  glShaderSource(shader, 1, &content, NULL);
  glCompileShader(shader);

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (compiled != GL_TRUE) {
    GLint info_len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);
    std::vector<GLchar> message(info_len);
    glGetShaderInfoLog(shader, info_len, NULL, &message[0]);

    glDeleteShader(shader);
    throw std::runtime_error(std::string(message.begin(), message.end()));
  }

  return shader;
}

GLuint create_shader_program(GLuint vertex_shader, GLuint fragment_shader) {
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);
  glLinkProgram(program);

  GLint linked;
  glGetProgramiv(program, GL_LINK_STATUS, &linked);

  if (linked != GL_TRUE) {
    GLint info_len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_len);
    std::vector<GLchar> message(info_len);
    glGetProgramInfoLog(program, info_len, NULL, &message[0]);

    glDeleteProgram(program);
    throw std::runtime_error(std::string(message.begin(), message.end()));
  }

  return program;
}

Shader::Shader() {}

Shader::Shader(const std::string& vshader_file, const std::string& fshader_file) {
  load_from(vshader_file, fshader_file);
}

Shader::~Shader() {}

GLuint Shader::get_program() { return program_; }

void Shader::load(const std::string& vshader_code, const std::string& fshader_code) {
  GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vshader_code);
  GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fshader_code);

  program_ = create_shader_program(vertex_shader, fragment_shader);

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}

void Shader::load_from(const std::string& vshader_file, const std::string& fshader_file) {
  load(read_file(vshader_file), read_file(fshader_file));
}

void Shader::unload() {
  if (program_ != 0) glDeleteProgram(program_);
}

void Shader::use() { glUseProgram(program_); }

void Shader::set_uniform(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(program_, name.c_str()), value);
}
