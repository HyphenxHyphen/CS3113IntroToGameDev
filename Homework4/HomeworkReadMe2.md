![097](https://github.com/HyphenxHyphen/CS3113IntroToGameDev/assets/71054673/12c3def2-eb35-4252-aec4-68289c7bb1cf)

DO NOT READ THIS UNTIL YOU HAVE READ HomeworkReadMe.md AND TRIED THE GAME

SPOILERS BELOW

TECHNICAL DETAILS BELOW



--------
--------
--------
-------- 
--------
--------
--------
-------- 
--------
--------
--------
-------- 
--------
--------
--------
-------- 
--------
--------
--------


Okay so I never said it'd be easy. But then again, I never said they were enemies just because they were Red. That's right, there's a simple aggro system. 

When you first spawn, you are "not evil" (m_evil = false;)

All other NPCs are also "not evil," except for the Goblins, who are evil by default. They are the only enemies that you must defeat. Run2.mp4 demonstrates that.

If you attack any non-evil entities, you will become "evil," and all the Guards will start chasing you to attack you and all the Villagers will run away from you. Then, you must defeat EVERYONE to move on.
If you kill all the goblins and decide for fun to attack a Guard before the level ends, the level victory will be interrupted until you defeat everyone.

The Guards stand around and are a bit slower and deal less damage. They take 3 hits to kill you. They however follow you (or your shadow/"landingTarget" if you are jumping) and they can predict if you are going to enter their attack range. 
Since there is an attack windup (Damage is only dealt on the 3rd frame), the Guards also have Prefiring. THEY CAN PREFIRE! WHAT!

The Villagers wander around by default and since they are unarmored and unarmed, they have less health and cannot attack. 
When you become evil, they will run away from you, until you get too close. When you do get too close, the villagers decide they'd rather take you down with them, and they run at you instead.
If they get close enough, they'll pick you up and run right until they hit an edge. When they do, they jump off, carrying you with them. 
You can struggle and try to get out of their grab by mashing Left and Right, but they might decide to just throw you.

The Goblins wander around by default and are faster and stronger than you. They will kill you in 2 hits. When they spot you, they don't care if you are evil or not, they'll just rush you.
They have attack prediction, so if you try to attack them, they just stand still until you finish. 
They also run away if you are close enough to hit them. So they just stand outside of your reach, and rush you when you stop attacking.
They, however, are light, and their attacks are heavy. 
They cannot handle their own recoil, so they'll stun themselves and slide forwards after they attack. Hit them while they can't react!

Each AI only has simple movement (move towards/away from target). I wanted to implement a* but nahhhhh. You can see the simple movement run into issues on map 3.

I have elevation implemented, and I originally wanted to be able to go up in elevation, but right now you can only go down by jumping off a cliff. Would not reccommend.
Elevation helps me determine if I have landed on flat ground or not, and also helps me decided which "thing" (renderable) to render. I want to render all the renderables on a lower elevation and higher y value first. 
So I created the Renderable class and made Entity, Effect, and GameMap into derived classes, so that I could sort them all into a single vector<Renderable*>.

I improved the Map class into GameMap, which can handle animated tiles. I also created the loadLevelData() function to load level data from a black and white image. 
The original Map class took in a vector of unsigned ints, with each int referencing a tile sprite on a tile map (tile atlas with multiple types of tiles).
However, nowadays those kinds of atlases are difficult to find, and also it'd be difficult to set up animated tiles.
You would also need to manually change corners and edges of tiles.
Instead, I've opted to build each level as several layers of tiles, each layer has the same tile. Because of that, I can represent each layer as a vector of 0s and 1s called m_levelDataRaw.
Then, I can automatically convert each vector into a vector of unsigned ints called m_levelDataProcessed, each representing a tile sprite on a tile map (tile atlas with only one type of tile).
Then, I can copy that vector into a new vector of unsigned ints called m_levelDataAnimated. 
From here, I can use a map<unsigned int, vector<int>> which stores the first frame of an animated tile and a vector containing all the frames.
Using the first frame (which is recorded in m_levelDataProcessed) I can get the vector, and loop through it with a m_animLCM (I use the LCM instead of the max because multiple different animations with different animation frame lengths may be playing at the same time). 

Functionally, I have each layer with a bool of isSolid and isPit, as well as functions accepting a position to determine if a specific tile on that layer is a solid or a pit. 
isSolid helps me say "no, wallclip is not enabled, and neither is walk on air." isPit helps me decide if a player L + ratio + you fell off.
This also means I can just have an invisible layer at the bottom of the map act as collisions. WE HAVE INVISIBLE WALLS!!!

I wrote the Effect class, which is essentially an Entity class, but is basically a particle effect. It can either loop an animation or play an animation once and then disappear. 
It has no collision and no functional purpose and is purely aesthetic. But then again, with enough juice, you can make even block breaker "fun and interesting." (https://www.youtube.com/watch?v=Fy0aCDmgnxg)

I left some of my code as "Old" so you can see the difference between using new tech and not using new tech. 
