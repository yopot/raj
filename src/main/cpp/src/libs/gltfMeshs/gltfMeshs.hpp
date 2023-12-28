#ifndef RAJ_GLTF_MESHS
#define RAJ_GLTF_MESHS

#include <tinygltf/tiny_gltf.h>
#ifdef FOR_RAJ_ANDROID
#include <GLES3/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <string>

/*
* it holds data for one mesh
* it contains draw func which bind & draws
*/
struct gltfMesh
{
 std::string name;
 GLuint vao;
 GLenum mode;
 GLsizei idxCount;
 GLenum idxCompType;
 void draw()
 {
  glBindVertexArray(vao);
  glDrawElements(mode, idxCount, idxCompType, 0);
 }
};

/*
 * call init method
 * It will prepare vbos,vaos & load mesh datas into GLbuffers
 * it stores mesh datas(name,vao,idxCount) in meshs var
 * you can get mesh data by using its index
 * its index is same as index in gltf node.mesh
 */
/*
gltfMeshs:
init():
1. call initVbos() and then call initVaos()

initVbos()
1. generate num of vbos = num of bufferViews in gltf file
2. fill vbos from read gltfBuffers using bufferViews
*/
class gltfMeshs
{
private:
 std::vector<GLuint> vbo, vao;
 // loop bufferView and create vbos
 void initVbos(const tinygltf::Model &);
 void initVaos(const tinygltf::Model &);

public:
 //its index is the value of mesh index gltf file
 std::vector<gltfMesh> meshs;
 void init(const tinygltf::Model &);
 ~gltfMeshs();
};

#endif