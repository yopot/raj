#ifndef RAJ_GLTF_LETTERS
#define RAJ_GLTF_LETTERS

#include "gltfMeshs.hpp"
#include "gltfLoader.hpp"
#include <map>

struct gltfLetter
{
 // get from rajMeshs obj using rajMeshIdx
 gltfMesh *msh;
 // width of the letter
 double width;
};

struct gltfLetters
{
public:
 std::map<char, gltfLetter> nodes;
 void init(const char* gltfFileName);

private:
 gltfMeshs mshs;
 gltfLoader gltf;
};

#endif