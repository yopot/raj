add_library(gltfLetters INTERFACE)
target_include_directories(gltfLetters INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(gltfLetters INTERFACE gltfLetters.cpp)