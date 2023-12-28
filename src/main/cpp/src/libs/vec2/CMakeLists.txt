add_library(vec2 INTERFACE)
target_include_directories(vec2 INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(vec2 INTERFACE vec2.cpp)

#for testing only
#add_library(vec2 vec2.cpp)