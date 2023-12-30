This repo contains c++ codes to build android app like below
![screenshot of android app](%5Bhttps://github.com/yopot/raj/blob/main/output.jpg?raw=true%5D%28https://github.com/yopot/raj/blob/main/src/main/output.jpg?raw=true%29)
 - It uses custom c++ android native glue code
 - It uses OpenGL for user interface
 - It uses 3d texts for font rendering

the 3d English letters are [here](https://github.com/yopot/raj/blob/main/src/main/assets/leftTopOrigin.glb)

This library tries to simulate android linear layout view in c++ using openGl.

The user code is in [here](https://github.com/yopot/raj/blob/main/src/main/cpp/src/app.cpp)

It uses tinygltf library to load glb file to display texts.
this library can be used to create user interface using opengl in all screens (android, desktop, etc)

feel free to check and make your own code out of this
