#include "gltfLoader.hpp"
#include "rajErr.hpp"

void gltfLoader::load(const char* fileName)
{
 #ifdef FOR_RAJ_ANDROID
 tinygltf::asset_manager = am;
#endif
 tinygltf::TinyGLTF loader;
 std::string erro;
 std::string warn;
 std::string fName{fileName};
 bool res;
 if (fName.compare(fName.size() - 3, 3, std::string("glb")) == 0)
  // res = loader.LoadBinaryFromMemory(&dat,&erro, &warn,);
  res = loader.LoadBinaryFromFile(&dat, &erro, &warn, fileName);
 else
  res = loader.LoadASCIIFromFile(&dat, &erro, &warn, fileName);
 if (!warn.empty())
  throw rajErr(warn);
 if (!erro.empty())
  throw rajErr(erro);
 if (!res)
  throw rajErr("Failed to load glTF: ");
}

void gltfLoader::printVerts()
{
 /*
 const unsigned char *gltfBuff = dat.buffers[0].data.data();

 float pos[dat.accessors[0].count * 3];
 unsigned short idx[dat.accessors[1].count];
 memcpy(pos, gltfBuff, dat.bufferViews[0].byteLength);
 memcpy(idx, gltfBuff + dat.bufferViews[1].byteOffset, dat.bufferViews[1].byteLength);

 printf("pos: %ld\n", dat.accessors[0].count * 3);
 for (size_t i = 0, j = 1; i < dat.accessors[0].count * 3; j++, i++)
 {
  if (j % 3 == 0)
   printf("%f,\n", pos[i]);
  else
   printf("%f,  ", pos[i]);
 }

 printf("\nidx: %ld\n", dat.accessors[1].count);
 for (size_t i = 0; i < dat.accessors[1].count; i++)
 {
  printf("%d, ", idx[i]);
 }
 printf("\n");
 */
}