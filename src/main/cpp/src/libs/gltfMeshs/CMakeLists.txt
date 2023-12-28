add_library(gltfMeshs INTERFACE)
target_include_directories(gltfMeshs INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(gltfMeshs INTERFACE gltfMeshs.cpp)