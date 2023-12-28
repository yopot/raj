#include <android/configuration.h>
#include <android/log.h>
#include <android/native_window.h>
#include <thread>
#include <cstdlib>
#include <cstring>
#include "rajAndroid.hpp"
#include "rajErr.hpp"

enum
{
 /**
  * Command from main thread: the AInputQueue has changed.  Upon processing
  * this command, android_app->inputQueue will be updated to the new queue
  * (or NULL).
  */
 RAJ_CMD_INPUT_CREATED = 0,
 RAJ_CMD_INPUT_DESTROYED,

 /**
  * Command from main thread: a new ANativeWindow is ready for use.  Upon
  * receiving this command, android_app->window will contain the new window
  * surface.
  */
 RAJ_CMD_WINDOW_CREATED,

 /**
  * Command from main thread: the existing ANativeWindow needs to be
  * terminated.  Upon receiving this command, android_app->window still
  * contains the existing window; after calling android_app_exec_cmd
  * it will be set to NULL.
  */
 RAJ_CMD_WINDOW_DESTROYED,

 /**
  * Command from main thread: the current ANativeWindow has been resized.
  * Please redraw with its new size.
  */
 RAJ_CMD_WINDOW_RESIZED,

 /**
  * Command from main thread: the system needs that the current ANativeWindow
  * be redrawn.  You should redraw the window before handing this to
  * android_app_exec_cmd() in order to avoid transient drawing glitches.
  */
 RAJ_CMD_WINDOW_REDRAW_NEEDED,

 /**
  * Command from main thread: the content area of the window has changed,
  * such as from the soft input window being shown or hidden.  You can
  * find the new content rect in android_app::contentRect.
  */
 RAJ_CMD_CONTENT_RECT_CHANGED,

 /**
  * Command from main thread: the app's activity window has gained
  * input focus.
  */
 RAJ_CMD_GAINED_FOCUS,

 /**
  * Command from main thread: the app's activity window has lost
  * input focus.
  */
 RAJ_CMD_LOST_FOCUS,

 /**
  * Command from main thread: the current device configuration has changed.
  */
 RAJ_CMD_CONFIG_CHANGED,

 /**
  * Command from main thread: the system is running low on memory.
  * Try to reduce your memory use.
  */
 RAJ_CMD_LOW_MEMORY,

 /**
  * Command from main thread: the app's activity has been started.
  */
 RAJ_CMD_START,
 RAJ_CMD_CREATE,

 /**
  * Command from main thread: the app's activity has been resumed.
  */
 RAJ_CMD_RESUME,

 /**
  * Command from main thread: the app should generate a new saved state
  * for itself, to restore from later if needed.  If you have saved state,
  * allocate it with malloc and place it in android_app.savedState with
  * the size in android_app.savedStateSize.  The will be freed for you
  * later.
  */
 RAJ_CMD_SAVE_STATE,

 /**
  * Command from main thread: the app's activity has been paused.
  */
 RAJ_CMD_PAUSE,

 /**
  * Command from main thread: the app's activity has been stopped.
  */
 RAJ_CMD_STOP,

 /**
  * Command from main thread: the app's activity is being destroyed,
  * and waiting for the app thread to clean up and exit before proceeding.
  */
 RAJ_CMD_DESTROY,
};

rajAndroid::
rajAndroid()
{
 LOGIO("rajAndroid()")
 backThread = std::thread(&rajAndroid::backMain, this);
}

rajAndroid::
~rajAndroid()
{
 LOGIO("~rajAndroid()");
 freeSavedState();
 AConfiguration_delete((AConfiguration *)aConfig.load());
}

void rajAndroid::
freeSavedState()
{
 LOGIO("freeSavedState()")
 if (savedState != nullptr)
 {
  free(savedState);
  savedState = nullptr;
  savedStateSize = 0;
 }
}

