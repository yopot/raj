#include "dragDetector.hpp"

GESTURE_STATE dragDetector::detect(const AInputEvent *motion_event)
{
 if (AMotionEvent_getPointerCount(motion_event) > 1)
  return false;

 int32_t action = AMotionEvent_getAction(motion_event);
 unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;

 switch (flags)
 {
 case AMOTION_EVENT_ACTION_MOVE:
  down_pointer_id_ = AMotionEvent_getPointerId(motion_event, 0);
  down_x_ = AMotionEvent_getX(motion_event, 0);
  down_y_ = AMotionEvent_getY(motion_event, 0);
  return GESTURE_STATE_MOVE;
  break;
 }

 return GESTURE_STATE_NONE;
}