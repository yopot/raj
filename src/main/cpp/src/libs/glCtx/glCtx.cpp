#include "glCtx.hpp"
#include <android/log.h>
#ifdef FOR_RAJ_ANDROID
#include <GLES3/gl3.h>
#else
#include <GL/freeglut.h>
#endif

#define LOGI(...)                                            \
 {                                                           \
                                                             \
  __android_log_print(ANDROID_LOG_INFO, "raj", __VA_ARGS__); \
 }

void glCtx::
init()
{
 initEGLContext();
}

void glCtx::
suspend()
{
 if (surface != EGL_NO_SURFACE)
 {
  eglDestroySurface(disp, surface);
  surface = EGL_NO_SURFACE;
 }
 LOGI("glCtx.suspend()")
}

void glCtx::
resume(ANativeWindow *window)
{
 this->window = window;
 initEGLSurface();
 glEnable(GL_DEPTH_TEST);
 glEnable(GL_STENCIL_TEST);
 glClearColor(0, 0, 0, 0);
 glClearStencil(0);
 LOGI("glCtx.resume()")
}

void glCtx::
terminate()
{
 if (disp != EGL_NO_DISPLAY)
 {
  eglMakeCurrent(disp, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  if (ctx != EGL_NO_CONTEXT)
  {
   eglDestroyContext(disp, ctx);
  }

  if (surface != EGL_NO_SURFACE)
  {
   eglDestroySurface(disp, surface);
  }
  eglTerminate(disp);
 }

 disp = EGL_NO_DISPLAY;
 ctx = EGL_NO_CONTEXT;
 surface = EGL_NO_SURFACE;
 LOGI("glctx terminate()")
}

void glCtx::
swap()
{
 eglSwapBuffers(disp, surface);
}

void glCtx::
initEGLContext()
{
 // get disp connection
 disp = eglGetDisplay(EGL_DEFAULT_DISPLAY);

 // initialize the EGL disp connection
 eglInitialize(disp, 0, 0);

 const EGLint attribs[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
                           EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                           EGL_BLUE_SIZE, 8,
                           EGL_GREEN_SIZE, 8,
                           EGL_RED_SIZE, 8,
                           EGL_DEPTH_SIZE, 24,
                           EGL_STENCIL_SIZE, 8,
                           EGL_NONE};

 // get num of configs matching given attr
 EGLint numConfigs = 0;
 eglChooseConfig(disp, attribs, nullptr, 0, &numConfigs);

 // get an appropriate EGL frame buffer configurations
 EGLConfig supportedConfigs[numConfigs];
 eglChooseConfig(disp, attribs, supportedConfigs, numConfigs, &numConfigs);

 // find fb configs that have given attrs & store it in config
 size_t i = 0;
 for (; i < numConfigs; i++)
 {
  auto &cfg = supportedConfigs[i];
  EGLint r, g, b, d;
  if (eglGetConfigAttrib(disp, cfg, EGL_RED_SIZE, &r) &&
      eglGetConfigAttrib(disp, cfg, EGL_GREEN_SIZE, &g) &&
      eglGetConfigAttrib(disp, cfg, EGL_BLUE_SIZE, &b) &&
      eglGetConfigAttrib(disp, cfg, EGL_DEPTH_SIZE, &d) &&
      r == 8 && g == 8 && b == 8 && d == 24)
  {

   config = supportedConfigs[i];
   break;
  }
 }

 const EGLint ctxattribs[] = {EGL_CONTEXT_MAJOR_VERSION, 3,
                              EGL_CONTEXT_MINOR_VERSION, 2,
                              EGL_NONE};
 /* create an EGL rendering context */
 ctx = eglCreateContext(disp, config, nullptr, ctxattribs);

 LOGI("inited egl ctx")
}

void glCtx::initEGLSurface()
{
 // create egl surface
 surface = eglCreateWindowSurface(disp, config, window, nullptr);
 // attach ctx to draw & read surface
 eglMakeCurrent(disp, surface, surface, ctx);

 LOGI("inited egl surface")
}

glCtx::~glCtx()
{
 terminate();
}