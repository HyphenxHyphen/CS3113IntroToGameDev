![a3e943aab372d95dc4e574eafb2d6157280248296c3cf3043cfbc7c027171e6b_1](https://github.com/HyphenxHyphen/CS3113IntroToGameDev/assets/71054673/99e8e4aa-aa07-4c9f-acd8-426bf2ff3468)

I'm late, but at least I'm not a Mojang dev.

Anyways, here are a bunch more features nobody asked for!

Let's get the requirements out of the way first.

I did not use `Entity`, because I broke up `Entity` into `GameObject`, `SpriteRenderer`, and `Rigidbody`. 
I had originally renamed `Map` to `GameMap` to avoid confusion, but after making it into a `Component`, it is now a `TileMap`.
I used the `Scene` class, but it's different from the `Scene` class provided. I added some stuff to it.
I included `CircleCollider2D` even though I didn't get to use it, because it's part of my engine. Check it out, it's pretty cool.

`Main.cpp` has that header comment block pledge thing.

Once again, thank you to [https://pixelfrog-assets.itch.io/tiny-swords](https://pixelfrog-assets.itch.io/tiny-swords) for the assets within `Assets/Tiny Swords`.
Files within `Assets/Audio` were collected from various sources. Except for `JumpSFX.wav` and `Dungeon1Looped.mp3`. I made those. 
Everything else in `Assets` were made by me.

There is a menu screen. It has a solid color background and is stored in `Scene0`.

There are 3 levels, stored within `Scene1`, `Scene2`, and `Scene3`. They scroll. Levels 2 and 3 have platforms.

The player gets 3 lives for the entire game. If you die three times, you get a `"You Lose"` and you get sent back to `Scene0`. If you beat all 3 levels, you get a `"You Win"` and you get sent back to `Scene0`. The illusion of choice...

Each level has at least 1 enemy with AI. The AI's been nerfed since the last homework.

There is audio, with 1 looping bgm and 4 sound effects included. That flaming torch is actually made of metal and very sharp.

Extra Credit: 
Press `p` while inside a level to pause.
There are special transitions between scenes. That sounds so fancy... It's just fade in and fade out...

Technical:
"We making it out of the Unity Engine with this one!!! :speaking_head: :fire: :fire: :fire: :exclamation: :exclamation: :exclamation:"

There are a few technical items I'd like to talk about.
First, there are the `GameObject` class and the `Component` class. A `GameObject` object keeps a vector of pointers to `Components` which can be attached to it. 
The best showcase of this tech is when I attached the `GoblinAI` class (script) to the enemies that were supposed to be Knights. The GoblinAI just kicks in without me having to do much else.

Second, there're the `Rigidbody` and the `Collider2D` classes. `Rigidbody` handles all the physics, while `Collider2D` child classes offer collision detection. 
There are 2 types of `Collider2D` children, the `PolygonCollider2D` and the `CircleCollider2D`.
Both `PolygonCollider2D` and `CircleCollider2D` uses SAT (Separating Axis Theorem) for collision detection.
Because `PolygonCollider2D` is stored as a vector of vertices instead of a length and width, `PolygonCollider2D` can then store more (or less) than 4 verticies, and in different non-regular convex shapes.
Thus I can have triangular colliders or hexagonal.
Because `CircleCollider2D` is stored as 2 seperate axis instead of a single radius, `CircleCollider2D` can have different radii on the x and y axis.
Tus I can have ellipitical colliders.
Because `Collider2D` has its own rotation, scale, and position values, I can have offset/non-centered, scaled, and rotated colliders.
Because `Rigidbody` considers all `Collider2D` objects attached to its parent, I can combine multiple colliders to form complex and concave shapes. For example, a star.

Lastly, and this is less important, because `Entity` was split up, I can now have `GameObject`s without `SpriteRenderer`s. Actually, `TileMap` is a component, so it is attached to a `GameObject`.
Since `TileMap` has its own rendering system, it does not use a `SpriteRenderer`. Attaching a `SpriteRenderer` would just render the sprite at the origin (where the parent `GameObject` object sits)
Entity logic is now in `EntityController`, with player input in `PlayerMovementController`. Any `GameObject` with a `PlayerMovementController` attached would be controllable.
