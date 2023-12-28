#include "gltfLetters.hpp"

void gltfLetters::init(const char *gltfFileName)
{
 gltf.load(gltfFileName);

 mshs.init(gltf.dat);

 for (size_t i = 0; i < gltf.dat.nodes.size(); i++)
 {
  // setting letter data
  gltfLetter letter;
  const tinygltf::Value::Object &o = gltf.dat.nodes[i].extras.Get<tinygltf::Value::Object>();
  letter.width = o.at("width").Get<double>();
  letter.msh = &mshs.meshs[gltf.dat.nodes[i].mesh];
  // inserting in gltfLetters
  nodes[gltf.dat.nodes[i].name[0]] = letter;
 }
}