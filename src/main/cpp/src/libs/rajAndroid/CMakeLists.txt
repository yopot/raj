add_library(rajAndroid INTERFACE)
target_sources(rajAndroid INTERFACE rajAndroid.cpp)
target_include_directories(rajAndroid INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(rajAndroid INTERFACE
android EGL GLESv3 log)
