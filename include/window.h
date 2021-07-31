#ifndef WINDOW_H
#define WINDOW_H

#include "bcm_host.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>

class Window {
public:
  Window(int width = 0, int height = 0);

  ~Window();

  int width();

  int height();

  void init();

  void update();

private:
  /* create a Dispmanx window */
  void init_dispmanx();

  /* create EGL rendering context */
  void init_egl();

  /* create EGL drawing surface from Dispmanx window */
  void egl_from_dispmanx();

  EGLDisplay display_ = EGL_NO_DISPLAY;
  EGLSurface surface_ = EGL_NO_SURFACE;
  EGLContext context_ = EGL_NO_CONTEXT;
  EGLConfig config_;

  EGL_DISPMANX_WINDOW_T window_;
  DISPMANX_DISPLAY_HANDLE_T dispman_display_;
};

#endif  // WINDOW_H
