# opengl-game-engine
Game engine written with C++ and OpenGL. Uses EnTT for the data side of the entity component system only. Currently supports 2D &amp; simple 3D graphics, custom shaders, input, &amp; simple physics.
I wrote most of this during a 6 week period in the summer of 2022, during which I went from reading OpenGL window tutorials to having a functioning interactive environment with a player character, simple collisions, textures and controls. That said, it needs a lot more, and a lot of it is unkempt and due for rewrites. I learned so much writing this software that often by the time I'd finish adding a functionality or section, I would have learned enough that I could already see many flaws and ways in which it could be improved. Overall, this was a tremendous learning experience and I intend to return to this project as game engine design and game design in general is something I'm very passionate about. 

There isn't much of a formal API here. This project started as simply a game, but I'll be holding on to a lot of this code for re-use in other games. Hence, the foundation I've created here is a sort of engine. As well, the scene manager and entity component systems are designed to be easily expandable, so gameplay and features can be added without significantly spaghettifying the code. However, I haven't done much game design yet. When built, the program opens with a playable character next to a few vehicle entities arranged in a specific layout for testing the physics system. The player can move, aim, sprint, collide with the cars, and zoom in & out. Not many other types of entities have been enumerated yet, though the foundation is there. 


# TODO
- world generation
- finish vehicle entity
- create NPC entity w/ simple AI
- better logging
- sound
- physics improvements
- GUI and menu support
