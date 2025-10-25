#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "font_types.h"

#include "player.h"
#include "display.h"
#include "game.h"

/**
 * Displays a menu and handles user input for menu navigation.
 *
 * This function displays a menu on a display device and handles user input
 * through a green knob button to navigate the menu options. It updates the
 * display and returns a menu selection value.
 *
 * @param green_knob_value Pointer to the current value of the green knob
 * @param players[]        An array of player_t pointers representing the players in the game
 * @param fb               Pointer to framebuffer for the display device.
 *
 * @return value indicating the next state of the program
 */
int menu(uint32_t* green_knob_value, player_t* players[], uint16_t* fb);

/**
 * Submenu function for selecting game options.
 * This function displays a sub-menu screen where players can select game options such as colors and game modes.
 * It handles knob rotations and button presses to update the selection and navigate between options.
 *
 * @param old_knobs_value Pointer to the previous knob values.
 * @param green_knob_value Pointer to the current value of the green knob.
 * @param players An array of player_t pointers representing the players in the game
 * @param numOfPlayers Number of players.
 * @param mode Pointer to the game mode.
 * @param fb Pointer to the framebuffer for displaying.
 *
 * @return Integer value representing the next state of the program.
 */
int subMenu(uint32_t* old_knobs_value, uint32_t* green_knob_value,
            player_t* players[], int numOfPlayers, int* mode, uint16_t* fb);

#endif
