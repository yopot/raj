#ifndef RAJ_GL_CONTEXT
#define RAJ_GL_CONTEXT

#include <EGL/egl.h>
#include <android/native_window.h>

class glCtx
{
public:
 void init();
 void swap();

 void suspend();
 void resume(ANativeWindow *window);
 ~glCtx();

 // EGL configurations
 ANativeWindow *window;
 EGLDisplay disp;
 EGLSurface surface;
 EGLContext ctx;
 EGLConfig config;

private:
 void initEGLContext();
 void initEGLSurface();
 void terminate();
};

#endif