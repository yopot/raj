#pragma once

#include <cstdint>
#include <android/input.h>
#include <android/configuration.h>

const uint32_t DOUBLE_TAP_TIMEOUT = 300 * 1000000;
const uint32_t TAP_TIMEOUT = 180 * 1000000;
const uint8_t DOUBLE_TAP_SLOP = 100;
const uint8_t TOUCH_SLOP = 8;

enum
{
 GESTURE_STATE_NONE = 0,
 GESTURE_STATE_START = 1,
 GESTURE_STATE_MOVE = 2,
 GESTURE_STATE_END = 4,
 GESTURE_STATE_ACTION = (GESTURE_STATE_START | GESTURE_STATE_END),
};
typedef uint8_t GESTURE_STATE;

class gestureDetector
{
public:
 virtual void SetConfiguration(AConfiguration *config);
 virtual GESTURE_STATE detect(const AInputEvent *motion_event) = 0;
protected:
 float dp_factor_ = 1.0f;
};