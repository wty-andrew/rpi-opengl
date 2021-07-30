#include "bcm_host.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2.h>

#include <cassert>
#include <chrono>
#include <thread>

int main(int argc, char** argv) {
  bcm_host_init();

  uint32_t screen_width;
  uint32_t screen_height;

  int32_t ret = 0;
  ret = graphics_get_display_size(0, &screen_width, &screen_height);
  assert(ret >= 0);

  /* create a Dispmanx window */
  VC_RECT_T dst_rect;
  VC_RECT_T src_rect;

  dst_rect.x = 0;
  dst_rect.y = 0;
  dst_rect.width = screen_width;
  dst_rect.height = screen_height;

  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.width = screen_width << 16;
  src_rect.height = screen_height << 16;

  DISPMANX_DISPLAY_HANDLE_T dispman_display = vc_dispmanx_display_open(0);
  DISPMANX_UPDATE_HANDLE_T dispman_update = vc_dispmanx_update_start(0);
  DISPMANX_ELEMENT_HANDLE_T dispman_element = vc_dispmanx_element_add(
    dispman_update,
    dispman_display,
    0,
    &dst_rect,
    0,
    &src_rect,
    DISPMANX_PROTECTION_NONE,
    0,
    0,
    DISPMANX_NO_ROTATE);

  EGL_DISPMANX_WINDOW_T window;
  window.element = dispman_element;
  window.width = screen_width;
  window.height = screen_height;
  vc_dispmanx_update_submit_sync(dispman_update);

  /* create EGL rendering context */
  static const EGLint config_attributes[] = { EGL_RED_SIZE,     8,
                                              EGL_GREEN_SIZE,   8,
                                              EGL_BLUE_SIZE,    8,
                                              EGL_ALPHA_SIZE,   8,
                                              EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                                              EGL_NONE };

  static const EGLint context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

  EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  assert(display != EGL_NO_DISPLAY);

  EGLBoolean result;
  result = eglInitialize(display, NULL, NULL);
  assert(result != EGL_FALSE);

  EGLConfig config;
  EGLint num_config;
  result = eglChooseConfig(display, config_attributes, &config, 1, &num_config);
  assert(result != EGL_FALSE);

  result = eglBindAPI(EGL_OPENGL_ES_API);
  assert(result != EGL_FALSE);

  EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
  assert(context != EGL_NO_CONTEXT);

  /* create EGL drawing surface from Dispmanx window */
  EGLSurface surface = eglCreateWindowSurface(display, config, &window, NULL);
  assert(surface != EGL_NO_SURFACE);

  result = eglMakeCurrent(display, surface, surface, context);
  assert(result != EGL_FALSE);

  // drawing with OpenGL
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // render
  eglSwapBuffers(display, surface);

  std::this_thread::sleep_for(std::chrono::seconds(3));

  return 0;
}
