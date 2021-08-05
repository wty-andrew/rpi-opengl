cmake_minimum_required(VERSION 3.7)
project(rpi-opengl)
set(CMAKE_CXX_STANDARD 14)

include_directories(
  include
  ${CMAKE_SYSROOT}/opt/vc/include
  ${CMAKE_SYSROOT}/opt/vc/include/interface/vcos/pthreads
  ${CMAKE_SYSROOT}/opt/vc/include/interface/vmcs_host
  ${CMAKE_SYSROOT}/opt/vc/include/interface/vmcs_host/linux
)

link_directories(
  ${CMAKE_SYSROOT}/opt/vc/lib
)

add_library(common STATIC
  src/window.cpp
  src/shader.cpp
  src/utils.cpp
)
target_link_libraries(common
  bcm_host
  vcos
  vchiq_arm
  EGL
  GLESv2
)

add_executable(main src/main.cpp)
target_link_libraries(main
  GLESv2
  common
)
