#ifndef RAJ_TEXT_NODE
#define RAJ_TEXT_NODE

#include "rectNode.hpp"
#include "gltfLetters.hpp"
#include "letterNode.hpp"
#include "textNode.hpp"
#include <string>
#include <vector>

class textNode : rectNode
{
public:
vec2 cursorNxt, cursorPrev, cursorCurr;
float txtBoxRight, txtBoxLeft, txtBoxTop, txtBoxBottom,
    txtBoxWidth, txtBoxHeight;
vec2 txtBoxOrigin;
static inline gltfLetters *enLetters = nullptr;
std::vector<letterNode> letters;
#ifdef FOR_RAJ_ANDROID
float txtHeight = 5;
float letterSpacing = 0.05;
float wordSpacing = 0.282;
float lineSpacing = 0.05;
#else
float txtHeight = 20;
float letterSpacing = 0.05;
float wordSpacing = 0.282;
float lineSpacing = 0.05;
#endif
std::string text;
void initTextNode(rectNode *parent, rectNodeOrigin o,
                  std::string w, std::string h,
                  float padl, float padr, float padb, float padt,
                  float r, float g, float b,
                  std::string text);
void findWidth();
void findHeight();
void calcChildsOrigin();
void printDimension(uint32_t c);
void printOrigin(uint32_t c);
void drawText();
void draw();
void onClick();

private:
void processText();
};

#endif