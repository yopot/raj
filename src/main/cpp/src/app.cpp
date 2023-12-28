#include "rajAndroid.hpp"
#include "rajProg.hpp"
#include "android/asset_manager.h"
#include "textNode.hpp"
#include "gltfLetters.hpp"
#include "linearLayout.hpp"
#define FOR_RAJ_ANDROID
#include "gltfLoader.hpp"
#include "android/window.h"
#include "jni.h"

class myApp : public rajAndroid
{
 public:
  linearLayout ll[10];
  textNode t[10];
  gltfLetters letters;
  rajProg prog;
  GLuint vao, vbo[2];
  bool is1stTime = true;
  bool shouldLoop = false;

  void onWindowCreated();
  void onWindowDestroyed();
  void onCreate();
  bool onInputEvent(AInputEvent* e);
  void onLooping();
  void displayKeyboard(bool pShow);
  ~myApp();
};

void myApp::onCreate()
{
 ANativeActivity *act = (ANativeActivity *)activity.load();
 gltfLoader::am = act->assetManager;
}

void myApp::
onWindowCreated()
{
 if(is1stTime)
 {
  is1stTime = false;
 letters.init("leftTopOrigin.glb");
 textNode::enLetters = &letters;
 rectNode::initProgram(widthMm, heightMm, widthPx, heightPx);
 letterNode::initProgram();
 rectNode::pxTommX = widthMm * 1.0 / widthPx;
 rectNode::pxTommY = heightMm * 1.0 / heightPx;
 rectNode::vWidthPx = widthPx;
 rectNode::vHeightPx = heightPx;

 ll[0].initLinearLayout(nullptr, rectNode::LT,
                        std::to_string(widthMm),
                        std::to_string(heightMm),
                        0, 0, 0, 5, 0, 0, 0);

 ll[1].initLinearLayout(&ll[0], rectNode::LT,
                        "100%", "100%",
                        1, 1, 1, 1, 0, 0, 0.5);
 t[0].initTextNode(&ll[1], rectNode::LT, "100%", "0", 2, 2, 2, 2, 0.25, 0.25, 0.25,
                   "I am yopot");
 t[1].initTextNode(&ll[1], rectNode::LT, "100%", "0", 2, 2, 2, 2, 0.3, 0.3, 0.3,
                   "change yopot to your name");
 t[2].initTextNode(&ll[1], rectNode::LT, "100%", "0", 2, 2, 2, 2, 0.35, 0.35, 0.35,
                   "thank you");

 ll[0].findDimension();
 ll[0].findScrollables();
 ll[0].calcChildsOrigin();
 }
 shouldLoop = true;
}

void myApp::
onLooping()
{
if (shouldLoop)
{
 glClearColor(1,0,0,1);
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
 ll[0].drawUi();
 ctx.swap();
}
}

void myApp::onWindowDestroyed()
{
 shouldLoop = false;
}

bool myApp::
onInputEvent(AInputEvent* e)
{
 
 if (AInputEvent_getType(e) == AINPUT_EVENT_TYPE_MOTION)
 {
  if (AMotionEvent_getPointerCount(e) == 1)
  {
   int32_t action = AMotionEvent_getAction(e);
   unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;

   switch (flags)
   {
   case AMOTION_EVENT_ACTION_DOWN:
    rectNode::processClick(true, AMotionEvent_getX(e, 0), heightPx - AMotionEvent_getY(e, 0), AMotionEvent_getY(e, 0));
    break;
   case AMOTION_EVENT_ACTION_UP:
   {
    rectNode::processClick(false, AMotionEvent_getX(e, 0), heightPx - AMotionEvent_getY(e, 0), AMotionEvent_getY(e, 0));
    displayKeyboard(true);
   }
    break;
   case AMOTION_EVENT_ACTION_MOVE:
    rectNode::processMouseMove(AMotionEvent_getX(e, 0), AMotionEvent_getY(e, 0));
    break;
   }
  }
 }
 

 return true;
}

myApp::~myApp()
{
 glDeleteVertexArrays(1, &vao);
 glDeleteBuffers(2, vbo);
}

rajAndroid* getRajAndroid()
{
 return new myApp();
}

void myApp::displayKeyboard(bool pShow)
{
 // Attaches the current thread to the JVM.
 jint lResult;
 jint lFlags = 0;

 JavaVM *lJavaVM = ((ANativeActivity*)activity.load())->vm;
 JNIEnv *lJNIEnv = ((ANativeActivity *)activity.load())->env;

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
 jobject lNativeActivity = ((ANativeActivity*)activity.load())->clazz;
 jclass ClassNativeActivity = lJNIEnv -> GetObjectClass(lNativeActivity);


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

//get xdpi & ydpi
 jclass ClassDisplayMetrics = lJNIEnv->FindClass(
  "android/util/DisplayMetrics");
 jfieldID fieldXDpi = lJNIEnv->GetFieldID(ClassDisplayMetrics, "xdpi", "F");
 jfieldID fieldYDpi = lJNIEnv->GetFieldID(ClassDisplayMetrics, "ydpi", "F");

 jfloat xdpi = lJNIEnv->GetFloatField(lDisplayMetrics, fieldXDpi);
 jfloat ydpi = lJNIEnv->GetFloatField(lDisplayMetrics, fieldYDpi);

 // Finished with the JVM.
 lJavaVM->DetachCurrentThread();
}