add_subdirectory("libs/rajQ" "libs/rajQ")
add_subdirectory("libs/glCtx" "libs/glCtx")
add_subdirectory("libs/rajErr" "libs/rajErr")
add_subdirectory("libs/rajProg" "libs/rajProg")
add_subdirectory("libs/rajAndroid" "libs/rajAndroid")
add_subdirectory("libs/gltfLoader" "libs/gltfLoader")
add_subdirectory("libs/gltfLetters" "libs/gltfLetters")
add_subdirectory("libs/vec2" "libs/vec2")
add_subdirectory("libs/gltfMeshs" "libs/gltfMeshs")
add_subdirectory("libs/ui" "libs/ui")
add_subdirectory("libs/gestureDetector" "libs/gestureDetector")
 
add_library(myApp INTERFACE)
target_link_libraries(myApp INTERFACE 
rajAndroid rajQ glCtx rajErr rajProg rectNode vec2 gltfLoader gltfLetters gltfMeshs
linearLayout textNode gestureDetector
)
target_compile_definitions(myApp INTERFACE FOR_RAJ_ANDROID)
target_include_directories(myApp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/libs")
add_library(native-activity SHARED app.cpp)
target_link_libraries(native-activity myApp)
