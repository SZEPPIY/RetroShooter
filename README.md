# Retro Shooter v1.0

A classic 2D arcade space shooter game built using C++ and the SFML 3 library. 

## Features

- **Object-Oriented Architecture:** Uses standard OOP principles, clean inheritance, and polymorphism to manage different enemy types ('Ufo (green color)' and 'Dragon (magenta color)') inside a single vector using smart pointers.
- **Ammunition System:** You start with 100 ammo. Shooting costs 1 ammo, but hitting an enemy rewards you with +2 ammo. If you miss, you waste resources.
- **Independent Drop System:** Red bonus items 'hearts' spawn randomly from the top of the screen based on a standalone timer, allowing the player to restore lost health points.
- **Health & Game Over:** The player has 3 lives. If 3 enemies pass the bottom of the screen, the game freezes and displays a "GAME OVER" screen.
- **Dynamic UI:** Real-time text display tracking current Ammo and HP.

## Controls

- 'A' / 'Left Arrow' - Move Left
- 'D' / 'Right Arrow' - Move Right
- 'Spacebar' - Shoot

