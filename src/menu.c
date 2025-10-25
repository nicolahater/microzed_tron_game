#include "menu.h"

int menu(uint32_t* green_knob_value, player_t* players[], uint16_t* fb) {
	int ret = MENU;
	font_descriptor_t* fdes = &font_rom8x16;
	int clockwiseRange, counterclockwise;
	static int flashing = 0;
	static int button = 0;
	bool isGreenKnobPresesd = false;

	int y = 5;

	displayPrint(DISPLAY_WIDTH/2 - 8*5*5/2 + 10, y, "TRON", 5, DISPLAY_COLOR(0, 0, 31), fdes, fb);
	drawBlock(0, y + 30*3*(button + 1) - 10, DISPLAY_WIDTH, 16*3 + 10, DISPLAY_COLOR(0, 0, 0), fb);

	static int x2 = 1;
	int y2 = y + 30*3*3;

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

	// flashing block effect on highlighted menu item
	if(flashing < 5 || isGreenKnobPresesd){
		drawBlock(0, y + 30*3*(button + 1) - 10, DISPLAY_WIDTH, 16*3 + 10, DISPLAY_COLOR(31, 0, 0), fb);
	}
	flashing = (flashing+1) % 10;

	// Display the menu options
	displayPrint(DISPLAY_WIDTH/2 - 8*8*3/2, y + 30*3, "IP vs PC", 3, DISPLAY_COLOR(4, 9, 19), fdes, fb);
	displayPrint(DISPLAY_WIDTH/2 - 8*9*3/2, y + 30*2*3, "IP vs IIP", 3, DISPLAY_COLOR(4, 9, 19), fdes, fb);

	for(int i = 0; i < 16*3; i++) {
		for(int j = 0; j < 16*8*3; j++) {
			fb[(y2+i)*DISPLAY_WIDTH + (x2-1)+j] = 0;
		}
	}

	// running line at the bottom of the display
	displayPrint(x2%DISPLAY_WIDTH, y2, "CTU FEL 2023 ", 3, getColorSet(5), fdes, fb);
	x2 = (x2+1)%DISPLAY_WIDTH;

	// Handle green knob button press
	if(IS_GREEN_KNOB_IS_PRESSED(*green_knob_value)){
		resetDisplay(fb);
		isGreenKnobPresesd = true;
		if(button == 0) {
			players[1]->isAI = true;
		} else {
			players[1]->isAI = false;
		}
		ret = SUB_MENU;
	}

	// Delay before returning the menu selection
	if(ret != MENU) sleep(1);
	return ret;
}

