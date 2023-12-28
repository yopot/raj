#pragma once

#include "gestureDetector.hpp"

class tapDetector : public gestureDetector
{
public:
 int32_t down_pointer_id_;
 float down_x_;
 float down_y_;
 virtual GESTURE_STATE detect(const AInputEvent *motion_event);
};