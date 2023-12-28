#ifndef RAJ_PICKING_TEXTURE
#define RAJ_PICKING_TEXTURE

#include <GL/glew.h>

class PickingTexture
{
public:
 float pixel[3];

 PickingTexture();

 ~PickingTexture();

 void Init(unsigned int WindowWidth, unsigned int WindowHeight);

 void EnableWriting();

 void DisableWriting();
 

void ReadPixel(unsigned int x, unsigned int y);

private:
 GLuint m_fbo;
 GLuint rbo;
 GLuint m_pickingTexture;
 GLuint m_depthTexture;
};

#endif
