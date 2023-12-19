# 2D Top-View Game Implementation

This repository contains the implementation of a 2D top-view game created for the DMET 502 Computer Graphics course at the German University in Cairo, Winter 2023.

## Project Overview

### General Idea:

In this 2D top-view game, players navigate through a scene with the goal of collecting collectibles and power-ups while avoiding obstacles. The player has five lives displayed as a health bar at the top of the screen. The game score increases upon collecting collectibles, and the game ends if the player loses all lives or if the game times out.

### Game Elements:

- **Environment:**
  - Player starting position: Middle of the screen.
  - Game obstacles positioned around the scene.
  - Collectibles and power-ups placed randomly.
  - Player's health bar, game score, and remaining game time displayed.

- **Player:**
  - Starts at a fixed position upon game start.
  - Controlled by keyboard keys to move in four directions.
  - Player rotates towards the direction of motion.

- **Collectibles:**
  - Placed randomly in the scene.
  - Disappear upon collision.
  - Collecting them increases the game score.

## Power-Ups

### Time Freeze Power-Up:

- **Functionality:**
  - The Time Freeze Power-Up freezes the game timer for a specified duration.
  - When acquired by the player, the game timer stops counting down temporarily.

- **Appearance:**
  - The Time Freeze Power-Up appears randomly at various positions in the scene.
  - It remains fixed in position until acquired by the player.

- **Effect Duration:**
  - The effect of the Time Freeze Power-Up lasts for 10 seconds.
  - After the specified duration, the power-up deactivates, and the game timer resumes.

### Score Duplication Power-Up:

- **Functionality:**
  - The Score Duplication Power-Up multiplies the points earned upon collecting collectibles.
  - When acquired by the player, the game score increases at an accelerated rate for a limited time.

- **Appearance:**
  - The Score Duplication Power-Up appears randomly at various positions in the scene.
  - It remains fixed in position until acquired by the player.

- **Effect Duration:**
  - The effect of the Score Duplication Power-Up lasts for 10 seconds.
  - After the specified duration, the power-up deactivates, and the game score returns to its normal rate.

### Player Lives:

- Player loses a life on collision with obstacles or game boundaries.

### Game End:

- End game screen displayed upon game end.
- Indicates game win or loss based on the game state.

### Bonus Features:

- Sound effects and background music.

## How to Play

1. Clone the repository.
2. Open the project in your preferred development environment.
3. Run the game on Visual Studio.
4. Use keyboard keys to control the player and navigate through the scene.
5. Collect collectibles and power-ups while avoiding obstacles.
6. Monitor the health bar, game score, and remaining time.
7. Enjoy the game!
