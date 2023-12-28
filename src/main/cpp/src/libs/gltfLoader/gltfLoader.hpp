#ifndef RAJ_GLTFLOADER
#define RAJ_GLTFLOADER

#ifdef FOR_RAJ_ANDROID
#include <android/asset_manager.h>
#define TINYGLTF_ANDROID_LOAD_FROM_ASSETS
#endif

#include <tinygltf/tiny_gltf.h>

struct gltfLoader
{
 tinygltf::Model dat;
 #ifdef FOR_RAJ_ANDROID
 inline static AAssetManager* am = nullptr;
 #endif
 void load(const char *fileName);
 void printVerts();
};

#endif