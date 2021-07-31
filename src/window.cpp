#include "window.h"

#include <cassert>

Window::Window(int width, int height) {
  bcm_host_init();

  if (width == 0 && height == 0) {
    uint32_t screen_width;
    uint32_t screen_height;

    int32_t ret = 0;
    ret = graphics_get_display_size(0, &screen_width, &screen_height);
    assert(ret >= 0);

    window_.width = screen_width;
    window_.height = screen_height;
  } else {
    assert(width > 0 && height > 0);
    window_.width = width;
    window_.height = height;
  }
}

Window::~Window() {
  if (display_ != EGL_NO_DISPLAY)
    eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

  if (surface_ != EGL_NO_SURFACE) eglDestroySurface(display_, surface_);

  if (context_ != EGL_NO_CONTEXT) eglDestroyContext(display_, context_);

  if (display_ != EGL_NO_DISPLAY) eglTerminate(display_);

  vc_dispmanx_display_close(dispman_display_);
  bcm_host_deinit();
}

int Window::width() { return window_.width; }

int Window::height() { return window_.height; }

void Window::init() {
  init_dispmanx();
  init_egl();
  egl_from_dispmanx();
}

void Window::update() { eglSwapBuffers(display_, surface_); }

void Window::init_dispmanx() {
  VC_RECT_T dst_rect;
  VC_RECT_T src_rect;

  dst_rect.x = 0;
  dst_rect.y = 0;
  dst_rect.width = window_.width;
  dst_rect.height = window_.height;

  src_rect.x = 0;
  src_rect.y = 0;
  src_rect.width = window_.width << 16;
  src_rect.height = window_.height << 16;

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

  dispman_display_ = dispman_display;
  window_.element = dispman_element;
  vc_dispmanx_update_submit_sync(dispman_update);
}

void Window::init_egl() {
  static const EGLint config_attributes[] = { EGL_RED_SIZE,     8,
                                              EGL_GREEN_SIZE,   8,
                                              EGL_BLUE_SIZE,    8,
                                              EGL_ALPHA_SIZE,   8,
                                              EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                                              EGL_NONE };

  static const EGLint context_attributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

  display_ = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  assert(display_ != EGL_NO_DISPLAY);

  EGLBoolean result;
  result = eglInitialize(display_, NULL, NULL);
  assert(result != EGL_FALSE);

  EGLint num_config;
  result = eglChooseConfig(display_, config_attributes, &config_, 1, &num_config);
  assert(result != EGL_FALSE);

  result = eglBindAPI(EGL_OPENGL_ES_API);
  assert(result != EGL_FALSE);

  context_ = eglCreateContext(display_, config_, EGL_NO_CONTEXT, context_attributes);
  assert(context_ != EGL_NO_CONTEXT);
}

void Window::egl_from_dispmanx() {
  surface_ = eglCreateWindowSurface(display_, config_, &window_, NULL);
  assert(surface_ != EGL_NO_SURFACE);

  EGLBoolean result = eglMakeCurrent(display_, surface_, surface_, context_);
  assert(result != EGL_FALSE);
}
