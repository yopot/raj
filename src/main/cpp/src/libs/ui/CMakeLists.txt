add_library(rectNode INTERFACE)
target_include_directories(rectNode INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}/rectMesh/"
"${CMAKE_CURRENT_SOURCE_DIR}/rectNode/"
# "${CMAKE_CURRENT_SOURCE_DIR}/pickingTex/"
)
target_sources(rectNode INTERFACE 
"${CMAKE_CURRENT_SOURCE_DIR}/rectNode/rectNode.cpp" 
"${CMAKE_CURRENT_SOURCE_DIR}/rectMesh/rectMesh.cpp"
# "${CMAKE_CURRENT_SOURCE_DIR}/pickingTex/pickingTex.cpp"
)

add_library(textNode INTERFACE)
target_include_directories(textNode INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}/textNode/"
"${CMAKE_CURRENT_SOURCE_DIR}/letterNode/")
target_sources(textNode INTERFACE 
"${CMAKE_CURRENT_SOURCE_DIR}/textNode/textNode.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/letterNode/letterNode.cpp")

add_library(linearLayout INTERFACE)
target_include_directories(linearLayout INTERFACE
"${CMAKE_CURRENT_SOURCE_DIR}/linearLayout/"
"${CMAKE_CURRENT_SOURCE_DIR}/rajScrollBar/")
target_sources(linearLayout INTERFACE 
"${CMAKE_CURRENT_SOURCE_DIR}/linearLayout/linearLayout.cpp" 
"${CMAKE_CURRENT_SOURCE_DIR}/rajScrollBar/rajScrollBar.cpp")

