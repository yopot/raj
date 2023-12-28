#ifdef FOR_RAJ_ANDROID
#include <GLES3/gl3.h>
#else
#include <GL/glew.h>
#endif
#include "rectMesh.hpp"

void rectMesh::initVertexBuffers()
{
 glGenBuffers(1, &vbo);
 glGenVertexArrays(1, &vao);
 
 glBindVertexArray(vao);
 glBindBuffer(GL_ARRAY_BUFFER, vbo);
 glBufferData(GL_ARRAY_BUFFER,sizeof(verts), verts,GL_STATIC_DRAW);
 glEnableVertexAttribArray(0);
 glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
 
 glBindVertexArray(0);
 glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void rectMesh::draw()
{
 glBindVertexArray(vao);
 glDrawArrays(GL_TRIANGLE_FAN,0,4);
}

rectMesh::~rectMesh()
{
 glDeleteBuffers(1, &vbo);
 glDeleteVertexArrays(1, &vao);
}