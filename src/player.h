#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "utils.h"
#include "display.h"

#define PLAYER_SIZE 5

typedef struct {
    int speed;                          // Current speed of the player
    int acceleration;                   // Acceleration value of the player
    int* amountOfAccelerations;         // Pointer to an array of the amount of accelerations for each player
    int maxAmountOfAccelerations;       // Maximum amount of accelerations allowed for a player
    int accelerationDuration;           // Duration of acceleration for a player
    int* durationCounter;               // Pointer to an array of counters to track acceleration duration for each player
    int accelerationRegeneration;       // Rate of acceleration regeneration for a player
    int* regenerationCounter;           // Pointer to an array of counters to track acceleration regeneration for each player
} player_speed_t;

typedef struct {
	int coordinates[2];				// x, y player coordinates on display
	int direction[2];    			// x, y vector direction
	uint16_t color;						// player color
	uint32_t knobsValue;			// knob value of certain player
	uint32_t controlKnob;			// knob mask that controls player
	player_speed_t* speed;
	bool isAI;
	uint8_t knob;							// knob that controls the player
	bool isLooser;
} player_t;

/**
 * Check if a player has collided with another object on the display framebuffer.
 *
 * @param playerCoordinates An array containing the current coordinates of the player.
 *                          The x-coordinate is at index 0 and the y-coordinate is at index 1.
 * @param playerDirection An array containing the current direction of the player.
 *                        The x-direction is at index 0 and the y-direction is at index 1.
 * @param fb Pointer to the display framebuffer.
 *
 * @return true if the player has collided with another object, false otherwise.
 */
bool isPlayerHitedAnotherObject(int playerCoordinates[], int playerDirection[], uint16_t* fb);

/**
 * Reset the players to their initial state.
 *
 * @param players An array of player pointers representing the players to be reset.
 */
void resetPlayers(player_t* players[]);

/**
 * Find the index of the winner among the players.
 *
 * @param players An array of player pointers representing the players to be checked.
 * @param numOfPlayers The number of players in the array.
 * @return The index of the winner player.
 */
int findWinner(player_t* players[], int numOfPlayers);

// Return random 1 or -1
void chooseRandomDirection(player_t* player);

/**
 * Retrieve the initial coordinate of a player on a specified axis.
 *
 * @param playerIndex The index of the player for which to retrieve the coordinate.
 * @param axis The axis (0 for X-axis, 1 for Y-axis) for which to retrieve the coordinate.
 *
 * @return The initial coordinate of the specified player on the specified axis.
 */
int getPlayersInitCoordinate(int playerIndex, int axis);

/**
 * Retrieve the initial direction of a player on a specified axis.
 *
 * @param playerIndex The index of the player for which to retrieve the direction.
 * @param axis The axis (0 for X-axis, 1 for Y-axis) for which to retrieve the direction.
 *
 * @return The initial direction of the specified player on the specified axis.
 */
int getPlayersInitDirection(int playerIndex, int axis);

/**
 * Retrieve the color set at the specified index.
 *
 * @param index The index of the color set to retrieve.
 *
 * @return The color set at the specified index.
 */
uint16_t getColorSet(int index);

// Initializes a player with the given parameters.
void initPlayer(player_t* player, int coordinates[2], int direction[2],
                uint32_t controlKnob, uint8_t knob, bool isLooser, bool isAI,
                player_speed_t* speed);

#endif
