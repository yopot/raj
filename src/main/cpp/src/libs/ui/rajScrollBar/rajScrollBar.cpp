#include "rajScrollBar.hpp"
#include "linearLayout.hpp"
#ifdef FOR_RAJ_ANDROID
#include "rajAndroid.hpp"
#endif

void rajScrollBar::
initRajScrollBar(rectNode* parent)
{
 this->parent = parent;
#ifdef FOR_RAJ_ANDROID
 scrollBody.givenWidth = "9"; // 9mm
#else 
 scrollBody.givenWidth = "20"; // 20mm = 2cm
#endif
 scrollBody.zAxis = parent->zAxis - 0.0001;
 scrollBody.r = 0.5;
 scrollBody.g = 0.5;
 scrollBody.b = 0.5;
 scrollHandle.r = 0.9;
 scrollHandle.g = 0.9;
 scrollHandle.b = 0.9;
 scrollHandle.zAxis = scrollBody.zAxis - 0.0001;
#ifdef FOR_RAJ_ANDROID
 scrollHandle.givenHeight = "9";
#else
 scrollHandle.givenHeight = "18";
#endif
 scrollHandle.type = rectNode::rectNodeType::ScrollBarType;
 scrollHandle.parent = parent;
 scrollHandle.clickable = true;
 rectNode::allNodes.push_back(&scrollHandle);
}

void rajScrollBar::
findDimension()
{
 scrollBody.width = std::atof(scrollBody.givenWidth.data());
 scrollBody.height = parent->height - parent->padt - parent->padb;
 scrollHandle.width = scrollBody.width;
 scrollHandle.height = std::atof(scrollHandle.givenHeight.data());
 if (((linearLayout *)parent)->maxDy < scrollBody.height)
 {
  float tmpHeight = scrollBody.height - ((linearLayout *)parent)->maxDy;
  if(tmpHeight > scrollHandle.height) // if tmpHeight is not less than default height
  scrollHandle.height = tmpHeight;
 }
 maxDy = scrollBody.height - scrollHandle.height;
 factor = ((linearLayout *)parent)->maxDy / maxDy;

}

void rajScrollBar::
findOrigin()
{
 scrollBody.origin.x = parent->origin.x + parent->width - parent->padr;
 scrollBody.origin.y = parent->origin.y - parent->padt;

 scrollHandle.origin = scrollBody.origin;
 scrollHandle.origin.y -= dy;
}

void rajScrollBar::
draw()
{
 // scrollBody
 glUniform2f(1, scrollBody.width, scrollBody.height);
 glUniform2f(2, scrollBody.origin.x, scrollBody.origin.y);
 glUniform2f(4, -1, 1);
 glUniform1f(5, scrollBody.zAxis);
  glUniform3f(6, scrollBody.r, scrollBody.g, scrollBody.b);
 scrollBody.msh[rectNode::rectNodeOrigin::LT].draw();

 //scrollHandle
 glUniform2f(1, scrollHandle.width, scrollHandle.height);
 glUniform2f(2, scrollHandle.origin.x, scrollHandle.origin.y);
 glUniform2f(4, -1, 1);
 glUniform1f(5, scrollHandle.zAxis);
 glUniform3f(6, scrollHandle.r, scrollHandle.g, scrollHandle.b);
 scrollHandle.msh[rectNode::rectNodeOrigin::LT].draw();
}

void rajScrollBar::
doScroll(int currPointerX, int currPointerY)
{
 // moved down
 if (currPointerY > pointerY)
 {
  if (dy < maxDy)
  {
   float mmMoved = (currPointerY - pointerY) * rectNode::pxTommY;

   if (dy + mmMoved > maxDy)
   {
    dy = maxDy;
    ((linearLayout *)parent)->dy = ((linearLayout *)parent)->maxDy;
   }
   else
   {
    dy += mmMoved;
    ((linearLayout *)parent)->dy = dy * factor;
   }

   scrollHandle.origin.y = scrollBody.origin.y - dy;
   ((linearLayout *)parent)->calcChildsOrigin();

   pointerY = currPointerY;
#ifdef FOR_RAJ_ANDROID
   LOGI("scroll maxDy: %f", maxDy)
   LOGI("scroll dy: %f", dy)
   LOGI("parent maxDy: %f", ((linearLayout *)parent)->maxDy)
   LOGI("parent dy: %f", ((linearLayout *)parent)->dy)
#endif
  }
 }
 else if (currPointerY < pointerY)
 {
  if (dy > 0)
  {
   float mmMoved = (pointerY - currPointerY) * rectNode::pxTommY;

   if (dy - mmMoved < 0)
   {
    dy = 0;
    ((linearLayout *)parent)->dy = 0;
   }
   else
   {
    dy -= mmMoved;
    ((linearLayout *)parent)->dy = dy * factor;
   }

   scrollHandle.origin.y = scrollBody.origin.y - dy;
   ((linearLayout *)parent)->calcChildsOrigin();

   pointerY = currPointerY;
  }
 }
}
