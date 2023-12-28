add_library(rajErr INTERFACE)
target_sources(rajErr INTERFACE rajErr.cpp)
target_include_directories(rajErr INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}")