int subMenu(uint32_t* old_knobs_value, uint32_t* green_knob_value, player_t* players[], int numOfPlayers, int* mode, uint16_t* fb){
	font_descriptor_t* fdes = &font_rom8x16;
	int ret = SUB_MENU;
	int clockwiseRange, counterclockwise;
	bool isGreenKnobPresesd = false;

	displayPrint(DISPLAY_WIDTH/4 - 5*8, 5, "COLOR", 3, DISPLAY_COLOR(0, 0, 31), fdes, fb);
	displayPrint(DISPLAY_WIDTH/1.3 - 5*8, 5, "MODE", 3, DISPLAY_COLOR(0, 0, 31), fdes, fb);

	int x = 20;
	int y = 60;
	for(int i = 0; i < 9; i++) {
		drawBlock(x + (i%3)*70 + (i%3)*10, 70*(i/3) + y + (i/3)*10, 70, 70, getColorSet(i), fb);
	}

	static int flashing = 0;

	static int numberPosition[2] = {0, 8};

	for(int i = 0; i < numOfPlayers; i++) {
		player_t* player = players[i];
		uint32_t new_knob_value = player->knobsValue;

		// Calculates the range of clockwise and counterclockwise rotation of the red and blue knobs
		clockwiseRange = (((new_knob_value & player->controlKnob) - (*old_knobs_value & player->controlKnob)) + 255) % 255;
		counterclockwise = (((*old_knobs_value & player->controlKnob) - (new_knob_value & player->controlKnob)) + 255) % 255;

		// Update the color item selection based on red and blue knobs rotation
		static int step = 1;
		if(clockwiseRange < counterclockwise && abs(clockwiseRange) > 1) {
			if(numOfPlayers > 1 && numberPosition[(i+1)%numOfPlayers] == (numberPosition[i] + 1) % 9) step = 2;
			numberPosition[i] = (numberPosition[i] + step) % 9;

		} else if(clockwiseRange > counterclockwise && abs(counterclockwise) > 1)  {
			if(numOfPlayers > 1 && numberPosition[(i+1)%numOfPlayers] == (numberPosition[i] + 8) % 9) step = 2;
			numberPosition[i] = (numberPosition[i] + 9 - step) % 9;

		}

		step = 1;
		*old_knobs_value = (new_knob_value & player->controlKnob) | (*old_knobs_value & ~player->controlKnob);
	}

	static int button = 0;
	int colorButton = 6;
	static uint32_t old_green_knob_value = 0;

	drawBlock(260, 70*button + y + button*10, 220, 70, 0, fb);

	if(old_green_knob_value == 0) {
		old_green_knob_value = *green_knob_value;
	}

	// Calculates the range of clockwise and counterclockwise rotation of the green knob
	clockwiseRange = ((GREEN_KNOB(*green_knob_value) - GREEN_KNOB(old_green_knob_value)) + 256) % 256;
	counterclockwise = ((GREEN_KNOB(old_green_knob_value) - GREEN_KNOB(*green_knob_value)) + 256) % 256;

	// Update the color item selection based on green knob rotation
	if(clockwiseRange < counterclockwise && clockwiseRange > 1) {
		button = (button+1) % 3;
	} else if(clockwiseRange > counterclockwise && counterclockwise > 1)  {
		button = (button+2) % 3;
	}
	old_green_knob_value = *green_knob_value;

	if(button == 2) colorButton = 0;
	else colorButton = 6;


	// flashing block effect on highlighted menu item
	if(flashing < 5 || isGreenKnobPresesd) {
		displayPrint(20 + 70/2 - 8*3/2 + 2 + (numberPosition[0]%3)*(70+10), 62 + 70/2 - 16*3/2 + (numberPosition[0]/3)*(70+10),
																			"1", 3, DISPLAY_COLOR(31, 63, 31), fdes, fb);
		if (numOfPlayers > 1)
		displayPrint(20 + 70/2 - 8*3/2 + 2 + (numberPosition[1]%3)*(70+10), 62 + 70/2 - 16*3/2 + (numberPosition[1]/3)*(70+10),
																			"2", 3, DISPLAY_COLOR(31, 63, 31), fdes, fb);
		drawBlock(260, 70*button + y + button*10, 220, 70, getColorSet(colorButton), fb);
	}
	flashing = (flashing+1) % 10;

	// Display the sub menu options
	displayPrint(DISPLAY_WIDTH/1.5 - 5*8, 75, "CLASSIC", 3, DISPLAY_COLOR(31, 63, 31), fdes, fb);
	displayPrint(DISPLAY_WIDTH/1.6 - 5*8, 155, "INFINITY", 3, DISPLAY_COLOR(31, 63, 31), fdes, fb);
	displayPrint(DISPLAY_WIDTH/1.4 - 5*8, 155 + 80, "BACK", 3, DISPLAY_COLOR(31, 63, 31), fdes, fb);

	// Handle green knob button press
	if(IS_GREEN_KNOB_IS_PRESSED(*green_knob_value)) {
		isGreenKnobPresesd = true;
		players[0]->color = getColorSet(numberPosition[0]);
		players[1]->color = getColorSet(numberPosition[1]);

		if(button == 0 || button == 1) {
			resetDisplay(fb);
			// Check which button is selected
			if(button == 0) {
				// Button 0 is selected: Classic mode
				// Draw the border
				drawBorder(BORDER_SIZE, BORDER_COLOR, fb);
				*mode = CLASSIC;
			} else {
				*mode = INFINITY;
			}
			ret = GAME;
		} else if (button == 2) {
			// Button 2 is selected: Back to menu
			resetDisplay(fb);
			ret = MENU;
		}
	}

	if (ret != SUB_MENU){
		resetPlayers(players);
		sleep(1);
	}
	return ret;
}
