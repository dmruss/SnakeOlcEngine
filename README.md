# SnakeOlcEngine


This is an implementation of the Nokia phone classic, Snake, built using the Olc Pixel Game Engine.  The header file "olcPixelGameEngine.h" is not my own work, but is the only header file needed to run this engine.  The repo for the engine can be found at https://github.com/OneLoneCoder/olcPixelGameEngine.  


The game logic itself is contained within the main.cpp file.  All varaibles and objects are created within the Snake game class using object rendering functions inherited from the engine.  The snake itself is a series of rectangle objects whose coordinates are stored within vector objects and update with each move, creating the classic snake behavior.  All object detection is done by comparisons with the snake head's coordinates and each other object with each call the OnUserUpdate() function.  


## How to run

There is useful documentation for how to run projects using the Pixel Game Engine on the repo's wiki.  I find compiling the project using Visual Studio to be the most straigtforward way to get up and running quickly.  