void rajAndroid::
backMain()
{
 try
 {
  while (true)
  {

   if (!rajAppQ.isEmpty())
   {
    switch (rajAppQ.pop())
    {
    case RAJ_CMD_CREATE:
    {
     ctx.init();
     // Attaches the current thread to the JVM.
     jint lResult;
     jint lFlags = 0;

     lJavaVM = ((ANativeActivity *)activity.load())->vm;
     lJNIEnv = ((ANativeActivity *)activity.load())->env;

     /*
      JNIEnv *env;
      bool attached = false;
      switch (vm->GetEnv((void **)&env, JNI_VERSION_1_6))
      {
      case JNI_OK:
       break;
      case JNI_EDETACHED:
       if (vm->AttachCurrentThread(&env, NULL) != 0)
       {
        throw std::runtime_error("Could not attach current thread");
       }
       attached = true;
       break;
      case JNI_EVERSION:
       throw std::runtime_error("Invalid java version");
      }
      */


     JavaVMAttachArgs lJavaVMAttachArgs;
     lJavaVMAttachArgs.version = JNI_VERSION_1_6;
     lJavaVMAttachArgs.name = "NativeThread";
     lJavaVMAttachArgs.group = NULL;

     lResult = lJavaVM->AttachCurrentThread(&lJNIEnv,
                                            &lJavaVMAttachArgs);
     if (lResult == JNI_ERR)
     {
      return;
     }

     // Retrieves NativeActivity.
     lNativeActivity = ((ANativeActivity *)activity.load())->clazz;
    ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);

     // call getResources().getDisplayMetrics()
     jmethodID MethodGetResources = lJNIEnv->GetMethodID(
         ClassNativeActivity, "getResources",
         "()Landroid/content/res/Resources;");
     jobject lgetResources = lJNIEnv->CallObjectMethod(
         lNativeActivity, MethodGetResources);

     jclass ClassResources = lJNIEnv->FindClass(
         "android/content/res/Resources");
     jmethodID MethodGetDisplayMetrics = lJNIEnv->GetMethodID(
         ClassResources, "getDisplayMetrics",
         "()Landroid/util/DisplayMetrics;");
     jobject lDisplayMetrics = lJNIEnv->CallObjectMethod(
         lgetResources, MethodGetDisplayMetrics);

     // get xdpi & ydpi
     jclass ClassDisplayMetrics = lJNIEnv->FindClass(
         "android/util/DisplayMetrics");
     jfieldID fieldXDpi = lJNIEnv->GetFieldID(ClassDisplayMetrics, "xdpi", "F");
     jfieldID fieldYDpi = lJNIEnv->GetFieldID(ClassDisplayMetrics, "ydpi", "F");

     xdpi = lJNIEnv->GetFloatField(lDisplayMetrics, fieldXDpi);
     ydpi = lJNIEnv->GetFloatField(lDisplayMetrics, fieldYDpi);
     
     onCreate();
    }
    break;
    case RAJ_CMD_START:
     onStart();
     break;
    case RAJ_CMD_RESUME:
    {
     onResume();
     freeSavedState();
    }
    break;
    case RAJ_CMD_INPUT_CREATED:
    {
     isInputQReady = true;
     LOGIO("RAJ_CMD_INPUT_CREATED");
    }
    break;
    case RAJ_CMD_WINDOW_CREATED:
    {
     ctx.resume((ANativeWindow *)nWindow.load());
     widthPx = ANativeWindow_getWidth((ANativeWindow*)nWindow.load());
     heightPx = ANativeWindow_getHeight((ANativeWindow*)nWindow.load());
     //1 inch = 25.4 mm
     widthMm = widthPx / (float)xdpi * 25.4;
     heightMm = heightPx / (float)ydpi * 25.4;

     onWindowCreated();
     isInputQReady = true;
    }
    break;
    case RAJ_CMD_WINDOW_RESIZED:
     onWindowResized();
     break;
    case RAJ_CMD_CONTENT_RECT_CHANGED:
     onContentRectChanged();
     break;
    case RAJ_CMD_WINDOW_REDRAW_NEEDED:
     onWindowRedrawNeeded();
     break;
    case RAJ_CMD_GAINED_FOCUS:
     onGainedFocus();
     break;
    case RAJ_CMD_LOST_FOCUS:
     onLostFocus();
     break;
    case RAJ_CMD_PAUSE:
     onPause();
     break;
    case RAJ_CMD_WINDOW_DESTROYED:
    {
     onWindowDestroyed();
     ctx.suspend();
     isInputQReady = false;
     nWindow = nullptr;
    }
    break;
    case RAJ_CMD_STOP:
     onStop();
     break;
    case RAJ_CMD_SAVE_STATE:
    {
     freeSavedState();
     onSaveState();
     isStateSaved.test_and_set();
     isStateSaved.notify_one();
    }
    break;
    case RAJ_CMD_CONFIG_CHANGED:
    {
     AConfiguration_fromAssetManager((AConfiguration *)aConfig.load(), ((ANativeActivity *)activity.load())->assetManager);
     onConfigChanged();
    }
    break;
    case RAJ_CMD_LOW_MEMORY:
    {
     onLowMemory();
    }
    break;
    case RAJ_CMD_INPUT_DESTROYED:
    {
     isInputQReady = false;
     inQ = nullptr;
     LOGIO("RAJ_CMD_INPUT_DESTROYED");
    }
    break;
    case RAJ_CMD_DESTROY:
    {
     LOGIO("RAJ CMD DESTROY");
     lJavaVM->DetachCurrentThread();
    }
     return;
    default:
     break;
    }
   }

   if (isInputQReady)
   {
    if (AInputQueue_hasEvents((AInputQueue *)inQ.load()) == 1)
    {
     LOGIO("has events")
     AInputEvent *event = NULL;
     while (AInputQueue_getEvent((AInputQueue *)inQ.load(), &event) >= 0)
     {
      if (AInputQueue_preDispatchEvent((AInputQueue *)inQ.load(), event))
      {
       continue;
      }
      bool handled = false;

      handled = onInputEvent(event);
      AInputQueue_finishEvent((AInputQueue *)inQ.load(), event, handled ? 1 : 0);
     }
    }
   }

   onLooping();
  }
 }
 catch (rajErr &e)
 {
  LOGIO("~~~rajErr~~~: %s", e.msg.data())
  ANativeActivity_finish((ANativeActivity *)activity.load());
 }
 catch (const std::exception &e)
 {
  LOGIO("~~~c++ err~~~:\n%s", e.what())
  ANativeActivity_finish((ANativeActivity *)activity.load());
 }
}

