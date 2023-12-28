#include "linearLayout.hpp"
#include <cmath>

void linearLayout::
initLinearLayout(rectNode *parent, rectNodeOrigin o,
                     std::string w, std::string h,
                     float padl, float padr, float padb, float padt,
                     float r, float g, float b)
{
 initRectNode(parent,o,w,h,padl,padr,padb,padt,r,g,b);
 this->type = rectNodeType::linearLayoutType;
 stencilId = stencilVal;
 stencilVal++;
}

void linearLayout::
findDimension()
{
 findWidth();
 findHeight();
}

void linearLayout::
findWidth()
{
 width = 0;
 // depends on children width
 if (givenWidth.size() == 1 && givenWidth[0] == '0')
 {
 } 
 // depends on parent width
 else if (givenWidth[givenWidth.size() - 1] == '%')
 {
  width = std::atof(givenWidth.substr(0, givenWidth.size() - 1).data());
  width = width * 0.01 * (parent->width - parent->padl - parent->padr);
  for (auto i : childs) i->findWidth();
 }
 else // fixed width
 {
  width = std::atof(givenWidth.data());
  for (auto i : childs) i->findWidth();
 }
}

void linearLayout::
findHeight()
{
 height = 0;
 allChildHeight = 0;
 maxDy = 0;

 // depends on children height
 if (givenHeight.size() == 1 && givenHeight[0] == '0')
 {
  for (auto i : childs)
  {
   i->findHeight();
   height += i->height;
  }
  allChildHeight = height;
  height = height + padt + padb;
 } 
 //depends on parent height
 else if (givenHeight[givenHeight.size() - 1] == '%')
 {
  height = std::atof(givenHeight.substr(0, givenHeight.size() - 1).data());
  height = height * 0.01 * (parent->height - parent->padt - parent->padb);
  for(auto i : childs){
   i->findHeight();
   allChildHeight += i->height;
  } 
 }
 else // fixed height
 {
  height = std::atof(givenHeight.data());
  for (auto i : childs)
  {
   i->findHeight();
   allChildHeight += i->height;
  }
 }
 
 maxDy = allChildHeight - height + padt + padb;
 #ifdef FOR_RAJ_ANDROID
 // increasing font size makes below not working
 //maxDy = maxDy * 0.45 + maxDy; // 0.45 is temporary hack
 #endif
}

void linearLayout::
findScrollables()
{
 // find scrollables
 for (auto i : allNodes)
 {
  if (i->type == rectNodeType::linearLayoutType)
  {
   if (i->allChildHeight > i->height - i->padt - i->padb)
   {
    linearLayout *j = (linearLayout *)i;
    j->needScroll = true;
    j->scrollBar.initRajScrollBar(i);
    j->scrollBar.findDimension();
    
    j->padr += j->scrollBar.scrollHandle.width;
    for (auto child : j->childs)
    {
     child->findWidth();
     child->findHeight();
    }
    /*
        if (j->padr < j->scrollBar.scrollHandle.width)
        {
         j->padr = j->scrollBar.scrollHandle.width;

         for (auto child : j->childs)
         {
          child->findWidth();
          child->findHeight();
         }
        }
      */
   }
  }
 }

 for (auto i : allNodes)
 {
  if (i->needScroll)
  {
   i->allChildHeight = 0;

   for (auto child : i->childs)
    i->allChildHeight += child->height;

  }
 }
}

void linearLayout::
calcChildsOrigin()
{
 if(needScroll)
  scrollBar.findOrigin();

 for (size_t i = 0; i < childs.size(); i++)
 {
  if(i == 0)
  {
   childs[i]->origin = origin + vec2(padl, -padt);
   childs[i]->origin.y += dy;
  }
  else
   childs[i]->origin = childs[i - 1]->origin + vec2(0, -childs[i - 1]->height);

  if (childs[i]->type == rectNode::rectNodeType::linearLayoutType && childs[i]->needScroll)
  {
   ((linearLayout*)childs[i])->scrollBar.findOrigin();
  }
  
  childs[i]->calcChildsOrigin();
 }
}

void linearLayout::
draw()
{
 glUseProgram(rectNode::prog.id);
 glUniform2f(3, rectNode::mm2ndcX, rectNode::mm2ndcY);
 glUniform1f(5, zAxis);
 glUniform3f(6, r, g, b);

 float w = 0, h = 0, ox = 0, oy = 0;
 if (parent != nullptr)
 {
  glStencilFunc(GL_EQUAL, ((linearLayout *)parent)->stencilId, 0XFF);
  glStencilMask(0xFF);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
 }

 switch (o)
 {
 case LT:
  {
   glUniform2f(4, -1, 1);
   if (padl)
  {
   ox = origin.x;
   oy = origin.y;
   w = padl;
   h = height;
   glUniform2f(1, w, h);
   glUniform2f(2, ox, oy);
   msh[LT].draw();
   }
   if(padt)
   {
    ox = origin.x + padl;
    oy = origin.y;
    w = width - padl - padr;
    h = padt;
    glUniform2f(1, w, h);
    glUniform2f(2, ox, oy);
    msh[LT].draw();
   }
   if(padr)
   {
   ox = origin.x + width - padr;
   oy = origin.y;
   w = padr;
   h = height;
   glUniform2f(1, w, h);
   glUniform2f(2, ox, oy);
   msh[LT].draw();
   }
   if(padb)
   {
    ox = origin.x + padl;
    oy = origin.y - height + padb;
    w = width - padl - padr;
    h = padb;
    glUniform2f(1, w, h);
    glUniform2f(2, ox, oy);
    msh[LT].draw();
   }
  
   ox = origin.x + padl;
   oy = origin.y - padt;
   w = width - padl - padr;
   h = height - padt - padb;
   glUniform2f(1, w, h);
   glUniform2f(2, ox, oy);

   if(parent == nullptr)
   {
    glStencilFunc(GL_ALWAYS, stencilId, 0XFF);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    msh[LT].draw();
   }
   else
   {
    // std::cout << "stencilId: " << stencilId << "\n";
    // std::cout << "parent: " << ((linearLayout *)parent)->stencilId << "\n";
    for (size_t i = 0; ((linearLayout*)parent)->stencilId + i != stencilId; i++)
    {
     glStencilFunc(GL_EQUAL, ((linearLayout *)parent)->stencilId + i, 0XFF);
     glStencilMask(0xFF);
     glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
     msh[LT].draw();
     if(i==0)
     {
      glColorMask(false,false,false,false);
      glDepthMask(false);
     }
    }
    glColorMask(true, true, true, true);
    glDepthMask(true);
   }

   glStencilMask(0x00);
   glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
  }
  break;
 
 default:
  break;
 }

 if (needScroll)
  scrollBar.draw();
}

#ifdef RAJ_DEBUG
void linearLayout::
printDimension(uint32_t c)
{
 printf("linearLayout dim: %f,%f\n", width, height);
 for(auto i : childs)
 {
 for(uint32_t i =0; i<c;i++) printf(" ");
 i->printDimension(c+2);
 }
}

void linearLayout::
printOrigin(uint32_t c)
{
 printf("linearLayoutOrigin: %f,%f\n", origin.x, origin.y);
 for(auto i : childs)
 {
 for(uint32_t i =0; i<c;i++) printf(" ");
 i->printOrigin(c+2);
 }
}
#endif