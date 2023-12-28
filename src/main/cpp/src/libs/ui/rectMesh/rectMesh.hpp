#ifndef RECTMESH
#define RECTMESH

#include <cstdint>
#include "vec2.hpp"
#include <iostream>


class rectMesh
{
private:
 uint32_t vbo, vao;
public:
 vec2 verts[4];//0:lb, 1:rb, 2:rt, 3:lt;
 rectMesh(){}
 ~rectMesh();
 void initVertexBuffers();
 void draw();
};

#endif