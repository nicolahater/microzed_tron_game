#include "game.h"

int playGame(player_t* players[], uint8_t numOfPlayers, uint32_t* old_knobs_value, uint32_t* LEDline, uint32_t leds[], int mode, uint16_t* fb) {
	uint8_t ret = GAME;
	int clockwiseRange, counterclockwise;
	bool isOver = false;
	static int changeDirectionTimer = 0;
	static uint8_t accelerationLEDLine[2] = {0xFF, 0xFF};
  static bool isResetTime = true;

		// Reset the game if it's the first run
    if(isResetTime) {
        if(!players[1]->isAI) setLEDLine(LEDline, 0xFF, 2);
				setLEDLine(LEDline, 0xFF, 3);
        resetPlayers(players);
        isResetTime = false;
    }

	// Process each player
	for(int i = 0; i < numOfPlayers; i++) {
		player_t* player = players[i];

		if(player->isAI) {
			if(changeDirectionTimer > 40) {
				chooseRandomDirection(player);
				changeDirectionTimer = 0;
			}
		} else {
			int x = player->direction[0];
			int y = player->direction[1];

			// Calculates the range of clockwise and counterclockwise rotation of the player's knobs
			clockwiseRange = ((getKnobValue(player->knobsValue, player->knob) - getKnobValue(*old_knobs_value, player->knob)) + 256) % 256;
			counterclockwise = ((getKnobValue(*old_knobs_value, player->knob) - getKnobValue(player->knobsValue, player->knob)) + 256) % 256;

			// Update the color item selection based on player's knobs rotation
			if(clockwiseRange < counterclockwise && clockwiseRange > 1) {
				player->direction[0] = (abs(x)-1) * -y;
				player->direction[1] = (abs(y)-1) * x;
			} else if(clockwiseRange > counterclockwise && counterclockwise > 1)  {
				player->direction[0] = (abs(x)-1) * y;
				player->direction[1] = (abs(y)-1) * -x;
			}

			// Handle player acceleration and acceleration recovery
			if(player->speed->durationCounter[i] >= player->speed->accelerationDuration) {
			player->speed->durationCounter[i] = 0;

			if(player->speed->amountOfAccelerations[i] > 0) {
					accelerationLEDLine[i] = i ? (accelerationLEDLine[i] << 1) : (accelerationLEDLine[i] >> 1);
					setLEDLine(LEDline, accelerationLEDLine[i], 3-i);
					player->speed->amountOfAccelerations[i]--;
				}
			}

			if(player->speed->regenerationCounter[i] >= player->speed->accelerationRegeneration) {
				player->speed->regenerationCounter[i] = 0;

				if(player->speed->amountOfAccelerations[i] < player->speed->maxAmountOfAccelerations) {
					player->speed->amountOfAccelerations[i]++;
					accelerationLEDLine[i] = i ? ((accelerationLEDLine[i] >> 1) | (0x1 << 7)) : ((accelerationLEDLine[i] << 1) | 0x1);
					setLEDLine(LEDline, accelerationLEDLine[i], 3-i);
				}
			}

			if(isKnobPressed(player->knobsValue, player->knob)){
        leds[i] = 0xFF0000;
				if(player->speed->amountOfAccelerations[i] > 0) player->speed->acceleration = 2;
				player->speed->durationCounter[i]++;
			} else {
                leds[i] = 0;
				player->speed->regenerationCounter[i]++;
				player->speed->acceleration = 1;
			}
		}

		// Update player coordinates based on speed, direction, and acceleration
		player->coordinates[0] += (player->speed->acceleration)*(player->speed->speed)*(player->direction[0]);
		player->coordinates[1] += -(player->speed->acceleration)*(player->speed->speed)*(player->direction[1]);

		// Handle player coordinates wrapping around the screen
    if(player->coordinates[0] <= 0) player->coordinates[0] = DISPLAY_WIDTH - 1;
    else if(player->coordinates[0] >= DISPLAY_WIDTH) player->coordinates[0] = 0;
		if(player->coordinates[1] <= 0) player->coordinates[1] = DISPLAY_HEIGHT - 1;
  	else if(player->coordinates[1] >= DISPLAY_HEIGHT) player->coordinates[1] = 0;

		fb[player->coordinates[1]*DISPLAY_WIDTH + player->coordinates[0]] = player->color;

        if(!isPlayerHitedAnotherObject(player->coordinates, player->direction, fb)) {
            player->isLooser = true;
            isOver = true;
            ret = OVER;
        }

		// Draw the player on the frame buffer
		for(int y = 0; y < PLAYER_SIZE; y++) {
			for(int x = 0; x < PLAYER_SIZE; x++) {
				fb[(player->coordinates[1]+y)*DISPLAY_WIDTH + player->coordinates[0]+x] = player->color;

			}
		}

	}

	// Reset acceleration for both players
	players[0]->speed->acceleration = 1;
	players[1]->speed->acceleration = 1;

	// timer for AI
	changeDirectionTimer++;

	// Update old knob values with current knob values
	*old_knobs_value = (*old_knobs_value & ~BLUE_KNOB_MASK) | ((getKnobValue(players[1]->knobsValue, players[1]->knob) << BLUE_KNOB_OFFSET));
	*old_knobs_value = (*old_knobs_value & ~RED_KNOB_MASK) | ((getKnobValue(players[0]->knobsValue, players[0]->knob) << RED_KNOB_OFFSET));

	// Restart the game if it is game is over
	if(isOver) {
		setLEDLine(LEDline, 0xFF, 2);
		setLEDLine(LEDline, 0xFF, 3);
    leds[0] = 0;
    leds[1] = 0;
    isResetTime = true;
		accelerationLEDLine[0] = 0xFF;
		accelerationLEDLine[1] = 0xFF;
	}

	// Update LED lines if the game is over
  if(ret != GAME){
    setLEDLine(LEDline, 0, 2);
		setLEDLine(LEDline, 0, 3);
  }

	return ret;
}

