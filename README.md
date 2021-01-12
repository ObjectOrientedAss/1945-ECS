# 1945-ECS

Welcome! This is a small project made by me in a couple of Christmas-Full Immersive-weeks.
Basically, it is a 2D game engine made in pure old C, accompanied by a little game (1945) to test its functionalities.
The engine uses SDL2 as graphic library, and of course its little children, SDL Image, SDL Mixer, and SDL TTF.
TTF is not optimized tho, so i have decided to use another library which is built over it, called SDL Font Cache.

In order to make things in a different way, i have decided to accomplish all of this with a little challenge: make the game systems in ECS.
I have never developed an ECS before this project, and i only knew some general things about it, so it is far from a real efficent ECS, but it should give an idea about how to start an ECS project and the main principles it relies on.

## What CAN the engine do right now?

* Handle inputs
* Handle sprites and spritesheets
* Handle fonts
* Handle sounds and music
* Handle time
* Handle randoms
* Handle object pooling
* Handle custom components and behaviours for you entities
* Handle entity and components destruction / toggling

## What CAN'T the engine do right now?

* Handle texture atlas.
* Handle rotations.
* Handle physics automatically: *you have to define your physics behaviours in a "physics" component in order to define the physics and the collisions on your entities, but i am planning to move all the existing physics component stuff in a separate physics engine very soon along with AABB (Axis Aligned Bounding Box) and CC (Circle Circle) methods.*
* Load the resources paths and types outside their relative engine .c files: *these files should not be available for user access and modification, so i'm planning to expose methods to let the user pass the paths and the types from their own external files.*

## Known bugs (Engine)

- At application quit, there is a free error on SDL Font Cache library, which is not written by me, i'm still trying to figure out how to solve it.

## Known issues (Engine)

- As stated before, this engine is missing a Physics Engine to automatically handle different types of collisions.
- Also, as said above, more decoupling between the user resources and the parts of the engine involved in their loading.
- Components are currently accessing to required components with too many GetComponent / GetComponentData calls, very bad for optimization.
- Removal loops for entities and components are currently not optimized.

## User Guide

Work in progress...
> Rome Engine wasn't built in a day.