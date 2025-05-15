The goal of this project is to build a simulation of a hexapod robot from scratch, using OpenGL and ImGUI.
For OpenGL, i use GLAD, and GLFW.

This project was inspired by the following video: https://www.youtube.com/watch?v=VwTd5cWJx2M which achieves something similar with the library libcinder, however, I wanted to try to do all the steps without libraries.

![A spider hexapod, infront of 3 stairs, with a edit GUI window, which contains variables of the spider leg, and a scene window, which shows the objects in it](docs/spider_red.png)

To run this, import the project into Visual Studio and build.
I ran this project using the installations: MSVC v143, C++ ATL for v143, and C++ MTC for v143. (Visual studio 2022).

If you are getting the error at `#include <afx.h>`, you didn't enable C++ MTC, which isn't enabled by default in the installation.
