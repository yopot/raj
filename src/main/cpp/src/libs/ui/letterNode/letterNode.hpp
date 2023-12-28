#ifndef RAJ_LETTER_NODE
#define RAJ_LETTER_NODE

#include "gltfMeshs.hpp"
#include "vec2.hpp"
#include "rajProg.hpp"

class letterNode
{
public:
 gltfMesh *msh;
 vec2 orignMm;
 void draw();
 // call this before creating any obj
 static void initProgram();
 inline static rajProg prog;
};

#endif