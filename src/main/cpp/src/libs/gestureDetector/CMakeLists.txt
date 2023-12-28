add_library(gestureDetector INTERFACE)
target_include_directories(gestureDetector INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}")
target_sources(gestureDetector INTERFACE 
tapDetector.cpp gestureDetector.cpp dragDetector.cpp
)

