#ifndef SHADER_H
#define SHADER_H

#include "utils.h"

#include <GLES2/gl2.h>

#include <string>
#include <vector>

class Shader {
public:
  Shader();

  Shader(const std::string& vshader_code, const std::string& fshader_code);

  ~Shader();

  GLuint get_program();

  void load(const std::string& vshader_code, const std::string& fshader_code);

  void load_from(const std::string& vshader_file, const std::string& fshader_file);

  void unload();

  void use();

private:
  GLuint program_ = 0;
};

#endif  // SHADER_H
