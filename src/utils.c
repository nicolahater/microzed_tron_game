#include "utils.h"


int getRandomSign() {
    srand(time(NULL));

    int randomNumber = rand() % 2;
    int randomSign = (randomNumber == 0) ? 1 : -1;

    return randomSign;
}

void setLEDLine(uint32_t* line, uint8_t value, int ledPosition) {
	int offset;
	uint32_t mask = 0xFF;

	switch(ledPosition){
		case FIRST_BAR:
			offset = 0;
			break;
		case SECOND_BAR:
			offset = 8;
			break;
		case THIRD_BAR:
			offset = 16;
			break;
		case FOURTH_BAR:
			offset = 24;
			break;
		default:
			 fprintf(stderr, "ledPosition must in the range 1 - 4 \n");
			 exit(2);
	}

	mask = mask << offset;
	*line = (*line & ~mask) | (value << offset);
}


uint16_t getKnobValue(uint32_t knob_value, uint8_t knob) {
	uint16_t value = 0;

	switch(knob) {
		case R:
			value = RED_KNOB(knob_value);
			break;
		case G:
			value = GREEN_KNOB(knob_value);
			break;
		case B:
			value = BLUE_KNOB(knob_value);
			break;
		default:
			printf("getKnobValue() error\n");
			exit(1);
	}

	return value;
}

bool isKnobPressed(uint32_t knob_value, uint8_t knob) {
	bool isPressed;

	switch(knob) {
		case R:
			isPressed = IS_RED_KNOB_IS_PRESSED(knob_value);
			break;
		case G:
			isPressed = IS_GREEN_KNOB_IS_PRESSED(knob_value);
			break;
		case B:
			isPressed = IS_BLUE_KNOB_IS_PRESSED(knob_value);
			break;
		default:
			printf("isKnobPressed() error\n");
			exit(1);
	}

	return isPressed;
}
