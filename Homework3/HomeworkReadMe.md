![MarioMaker](https://github.com/HyphenxHyphen/CS3113IntroToGameDev/assets/71054673/c5b7a456-629d-4420-bc35-8734d3209185)

I know, I know, it's a day late, but I'll take the -10% because I've learned a LOT from this

I used deltaTime and Fixed Time for physics.
I used the Entity class.

The player (grabber) falls with gravity.
Movement is applied only via Acceleration*.
Movement and position are not directly updated!
(*Okay, but there is a limit on the downward Y velocity, as Air Friction.
  And a limit to the upward Y velocity, as Weight.
  And there is a limit to positive Y acceleration, as Gravity.
  And there is a limit to X acceleration as Inertia.)

There is a Mission Failed screen (MISSED! YOU LOSE!).
There is a Mission Accomplished screen (GRABBED THE X COIN! YOU WIN!).

Extra Credit:
There is a Fuel mechanic!

And now, for the extra features that nobody asked for, and costed me 10% for... (average game dev industry moment)

There are 3 different targets (coins)!
There are game modes! That means MAIN_MENU, GAME_LEVEL, and GAME_OVER (no there aren't any mini-games).
Press any key to start on MAIN_MENU, press any key to reset on GAME_OVER!
This means that there is a reset feature in the game! And each reset generates a new map!
There is also tiling for tiles!
I could've just handwritten or pre-created the messages, but I didn't, so...
There is a font-writing system!
