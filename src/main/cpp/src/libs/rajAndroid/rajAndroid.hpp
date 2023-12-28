#ifndef RAJ_ANDROID
#define RAJ_ANDROID

#include <android/native_activity.h>
#include <android/configuration.h>
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <thread>
#include <atomic>
#include <mutex>
#include <cstdint>
#include "rajQ.hpp"
#include "glCtx.hpp"
#include "tapDetector.hpp"
#include "dragDetector.hpp"

// logiMutex.lock();
// logiMutex.unlock();
// app->logiMutex.lock();
// app->logiMutex.unlock();

#ifndef NDEBUG
#define LOGI(...)                                            \
 {                                                           \
                                                             \
  __android_log_print(ANDROID_LOG_INFO, "myApp", __VA_ARGS__); \
 }

#define LOGIO(...)                                           \
 {                                                           \
                                                             \
  __android_log_print(ANDROID_LOG_INFO, "rajAndroid", __VA_ARGS__); \
 }
#else
#define LOGI(...)
#endif

class rajAndroid
{
public:
 rajQ rajAppQ;
 glCtx ctx;
 std::thread backThread;
 std::atomic<void *> activity; // ANativActivity
 std::atomic<void *> aConfig;  // AConfiguration
 std::atomic<void *> nWindow;  // ANativeWindow
 JavaVM *lJavaVM = nullptr;
 JNIEnv *lJNIEnv = nullptr;
 jobject lNativeActivity;
 jclass ClassNativeActivity;
 jfloat xdpi, ydpi;
 tapDetector aTap;
 dragDetector aDrag;
 struct mArect
 {
  std::atomic_int32_t left;
  std::atomic_int32_t top;
  std::atomic_int32_t right;
  std::atomic_int32_t bottom;
 } cntRect;
 std::atomic<void *> inQ; // AInputQueue
 // should filled using malloc
 std::atomic<void *> savedState;
 std::atomic_size_t savedStateSize;
 std::atomic_flag isStateSaved;
 int32_t widthPx;
 int32_t heightPx;
 float widthMm;
 float heightMm;
 bool isInputQReady = false;
#ifndef NDEBUG
 std::mutex logiMutex;
#endif

 virtual bool onInputEvent(AInputEvent *event) { return true; }
 virtual void onConfigChanged() {}
 virtual void onContentRectChanged() {}
 virtual void onLowMemory() {}
 virtual void onWindowCreated() {}
 virtual void onWindowDestroyed() {}
 virtual void onWindowRedrawNeeded() {}
 virtual void onWindowResized() {}
 virtual void onPause() {}
 virtual void onResume() {}
 virtual void onSaveState() {}
 virtual void onCreate() {}
 virtual void onStart() {}
 virtual void onStop() {}
 virtual void onGainedFocus() {}
 virtual void onLostFocus() {}
 virtual void onLooping() {}
 void freeSavedState();
 void backMain();
 rajAndroid();
 virtual ~rajAndroid();
};

#endif