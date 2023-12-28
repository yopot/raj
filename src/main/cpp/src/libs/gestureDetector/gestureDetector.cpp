#include "gestureDetector.hpp"

void gestureDetector::SetConfiguration(AConfiguration *config)
{
 dp_factor_ = 160.f / AConfiguration_getDensity(config);
}