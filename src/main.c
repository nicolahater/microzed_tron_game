#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "serialize_lock.h"
#include "font_types.h"

#include "menu.h"
#include "player.h"
#include "utils.h"
#include "display.h"
#include "game.h"


int main(int argc, char *argv[]) {
	// Mapping physical addresses
	volatile void *lcdBaseAddr = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);
	volatile unsigned char *mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);

	if(lcdBaseAddr == NULL) {
		fprintf(stderr, "Something failed\n");
		exit(-1);
	}

	// Initializing the LCD display
	parlcd_hx8357_init((unsigned char *) lcdBaseAddr);

	uint16_t *fb = malloc(DISPLAY_WIDTH*DISPLAY_HEIGHT*sizeof(uint16_t));

	// Creating player objects
	player_t player1;
	player_t player2;

	// Initializing player speed settings
	player_speed_t speed;
	speed.speed = 2;
	speed.acceleration = 1;
	speed.maxAmountOfAccelerations = 8;
	speed.accelerationDuration = 10;
	speed.accelerationRegeneration = 20;
	speed.durationCounter = calloc(NUM_OF_PLAYERS, sizeof(int));
	speed.regenerationCounter = calloc(NUM_OF_PLAYERS, sizeof(int));
	speed.amountOfAccelerations = calloc(NUM_OF_PLAYERS, sizeof(int));
	speed.amountOfAccelerations[0] = speed.maxAmountOfAccelerations;
	speed.amountOfAccelerations[1] = speed.maxAmountOfAccelerations;

	// Initializing player 1
	int coordinates[2] = {40, 160};
	int direction[2] = {1, 0};
	initPlayer(&player1, coordinates, direction, RED_KNOB_MASK, R, false, false, &speed);

	// Initializing player 2
	coordinates[0] = 440;
	coordinates[1] = 160;
	direction[0] = -1;
	direction[1] = 0;
	initPlayer(&player2, coordinates, direction, BLUE_KNOB_MASK, B, false, false, &speed);

	player_t* players[] = {&player1, &player2};

	uint32_t old_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	uint32_t new_knobs_value;

	// Clearing the framebuffer
	resetDisplay(fb);

	int winner = -1;
	uint32_t leds[2] = {0, 0};
	int mode;
	uint32_t line = 0;
	int state = MENU;

	// Game loop
	while(true) {
		// Reading the current knob values
		new_knobs_value = *(volatile uint32_t*)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		player1.knobsValue = new_knobs_value;
		player2.knobsValue = new_knobs_value;

		// Updating the LED values
		*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = leds[0];
		*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = leds[1];

		 // Handling different states
		switch(state){
			case MENU:
				state = menu(&new_knobs_value, players, fb);
				break;
			case SUB_MENU:
				state = subMenu(&old_knobs_value, &new_knobs_value, players, NUM_OF_PLAYERS, &mode, fb);
				break;
			case GAME:
				// Reset the winner variable
				winner = -1;
				state = playGame(players, NUM_OF_PLAYERS, &old_knobs_value, &line, leds, mode, fb);
				break;
			case OVER:
				// If winner is not determined, find the winner
				if(winner == -1) {
					winner = findWinner(players, NUM_OF_PLAYERS);
				}

				state = gameOver(winner, &new_knobs_value, mode, fb);
				break;
		}

		// Updating the framebuffer on the LCD display
		*(volatile uint16_t *)(lcdBaseAddr + PARLCD_REG_CMD_o) = 0x2c;
		for(int ptr = 0; ptr < DISPLAY_WIDTH*DISPLAY_HEIGHT; ptr++) {
			*(volatile uint16_t *)(lcdBaseAddr + PARLCD_REG_DATA_o) = fb[ptr];
		}

		 // Updating the LED line
		*(volatile uint32_t*)(mem_base + SPILED_REG_LED_LINE_o) = line;
	}

	free(fb);
	free(speed.durationCounter);
	free(speed.regenerationCounter);
	free(speed.amountOfAccelerations);
}
