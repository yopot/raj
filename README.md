This repo contains c++ codes to build android app like this
![screenshot of android app](https://github.com/yopot/raj/blob/main/output.jpg)
 - It uses custom c++ android native glue code
 - It uses OpenGL for user interface
 - It uses 3d texts for font rendering

the 3d English letters are [here](https://github.com/yopot/raj/blob/main/src/main/assets/leftTopOrigin.glb)

This library tries to simulate android linear layout view in c++ using OpenGl.

The user code is in [here](https://github.com/yopot/raj/blob/main/src/main/cpp/src/app.cpp)

It uses tinygltf library to load glb file to display texts.
The libraries in src/main/cpp/src/libs can be used to create user interface using OpenGL in any screens(android, desktop, etc.)
