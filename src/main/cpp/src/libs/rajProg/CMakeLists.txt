add_library(rajProg INTERFACE)
target_include_directories(rajProg INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(rajProg INTERFACE rajProg.cpp)