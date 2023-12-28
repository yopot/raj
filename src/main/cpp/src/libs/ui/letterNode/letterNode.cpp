#include "letterNode.hpp"

void letterNode::draw()
{
 glUniform2f(4, orignMm.x, orignMm.y);
 msh->draw();
}

void letterNode::initProgram()
{
 
#ifdef FOR_RAJ_ANDROID
 std::string vSrc = "#version 310 es\n";
 std::string fSrc = "#version 310 es\nprecision mediump float;\n";
#else
 std::string vSrc = "#version 460 core\n";
 std::string fSrc = "#version 460 core\n";
#endif

 vSrc += R"**(
 layout(location=0) in vec2 pos;
 layout(location=1) uniform vec2 mmToNdcScale;
 layout(location=2) uniform float z;
 layout(location=3) uniform float textHeight;
 layout(location=4) uniform vec2 uiMove;
 void main() {
	 gl_Position = vec4( ((pos * textHeight) + uiMove)
                     *mmToNdcScale + vec2(-1,1),
                     z,1);
 }
 )**";
 ;

 fSrc += R"**(
layout(location=5) uniform vec3 col;
out vec4 color;
void main(){
	color = vec4(col,1);
}
)**";

 prog.init(vSrc.data(), fSrc.data());
}