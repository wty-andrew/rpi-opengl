#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLES2/gl2.h>

#include <string>

class Texture {
public:
  static GLuint load(const std::string& img_file);
};

#endif  // TEXTURE_H
