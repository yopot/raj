#ifndef RAJ_SCROLL_BAR
#define RAJ_SCROLL_BAR

#include "rectNode.hpp"

class rajScrollBar
{
 public:
 rectNode* parent;
 rectNode scrollBody;
 rectNode scrollHandle;
 int pointerX = 0; 
 int pointerY = 0;
 float dy = 0, maxDy = 0;
 float factor = 0; // factor = distance b/w 1stchild's InitialPos & curPos / distance b/w scrollBodyOrigin & scrollHandleOrigin
 void initRajScrollBar(rectNode* parent);
 void findDimension();
 void findOrigin();
 void draw();
 void doScroll(int currPointerX, int currPointerY);
};

#endif