rajAndroid *app;
rajAndroid *getRajAndroid();

static void onStart(ANativeActivity *activity)
{
 LOGIO("-----onStart: %p\n", activity);
 app->rajAppQ.push(RAJ_CMD_START);
 // printInfo(activity);
}

static void onResume(ANativeActivity *activity)
{
 LOGIO("-----onResume: %p\n", activity);
 app->rajAppQ.push(RAJ_CMD_RESUME);
}

static void *onSaveInstanceState(ANativeActivity *activity, size_t *outLen)
{
 LOGIO("-----onSaveInstanceState: %p, %d\n", activity, *outLen);

 app->rajAppQ.push(RAJ_CMD_SAVE_STATE);

 app->isStateSaved.wait(false);
 app->isStateSaved.clear();

 LOGIO("after state saved");
 void *savedState = nullptr;

 if (app->savedState != nullptr)
 {
  LOGIO("savedState has data");
  savedState = app->savedState;
  *outLen = app->savedStateSize;
  app->savedState = nullptr;
  app->savedStateSize = 0;
 }

 return savedState;
}

static void onPause(ANativeActivity *activity)
{
 LOGIO("-----onPause: %p\n", activity);
 app->rajAppQ.push(RAJ_CMD_PAUSE);
}

static void onStop(ANativeActivity *activity)
{
 LOGIO("-----onStop: %p\n", activity);
 app->rajAppQ.push(RAJ_CMD_STOP);
}

static void onDestroy(ANativeActivity *activity)
{
 LOGIO("-----onDestroy: %p\n", activity);
 app->rajAppQ.push(RAJ_CMD_DESTROY);
 app->backThread.join();
 delete app;
 //	printInfo(activity);
}

static void onWindowFocusChanged(ANativeActivity *activity, int focused)
{
 LOGIO("-----onWindowFocusChanged: %p -- %d\n", activity, focused);
 app->rajAppQ.push(focused ? RAJ_CMD_GAINED_FOCUS : RAJ_CMD_LOST_FOCUS);
}

