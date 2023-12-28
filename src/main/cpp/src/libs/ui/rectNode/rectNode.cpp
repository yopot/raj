#include "rectNode.hpp"
#include "rajProg.hpp"
#include "linearLayout.hpp"
#include <algorithm>
#ifdef FOR_RAJ_ANDROID
#include "rajAndroid.hpp"
#endif

void rectNode::initProgram(
 int screenWmm, int screenHmm, 
 int screenW, int screenH)
{
 msh[LT].verts[3] = {0, 0};
 msh[LT].verts[0] = {0, -1};
 msh[LT].verts[2] = {1, 0};
 msh[LT].verts[1] = {1, -1};
 msh[LT].initVertexBuffers();

 msh[LB].verts[3] = {0, 1};
 msh[LB].verts[0] = {0, 0};
 msh[LB].verts[2] = {1, 1};
 msh[LB].verts[1] = {1, 0};
 msh[LB].initVertexBuffers();

 msh[RT].verts[3] = {-1, 0};
 msh[RT].verts[0] = {-1, -1};
 msh[RT].verts[2] = {0, 0};
 msh[RT].verts[1] = {0, -1};
 msh[RT].initVertexBuffers();

 msh[RB].verts[3] = {-1, 1};
 msh[RB].verts[0] = {-1, 0};
 msh[RB].verts[2] = {0, 1};
 msh[RB].verts[1] = {0, 0};
 msh[RB].initVertexBuffers();

 mm2ndcX = 2.0 / screenWmm;
 mm2ndcY = 2.0 / screenHmm;

#ifdef FOR_RAJ_ANDROID
 std::string vSrc = "#version 310 es\n";
 std::string fSrc = "#version 310 es\nprecision mediump float;\n";
#else
 std::string vSrc = "#version 460 core\n";
 std::string fSrc = "#version 460 core\n";
#endif

 vSrc += R"**(
 layout(location=0) in vec2 pos;
 layout(location=1) uniform vec2 scale;
 layout(location=2) uniform vec2 origin;
 layout(location=3) uniform vec2 ndcScale;
 layout(location=4) uniform vec2 ndcMove;
 layout(location=5) uniform float z;
 void main() {
	 gl_Position = vec4(
   (pos*scale+origin)*ndcScale+ndcMove, z, 1);
 }
 )**";

 fSrc += R"**(
layout(location=6) uniform vec3 col;
out vec4 color;
void main(){
	color = vec4(col,1);
}
)**";

 prog.init(vSrc.data(), fSrc.data());
}

void rectNode::
initRectNode(rectNode *parent, rectNodeOrigin o,
                 std::string w, std::string h,
                 float padl, float padr, float padb, float padt,
                 float r, float g, float b)
{
 this->parent = parent;
 if(parent != nullptr)
 {
 parent->childs.push_back(this);
 zAxis = parent->zAxis - 0.0001;
 }
 this->o = o;
 this->givenWidth = w;
 this->givenHeight = h;
 this->padl = padl;
 this->padr = padr;
 this->padb = padb;
 this->padt = padt;
 this->r = r;
 this->g = g;
 this->b = b;
 allNodes.push_back(this);
}

void rectNode::
drawUi()
{
 for(auto node: allNodes)
 {
  if(node->type != ScrollBarType)
  node->draw();
 }
}

void rectNode::
readNodeAtGivenLoc(int x, int y)
{
 float xnd = x * 2.0 / vWidthPx - 1;
 float ynd = y * 2.0 / vHeightPx - 1;
 float a = (xnd + 1) / mm2ndcX;
 float b = (ynd - 1) / mm2ndcY;
 rectNode *clickedNode = nullptr;
 bool oneTime = true;
 for (auto i : allNodes)
 {
  if (i->clickable)
  {
   if (i->origin.x <= a && a <= i->origin.x + i->width && i->origin.y - i->height <= b && b <= i->origin.y)
   {
    for (rectNode *j = i->parent; true; j = j->parent)
    {
     if(i->type == ScrollBarType && oneTime)
     {
      oneTime = false;
      if (j->parent == nullptr)
      {
       clickedNode = i;
       break;
      }
      else
       continue;
     }
     if (j->origin.x + j->padl <= a && a <= j->origin.x + j->width - j->padr && j->origin.y - j->height + j->padb <= b && b <= j->origin.y - j->padt)
     {
      if (j->parent == nullptr)
      {
       clickedNode = i;
       break;
      }
      else
       continue;
     }
     else
      break;
    }
   }
  }
 }
 pointerDownNode = clickedNode;

}

void rectNode::
processClick(bool isPointerDown, int x, int adjY, int y)
{
 if (isPointerDown)
 {
  readNodeAtGivenLoc(x, adjY);

  if (pointerDownNode != nullptr)
  {
   if (pointerDownNode->type == ScrollBarType)
   {
    ((linearLayout *)pointerDownNode->parent)->scrollBar.pointerX = x;
    ((linearLayout *)pointerDownNode->parent)->scrollBar.pointerY = y;
   }
  }
 }
 else
 {
  if (pointerDownNode != nullptr)
  {
   if (pointerDownNode->type != ScrollBarType)
   {
    rectNode *prevDownNode = pointerDownNode;
    readNodeAtGivenLoc(x, adjY);

    if (pointerDownNode == prevDownNode)
    {
     pointerDownNode->onClick();
    }
   }
   pointerDownNode = nullptr;
  }
 }
}

void rectNode::
processMouseMove(int x, int y)
{
 if(pointerDownNode != nullptr && pointerDownNode->type == ScrollBarType)
 {
  linearLayout *j = (linearLayout *)pointerDownNode->parent;
  j->scrollBar.doScroll(x, y);
 }
}

