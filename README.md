# SpriteLight
## 2D/3D game-engine/framework using SDL2 and OpenGL <br>
## Can be compiled statically and dynamically on Linux and Windows <br>
## A game engine which will make developing games in opengl easier and more organized/structured while still allowing for low-level access <br>
### I will port all of my future games / and some existing games to this engine as a stress/compatibility test <br>
### Even though it might look 2D the main camera actually has a perspective projection while all screen-space objects will use an orthographic projection (a flat projection with no depth information) <br>
On compile the headers will merge into one main header <br>
enable SDF fonts by setting state->sdf_font to 1 <br>
It currently has a lot of useful text functions like DrawSubText(), MeasureText() and more to come! <br>
The game engine syntax itself involves functions to be called during the main loop like UpdateEngine() etc. to handle input and supply global struct pointer which contains all kinds of useful data like frame-time <br>
and an inbuilt camera system which can handle multiple camera types like panning etc.

![image](https://github.com/DissolveDZ/SpriteLight/assets/68782699/76a4f395-37e2-4f68-99ed-f044b5cbdf61)

![image](https://github.com/DissolveDZ/SpriteLight/assets/68782699/04fadc83-d5a5-4f71-9428-ef958357c15c)