static void onNativeWindowCreated(ANativeActivity *activity, ANativeWindow *window)
{
 LOGIO("-----onNativeWindowCreated: %p -- %p\n", activity, window);
 app->nWindow = window;
 app->rajAppQ.push(RAJ_CMD_WINDOW_CREATED);
}

static void onNativeWindowResized(ANativeActivity *activity, ANativeWindow *window)
{
 LOGIO("-----onNativeWindowResized: %p -- %p\n", activity, window);
 app->rajAppQ.push(RAJ_CMD_WINDOW_RESIZED);
}

static void onNativeWindowRedrawNeeded(ANativeActivity *activity, ANativeWindow *window)
{
 LOGIO("-----onNativeWindowRedrawNeeded: %p -- %p\n", activity, window);
 app->rajAppQ.push(RAJ_CMD_WINDOW_REDRAW_NEEDED);
}

static void onNativeWindowDestroyed(ANativeActivity *activity, ANativeWindow *window)
{
 LOGIO("-----onNativeWindowDestroyed: %p -- %p\n", activity, window);
 app->rajAppQ.push(RAJ_CMD_WINDOW_DESTROYED);
}

static void onInputQueueCreated(ANativeActivity *activity, AInputQueue *queue)
{
 LOGIO("-----onInputQueueCreated: %p -- %p\n", activity, queue);
 app->inQ = queue;
 app->rajAppQ.push(RAJ_CMD_INPUT_CREATED);
}

static void onInputQueueDestroyed(ANativeActivity *activity, AInputQueue *queue)
{
 LOGIO("-----onInputQueueDestroyed: %p -- %p\n", activity, queue);
 app->rajAppQ.push(RAJ_CMD_INPUT_DESTROYED);
}

static void onContentRectChanged(ANativeActivity *activity, const ARect *rect)
{
 LOGIO("-----onContentRectChanged: %p -- %p\n", activity, rect);
 app->cntRect.bottom = rect->bottom;
 app->cntRect.top = rect->top;
 app->cntRect.left = rect->left;
 app->cntRect.right = rect->right;
 app->rajAppQ.push(RAJ_CMD_CONTENT_RECT_CHANGED);
}

static void onConfigurationChanged(ANativeActivity *activity)
{
 LOGIO("-----onConfigurationChanged: %p\n", activity);
 // AConfiguration_fromAssetManager((AConfiguration *)app->aConfig.load(), activity->assetManager);
 app->rajAppQ.push(RAJ_CMD_CONFIG_CHANGED);
}

static void onLowMemory(ANativeActivity *activity)
{
 LOGIO("-----onLowMemory: %p\n", activity);
 app->rajAppQ.push(RAJ_CMD_LOW_MEMORY);
}

void ANativeActivity_onCreate(ANativeActivity *activity,
                              void *savedState, size_t savedStateSize)
{
 app = getRajAndroid();
 LOGIO("-----ANativeActivity_onCreate %p", activity);
 app->isStateSaved.clear();
 app->activity = activity;
 app->aConfig = AConfiguration_new();
 AConfiguration_fromAssetManager((AConfiguration *)app->aConfig.load(), activity->assetManager);
 if (savedState != nullptr)
 {
  LOGIO("savedState has data");
  app->savedState = malloc(savedStateSize);
  app->savedStateSize = savedStateSize;
  memcpy(app->savedState, savedState, savedStateSize);
 }

 app->rajAppQ.push(RAJ_CMD_CREATE);

 activity->callbacks->onStart = onStart;
 activity->callbacks->onResume = onResume;
 activity->callbacks->onSaveInstanceState = onSaveInstanceState;
 activity->callbacks->onPause = onPause;
 activity->callbacks->onStop = onStop;
 activity->callbacks->onDestroy = onDestroy;
 activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
 activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
 activity->callbacks->onNativeWindowResized = onNativeWindowResized;
 activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
 activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
 activity->callbacks->onInputQueueCreated = onInputQueueCreated;
 activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
 activity->callbacks->onContentRectChanged = onContentRectChanged;
 activity->callbacks->onConfigurationChanged = onConfigurationChanged;
 activity->callbacks->onLowMemory = onLowMemory;
 activity->instance = nullptr;
}
