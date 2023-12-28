#ifndef RAJ_RECT_NODE
#define RAJ_RECT_NODE

#include "rectMesh.hpp"
#include "rajProg.hpp"
#include <vector>

#define ArrowKeyMmPerMove 5

class rectNode
{
public:
 enum rectNodeOrigin
 {
  LT=0,
  LB,
  RT,
  RB
 };
 enum rectNodeType
 {
  textNodeType, linearLayoutType, ScrollBarType
 };
 enum pageMove
 {
  pageUp,pageDown,pageRight,pageLeft,arrowDown,arrowUp
 };

 inline static rectMesh msh[4];
 inline static rajProg prog; //prog that draw rectNode
 
 inline static int vWidthPx = 0, vHeightPx = 0;
 inline static float mm2ndcX, mm2ndcY; // mmTondcScale factor
 inline static float pxTommX, pxTommY;
 inline static std::vector<rectNode *> allNodes;
 inline static rectNode *pointerDownNode = nullptr;

 rectNodeType type = rectNodeType::textNodeType;
 rectNode *parent = nullptr;
 std::vector<rectNode*> childs;
 bool needScroll = false;
 bool clickable = false;
 rectNodeOrigin o = LT;
 vec2 origin;
 std::string givenWidth, givenHeight;
 float width=0, height=0, zAxis=0;
 float padl=0, padr=0, padt=0, padb=0;
 float r =0, g =0, b =0;
 float allChildHeight = 0;

 void initRectNode(rectNode *parent, rectNodeOrigin o,
                   std::string w, std::string h,
                   float padl, float padr, float padb, float padt,
                   float r, float g, float b);
 void drawUi();
 // void pickClickables(rectNode* parent);
 virtual void findWidth(){}
 virtual void findHeight(){}
 virtual void calcChildsOrigin(){}
 virtual void findDimension(){}
 virtual void draw(){}
 virtual void drawText(){}
 virtual void onClick(){}
 static void initProgram(int screenWmm, int screenHmm, int screenW, int screenH);
 static void processClick(bool isPointerDown, int x, int adjY, int y);
 static void processMouseMove(int x, int y);

private:
 static void readNodeAtGivenLoc(int x, int y);
};

#endif