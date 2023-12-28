#include "gltfMeshs.hpp"

void gltfMeshs::init(const tinygltf::Model& gltf)
{
 initVbos(gltf);
 initVaos(gltf);
}

void gltfMeshs::initVbos(const tinygltf::Model& gltf)
{
 // vbo index = pos, index in accesssor gltf file
 vbo.resize(gltf.bufferViews.size());
 glGenBuffers(gltf.bufferViews.size(), vbo.data());

 // 3.assuming there is only one buffer
 const tinygltf::Buffer &buffer = gltf.buffers[0];

 for (size_t i = 0; i < gltf.bufferViews.size(); i++)
 {
  // 4. assuming every bufferView.target is not zero, if zero its err
  const tinygltf::BufferView &bufferView = gltf.bufferViews[i];
  glBindBuffer(bufferView.target, vbo[i]);
  glBufferData(bufferView.target, bufferView.byteLength,
               &buffer.data.at(0) + bufferView.byteOffset,
               GL_STATIC_DRAW);
 }

 glBindBuffer(GL_ARRAY_BUFFER, 0);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void gltfMeshs::initVaos(const tinygltf::Model &gltf)
{
 // vao indx = In gltf file it is mesh in node
 vao.resize(gltf.meshes.size());
 meshs.resize(gltf.meshes.size());
 glGenVertexArrays(gltf.meshes.size(), vao.data());

 for (size_t i = 0; i < gltf.meshes.size(); i++)
 {
  int posAccessorIdx = gltf.meshes[i].primitives[0].attributes.at("POSITION");
  int idxAccessorIdx = gltf.meshes[i].primitives[0].indices;
  const tinygltf::Accessor &posAccessor = gltf.accessors[posAccessorIdx];
  const tinygltf::Accessor &idxAccessor = gltf.accessors[idxAccessorIdx];
  const tinygltf::BufferView &posBuffView = gltf.bufferViews[posAccessor.bufferView];
  const tinygltf::BufferView &idxBuffView = gltf.bufferViews[idxAccessor.bufferView];

  meshs[i].vao = vao[i];
  meshs[i].name = gltf.meshes[i].name;
  meshs[i].mode = gltf.meshes[i].primitives[0].mode;
  meshs[i].idxCount = idxAccessor.count;
  meshs[i].idxCompType = idxAccessor.componentType;

  glBindVertexArray(vao[i]);
  glBindBuffer(posBuffView.target, vbo[posAccessor.bufferView]);
  glEnableVertexAttribArray(0); // assume posistion attribute location is 0
  glVertexAttribPointer(0, 3, posAccessor.componentType, GL_FALSE, 0, 0);
  glBindBuffer(idxBuffView.target, vbo[idxAccessor.bufferView]);
 }

 glBindVertexArray(0);
 glBindBuffer(GL_ARRAY_BUFFER, 0);
 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

gltfMeshs::~gltfMeshs()
{
 glDeleteVertexArrays(vao.size(), vao.data());
 glDeleteBuffers(vbo.size(), vbo.data());
}