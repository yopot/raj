// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#ifdef FOR_RAJ_ANDROID
#define TINYGLTF_ANDROID_LOAD_FROM_ASSETS
#endif
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tinygltf/tiny_gltf.h>