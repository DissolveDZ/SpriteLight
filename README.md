# SpriteLight
## 2D/3D game-engine/framework using SDL2 and OpenGL <br>
## Can be compiled statically and dynamically on Linux and Windows <br>
## A game engine which will make developing games in opengl easier and more organized/structured while still allowing for low-level access <br>
### I will port all of my future games / and some existing games to this engine as a stress/compatibility test <br>
### Even though it might look 2D the main camera actually has a perspective projection while all screen-space objects will use an orthographic projection (a flat projection with no depth information) <br>
On compile the headers will merge into one main header <br>
Now supports font rendering using freetype, I'm currently working on SDF fonts <br> 
It currently has a lot of useful text functions like DrawSubText(), MeasureText() and more to come! <br>
The game engine syntax itself involves functions to be called during the main loop like UpdateEngine() etc. to handle input and supply global struct pointer which contains all kinds of useful data like frame-time <br>
and an inbuilt camera system which can handle multiple camera types like panning etc.

![image](https://github.com/DissolveDZ/SpriteLight/assets/68782699/8f7a6df8-ccb1-49d1-8dd9-f9ea7bb0b4e3)
