![MeIRL](https://github.com/HyphenxHyphen/CS3113IntroToGameDev/assets/71054673/3b849164-5fe1-4f78-ac2e-ed7225372714)

I'm early this time!!

And there're lots more features! (that no one asked for)

I used deltaTime, the Entity, and Map classes (But I renamed "Map" to "GameMap" so I don't confuse it for "map" = dictionaries).
I also made a bunch of other classes, and now Entity and GameMap are both derived from the same base class. (It's a surprise tool that will help us later)

I didn't use any ClosedBLn't functionalities that we haven't learned in class, I didn't watch tutorials on how to code this! 
My code's a mess, but I figured it out myself, so I'm happy with it.

I used new assets. These were mostly from https://pixelfrog-assets.itch.io/tiny-swords.
It's licensed under Public Domain and can be used without attribution, but I'll credit anyways. Thank you God Artist 🙏 🙇

There are 3 different AIs in the game with different behaviors: the Guard, Villager, and Goblin
I'll explain each below later.

There are 3 different levels/maps in the game. Map 1 is a bit of a tutorial, Map 2 and 3 fulfill the requirement that the AI are not all on one floor. At least one (more than one) is on a different platform.

Enemies can be defeated by hitting them with your sword. WASD to move, arrow keys to attack in that direction, space to jump. 
Space + Right/Left will cause you to jump in that direction. 
Attacking will cancel movement input.

The camera follows the player at its center, and any text is displayed centered on the camera. You can use the mouse scroll wheel to scroll in and out.

Enemies that are defeated are removed, they don't show up, and they aren't updated, because they're deleted.
There is a bit of garbage collection tech (marking entities to be deleted, and deleting them as the last step of a frame, and making sure all others dereference the entity)

If you die, you lose, a "You Lose" message is displayed and you get to retry the level.
If you kill all enemies, you win, a "You Win" message is displayed and you get to move on to the next level. 
Beating the final level will loop you back to the first level.

Extra Credit: 
Other than getting stabbed to death, one AI has a different way of killing you. It's funny, so I won't spoil it.

You can watch Run 1 to see some gameplay.
Do NOT watch Run 2 before trying the game for yourself.
