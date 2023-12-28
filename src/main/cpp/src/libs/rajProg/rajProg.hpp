#pragma once

#ifdef FOR_RAJ_ANDROID
#include <GLES3/gl3.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif
#include <map>
#include <string>
using namespace std;

class rajProg
{
 bool compileShader(GLenum shaderType,const char* src);
public:
 string errMsg;
 GLuint id;
 map<string,GLint> attribute;
 map<string,GLint> uniform;
 void init(const char* vSrc, const char* fSrc);
 ~rajProg();
};
