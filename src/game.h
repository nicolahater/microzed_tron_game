#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "player.h"
#include "display.h"

#define CLASSIC 1
#define INFINITY 2

#define BORDER_COLOR DISPLAY_COLOR(0, 0, 31)
#define BORDER_SIZE 5

typedef struct {
	uint8_t numOfPlayers;
	player_t** players;
	uint8_t mode;
} game_t;

/**
 * Updates the state of the game and players based on user input or AI behavior.
 * It utilizes knob input to control the corresponding player:
 *
 * @param players An array of player_t pointers representing the players in the game.
 * @param numOfPlayers The number of players in the game.
 * @param old_knobs_value A pointer to the previous knob values.
 * @param LEDline A pointer to the LED line values.
 * @param leds An array representing the LED colors for each player.
 * @param mode The game mode.
 * @param fb A pointer to the framebuffer.
 * @return An integer representing the game state:
 *         - GAME: The game is still in progress.
 *         - OVER: The game is over.
 */
int playGame(player_t* players[], uint8_t numOfPlayers, uint32_t* old_knobs_value,
						 uint32_t* LEDline, uint32_t leds[], int mode, uint16_t* fb);

/**
 * Handles the game over screen and user input for restarting or going back to the main menu.
 *
 * @param winner The index of the winning player (0 or 1)
 * @param green_knob_value Pointer to the current value of the green knob
 * @param mode The game mode
 * @param fb Pointer to the frame buffer for the display
 * @return The value indicating the next action:
 *         - OVER: No action (game over)
 *         - GAME: Restart the game
 *         - MENU: Go back to the main menu
 */
int gameOver(int winner, uint32_t* green_knob_value, int mode, uint16_t* fb);

#endif
