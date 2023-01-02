# Chicken Invaders
This is a simple game in two dimensions implemented using C++ and the OpenGl library as a part of the graphics course.

## Environmentand controls
The game consists of an enemy and a player, the enemy is bigger in size and is in the top of the screen and periodically will shoot the player which is smaller in size and is in the middle of the screen. The enemy moves periodically in a horizontal direction at the top of the window while the player can move freely using the controls:
- **W**: Moves upwards.
- **A**: Moves downwards.
- **S**: Moves right.
- **D**: Moves left.


The enemy will periodically shoot bullets on the player which will make the player lose one of thier 3 lives. While the player can shoot the enemy using the **C** Button.

The target of the game is to finish the enemy health (which is represented by the health bar at the bottom of the screen) while avoiding the enemy bullets.

Random power ups will show up on the screen. When the player colides with ot, they will be shielded from any bullets from the enemy for the next couple of seconds.

## Logic
Mathematical formulae to detect the collisions between the various objects in the game, model them and transform them. Some of these transformations were done using the OpenGl libraries.

## Preview

![Screenshot 2023-01-03 005142](https://user-images.githubusercontent.com/68449722/210283568-a1efe95c-9eb2-425e-840f-73d393f1c642.png)
