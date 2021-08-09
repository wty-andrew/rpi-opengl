#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdexcept>

GLuint Texture::load(const std::string& img_file) {
  int width, height, channels;
  stbi_set_flip_vertically_on_load(true);
  GLubyte* data = stbi_load(img_file.c_str(), &width, &height, &channels, STBI_rgb_alpha);
  if (data == NULL) throw std::runtime_error("Failed to load texture: " + img_file);

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  stbi_image_free(data);
  return texture;
}
