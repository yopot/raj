#include "rajProg.hpp"
#include "rajErr.hpp"

rajProg::~rajProg()
{
 glDeleteProgram(id);
}

bool rajProg::compileShader(GLenum shaderType, const char *src)
{
 GLuint shaderId = glCreateShader(shaderType);
 glShaderSource(shaderId, 1,&src, NULL);
 glCompileShader(shaderId);
 GLint status;
 glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
 if (!status)
 {
		GLchar err[512];
		glGetShaderInfoLog(shaderId, 512, NULL, err);
		errMsg += err;
		glDeleteShader(shaderId);
		return false;
 }
 glAttachShader(id, shaderId);
 glDeleteShader(shaderId);
 return true;
}

void rajProg::init(const char *vSrc, const char *fSrc)
{
 id = glCreateProgram();
 
 bool status = compileShader(GL_VERTEX_SHADER, vSrc);

 if (!status)
 {
		errMsg += "\nvertex shader err\n";
  throw rajErr(errMsg);
 }

 status = compileShader(GL_FRAGMENT_SHADER, fSrc);
 if (!status)
 {
		errMsg += "\nfragment shader err\n";
  throw rajErr(errMsg);
 }

 glLinkProgram(id);
 GLint isLink;
 glGetProgramiv(id, GL_LINK_STATUS, &isLink);
 if (!isLink)
 {
		errMsg += "program link err\n";
		GLchar err[512];
		glGetProgramInfoLog(id, 512, NULL, err);
		errMsg += err;
  throw rajErr(errMsg);
 }
 glValidateProgram(id);
 GLint isValid;
 glGetProgramiv(id, GL_VALIDATE_STATUS, &isValid);
 if (!isValid)
 {
		errMsg += "program validation err\n";
		GLchar err[512];
		glGetProgramInfoLog(id, 512, NULL, err);
		errMsg += err;
  throw rajErr(errMsg);
 }

 for (auto &&i : attribute)
 {
		GLint loc = glGetAttribLocation(id, i.first.data());
		if (loc == -1)
		{
			errMsg += "failed to get attrib location of ";
			errMsg += i.first;
			errMsg += '\n';
   throw rajErr(errMsg);
  }
		attribute[i.first] = loc;
 }

 for (auto &&i : uniform)
 {
		GLint loc = glGetUniformLocation(id, i.first.data());
		if (loc == -1)
		{
			errMsg += "failed to get uniform location of ";
			errMsg += i.first;
			errMsg += '\n';
   throw rajErr(errMsg);
  }
		uniform[i.first] = loc;
 }

}

