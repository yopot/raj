add_library(gltfLoader INTERFACE)
target_include_directories(gltfLoader INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(gltfLoader INTERFACE 
gltfLoader.cpp tinygltf.cpp)

