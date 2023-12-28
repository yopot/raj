#ifndef RAJ_LINEAR_LAYOUT
#define RAJ_LINEAR_LAYOUT

#include "rectNode.hpp"
#include "rajScrollBar.hpp"

class linearLayout : public rectNode
{
public:
 inline static uint8_t stencilVal = 1;
 uint8_t stencilId = 0;
 rajScrollBar scrollBar;
 float dy = 0, maxDy = 0;

 void initLinearLayout(rectNode *parent, rectNodeOrigin o,
                       std::string w, std::string h,
                       float padl, float padr, float padb, float padt,
                       float r, float g, float b);
 void findWidth();
 void findHeight();
 void findDimension();
 void findScrollables();
 void calcChildsOrigin();
 void draw();
 #ifdef RAJ_DEBUG
 void printDimension(uint32_t);
 void printOrigin(uint32_t);
 #endif
};

#endif