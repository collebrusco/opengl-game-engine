# opengl-game-engine
Game engine written with C++ and OpenGL. Uses EnTT for the data side of the entity component system only. Currently supports 2D &amp; simple 3D graphics, custom shaders, input, &amp; simple physics.
I wrote most of this during a 6 week period in the summer of 2022, during which I went from reading OpenGL window tutorials to having a functioning interactive environment with a player character, simple collisions, textures and controls. That said, it needs a lot more, and a lot of it is unkempt and due for rewrites. I learned so much writing this software that often by the time I'd finish adding a functionality or section, I would have learned enough that I could already see many flaws and ways in which it could be improved. Overall, this was a tremendous learning experience and I intend to return to this project as game engine design and game design in general is something I'm very passionate about. 

There isn't much of a formal API here. This project started as simply a game, but I'll be holding on to a lot of this code for re-use in other games. Hence, the foundation I've created here is a sort of engine. As well, the scene manager and entity component systems are designed to be easily expandable, so gameplay and features can be added without significantly spaghettifying the code. However, I haven't done much game design yet. When built, the program opens with a playable character next to a few vehicle entities arranged in a specific layout for testing the physics system. The player can move, aim, sprint, collide with the cars, and zoom in & out. Not many other types of entities have been enumerated yet, though the foundation is there. 

# Design Patterns & Features
- Command pattern:
Each "Actor" (Entity) inherits from a parent class and must implement a method "takeTurn()" that returns a list of action objects. Each action object inherits from a parent action class that must implement a "perform" function. These are simple objects that are mostly just functions, but being objects allows for some additional functionality, particularly polymorphism. In the main loop, the scene gathers these action objects from each entity, and runs through them all at once, calling perform() on each. This minimizes CPU cache misses in theory. There's an action object for walk, zoom, aim, attack, etc. The general idea of this pattern is taken from a talk by Bob Nystrom, but implemented here by me. The idea here is that each of these actions may have otherwise been methods contained in each different entity class, which is cumbersome, causes lots of repition, and most importantly doesn't lend itself to creative expandability of the game. Using this pattern, these actions and all the associated code live outside of the entity classes, and it is much easier to create a new type of entity that performs various actions. This focus on organization that is easily expandable is where I spent most of my time in the  planning phase.

# TODO
- world generation
- finish vehicle entity
- create NPC entity w/ simple AI
- better logging
- sound
- physics improvements
- GUI and menu support

# Credit
I use the ECS library of EnTT to handle the data side of the entity component system.
https://github.com/skypjack/entt

I adapted a few lines of code from jdh to improve mouse scroll input response.
https://github.com/jdah/minecraft-weekend

# This engine is NOT intended for commercial use in any way!
I created this as a passion project and learning tool independent of any school or professional organization.