int gameOver(int winner, uint32_t* green_knob_value, int mode, uint16_t* fb) {
	int ret = OVER;
	int clockwiseRange, counterclockwise;
	font_descriptor_t* fdes = &font_rom8x16;
	static int button = 0; // Variable to track the selected button
	static int flashing = 0;
	bool isGreenKnobPresesd = false;

	char* winnersSet[] = {"Player 1 win", "Player 2 win"};
	int blockWidth = 300;
	int blockHeight = 200;
	int fontSize = 3;

	drawBlock(DISPLAY_WIDTH/2 - blockWidth/2, DISPLAY_HEIGHT/2 - blockHeight/2, blockWidth, blockHeight, getColorSet(0), fb);
	displayPrint(DISPLAY_WIDTH/2 - blockWidth/2 + 10, DISPLAY_HEIGHT/2 - blockHeight/2 + 10, winnersSet[winner], fontSize, (uint16_t)0xFFFFFF, fdes, fb);

	static uint32_t old_green_knob_value = 0;
	if(old_green_knob_value == 0) {
		old_green_knob_value = *green_knob_value;
	}

	// Calculates the range of clockwise and counterclockwise rotation of the green knob
	clockwiseRange = ((GREEN_KNOB(*green_knob_value) - GREEN_KNOB(old_green_knob_value)) + 256) % 256;
	counterclockwise = ((GREEN_KNOB(old_green_knob_value) - GREEN_KNOB(*green_knob_value)) + 256) % 256;

	// Update the menu item selection based on green knob rotation
	if(clockwiseRange < counterclockwise && clockwiseRange > 1) {
		button = (button+1) % 2;
	} else if(clockwiseRange > counterclockwise && counterclockwise > 1)  {
		button = (button+1) % 2;
	}
	old_green_knob_value = *green_knob_value;

	if(IS_GREEN_KNOB_IS_PRESSED(*green_knob_value)){
		isGreenKnobPresesd = true;
		if(button == 0) ret = GAME;
		else ret = MENU;
	}

	// flashing block effect on highlighted menu item
	if(flashing < 5 || isGreenKnobPresesd){
		drawBlock(DISPLAY_WIDTH/2 - blockWidth/2, DISPLAY_HEIGHT/2 - blockHeight/2 + 75 + button*50, blockWidth, 50, getColorSet(6), fb);
	}
	flashing = (flashing+1) % 10;


	displayPrint(DISPLAY_WIDTH/2 - blockWidth/2 + 60, DISPLAY_HEIGHT/2 - blockHeight/2 + 75, "RESTART", fontSize, (uint16_t)0xFFFFFF, fdes, fb);
	displayPrint(DISPLAY_WIDTH/2 - blockWidth/2 + 40, DISPLAY_HEIGHT/2 - blockHeight/2 + 130, "MAIN MENU", fontSize, (uint16_t)0xFFFFFF, fdes, fb);

	if (ret != OVER) {
		sleep(1);
		resetDisplay(fb);
        if (mode == CLASSIC && ret == GAME){
            drawBorder(BORDER_SIZE, BORDER_COLOR, fb);
            }
	}
	return ret;
}
