#include "utils.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::string read_file(const std::string& filepath) {
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  file.open(filepath);

  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
