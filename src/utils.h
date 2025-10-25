#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MENU 0
#define SUB_MENU 1
#define GAME 2
#define OVER 3
#define PAUSE 4

#define R 0
#define G 1
#define B 2

#define RED_KNOB_OFFSET 16
#define GREEN_KNOB_OFFSET 8
#define BLUE_KNOB_OFFSET 0

#define RED_KNOB_MASK (0xFF << 16)
#define GREEN_KNOB_MASK (0xFF << 8)
#define BLUE_KNOB_MASK (0xFF << 0)

#define RED_KNOB(color) ((color >> 16) & 0xFF)
#define GREEN_KNOB(color) ((color >> 8) & 0xFF)
#define BLUE_KNOB(color) ((color >> 0) & 0xFF)

#define IS_RED_KNOB_IS_PRESSED(value) ((value >> 26) & 0x1)
#define IS_GREEN_KNOB_IS_PRESSED(value) ((value >> 25) & 0x1)
#define IS_BLUE_KNOB_IS_PRESSED(value) ((value >> 24) & 0x1)

#define FIRST_BAR 1
#define SECOND_BAR 2
#define THIRD_BAR 3
#define FOURTH_BAR 4

#define NUM_OF_PLAYERS 2

/**
 * Get a random sign (+1 or -1).
 *
 * @return The randomly generated sign (+1 or -1).
 */
int getRandomSign();

/**
 * Get the value of the specific knob.
 *
 * @param knob_value The value representing the state of the knobs.
 * @param knob The knob to get the value from (R, G, or B).
 *
 * @return The value of the specified knob.
 */
uint16_t getKnobValue(uint32_t knob_value, uint8_t knob);

/**
 * Checks if a specific knob is pressed based on the knob value.
 *
 * @param knob_value The value representing the state of the knobs.
 * @param knob The knob to check (R, G, or B).
 *
 * @return true if the specified knob is pressed, false otherwise.
 */
bool isKnobPressed(uint32_t knob_value, uint8_t knob);

/**.
 * Set specific value at the given quarter of the LED line
 *
 * @param line A pointer to the LED buffer.
 * @param value The value to set for the LED quarter (8-bit value).
 * @param ledPosition The number of the quarter(from right to left).
 */
void setLEDLine(uint32_t* line, uint8_t value, int ledPosition);

#endif
