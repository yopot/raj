add_library(rajQ INTERFACE)
target_sources(rajQ INTERFACE rajQ.cpp)
target_include_directories(rajQ INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}")
