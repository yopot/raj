#include "textNode.hpp"
#include "linearLayout.hpp"
#ifdef FOR_RAJ_ANDROID
#include "rajAndroid.hpp"
#else
#include <iostream>
#endif

void textNode::
initTextNode(rectNode *parent, rectNodeOrigin o,
                 std::string w, std::string h,
                 float padl, float padr, float padb, float padt,
                 float r, float g, float b,
                 std::string text)
{
 initRectNode(parent, o, w, h, padl, padr, padb, padt, r, g, b);
 this->text = text;
 this->type = rectNodeType::textNodeType;
 clickable = true;
}

void textNode::
findWidth()
{
 width = 0;
 // depends on children width, but text needs fixed
 // width, so find its ancestors fixed width
 // assuming its parent has fixed width now
 if (givenWidth.size() == 1 && givenWidth[0] == '0')
 {
  width = parent->width - parent->padl - parent->padr;
 } // depends on parent width
 else if (givenWidth[givenWidth.size() - 1] == '%')
 {
  width = std::atof(givenWidth.substr(0, givenWidth.size() - 1).data());
  width = width * 0.01 * (parent->width - parent->padl - parent->padr);
 }
 else // fixed width
 {
  width = std::atof(givenWidth.data());
 }
}

void textNode::findHeight()
{
 height = 0;
 processText();

 //depends on children height
 if(givenHeight.size() == 1 && givenHeight[0] == '0')
 {
  height = txtBoxHeight + padt + padb;
 }//depends on parent height
 else if (givenHeight[givenHeight.size()-1] == '%')
 {
  height = std::atof(givenHeight.substr(0, givenHeight.size() - 1).data());
  height = height * 0.01 * (parent->height - parent->padt - parent->padb);
 }
 else // fixed height
 {
  height = std::atof(givenHeight.data());
 }
}

void textNode::
processText()
{
 txtBoxLeft = 0;
 txtBoxTop = 0;
 txtBoxRight = width - padl - padr;
 txtBoxWidth = txtBoxRight - txtBoxLeft;

 cursorCurr.x = txtBoxLeft;
 cursorCurr.y = txtBoxTop;
 float letterSpace = letterSpacing * txtHeight;
 float wordSpace = wordSpacing * txtHeight;
 float lineSpace = lineSpacing * txtHeight;
 letters.clear();

 if (text[text.size() - 1] != '\n')
  text.push_back('\n');

 float wordWidth = 0;
 size_t wordStart = 0;
 for (size_t letterIdx = 0; letterIdx < text.size(); letterIdx++)
 {
  switch (text[letterIdx])
  {
  case ' ':
  case '\n':
  {
   // check word fits in available space (cursorCurr to txtBoxWidth)
   // if it not fits, move cursor to newline
   wordWidth *= txtHeight;
   if (text[letterIdx] == ' ')
    wordWidth += letterSpace;
   float availWidth = txtBoxRight - cursorCurr.x;

   if (wordWidth > availWidth)
   {
    if (wordWidth <= txtBoxWidth) // not greater than txtBoxWidth
    {
     cursorCurr.y = cursorCurr.y - txtHeight - lineSpace;
     cursorCurr.x = txtBoxLeft;
    }
   }
   // reset to 0 for next word width calculation
   wordWidth = 0;

   // calc origin of each letter in word and
   // move the cursor respectively
   std::string word = text.substr(wordStart, letterIdx - wordStart);
   // calc nxt word start idx
   wordStart = letterIdx + 1;
   for (auto &&i : word)
   {
    letterNode tmp;
    tmp.msh = enLetters->nodes[i].msh;
    tmp.orignMm = cursorCurr;
    letters.push_back(tmp);
    // for next letter
    cursorCurr.x = cursorCurr.x + (enLetters->nodes[i].width * txtHeight) + letterSpace;
   }

   // move the cursor by space or newline
   if (text[letterIdx] == ' ')
   {
    cursorCurr.x += wordSpace;
   }
   else
   {
    cursorCurr.y = cursorCurr.y - txtHeight - lineSpace;
    cursorCurr.x = txtBoxLeft;
   }
  }
  break;
  default:
   // add each letter width to curr word width
   wordWidth += enLetters->nodes[text[letterIdx]].width;
   break;
  }
 }
 
 txtBoxBottom = -cursorCurr.y;
 txtBoxHeight = txtBoxBottom - txtBoxTop;
}

void textNode::
calcChildsOrigin()
{
 for (auto &i : letters)
  i.orignMm -= txtBoxOrigin;
 txtBoxOrigin = origin + vec2(padl, -padt);
 for (auto &i : letters)
  i.orignMm += txtBoxOrigin;
}

void textNode::
printOrigin(uint32_t c)
{
 printf("textNodeOrigin: %f,%f\n", origin.x, origin.y);
}

void textNode::
printDimension(uint32_t c)
{
 printf("textNode dim: %f,%f\n",width,height);
}

void textNode::
draw()
{
 glUseProgram(rectNode::prog.id);
 glUniform2f(1, width, height);
 glUniform2f(2, origin.x, origin.y);
 glUniform2f(3, rectNode::mm2ndcX, rectNode::mm2ndcY);
 glUniform1f(5, zAxis);
 glUniform3f(6, r, g, b);

 switch (o)
 {
 case LT:
 {
  glUniform2f(4, -1, 1);
 }
 break;
 case LB:
 {
  glUniform2f(4, -1, -1);
 }
 break;
 case RT:
 {
  glUniform2f(4, 1, 1);
 }
 break;
 case RB:
 {
  glUniform2f(4, 1, -1);
 }
 break;
 }

 glStencilFunc(GL_EQUAL, ((linearLayout *)parent)->stencilId, 0XFF);
 glStencilMask(0x00);
 glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
 
 msh[o].draw();
 
 drawText();
}

void textNode::
drawText()
{
 glUseProgram(letterNode::prog.id);
 glUniform2f(1, mm2ndcX, mm2ndcY);
 glUniform1f(2, zAxis-0.0001);
 glUniform1f(3, txtHeight);
 glUniform3f(5, 1,1,1);
 for (auto &&letter : letters)
  letter.draw();

 glStencilFunc(GL_ALWAYS, 1, 0XFF);
}

void textNode::
onClick()
{
#ifdef FOR_RAJ_ANDROID
 LOGI("%s",text.data())
#else
std::cout << text << "\n";
#endif
}
