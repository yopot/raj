add_library(glCtx INTERFACE)
target_sources(glCtx INTERFACE glCtx.cpp)
target_include_directories(glCtx INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}")
