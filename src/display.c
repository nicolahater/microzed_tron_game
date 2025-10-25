#include "display.h"

void drawChar(int x, int y, char ch, int fontSize, uint16_t color, font_descriptor_t* fdes, uint16_t *fb) {
	for(int i = 0; i < fdes->height; i++) {
		for(int j = 0; j < 16; j++) {
			if(((fdes->bits[16*ch + i] >> (15-j)) & 0x1) == 0) {
				continue;
			}

			for(int h = 0; h < fontSize; h++) {
				for(int w = 0; w < fontSize; w++) {
					fb[(y+i*fontSize+h)*DISPLAY_WIDTH + x+fontSize*j+w] = color;
				}
			}
		}
	}
}

void displayPrint(int x, int y, char* str, int fontSize, uint16_t color, font_descriptor_t* fdes, uint16_t *fb) {
	int length = strlen(str);
	int char_width = fdes->maxwidth;

	for(int i = 0; i < length; i++) {
		drawChar(x + i*fontSize*char_width, y, str[i], fontSize, color, fdes, fb);
	}
}

void drawBlock(int x, int y, int width, int height, uint16_t color, uint16_t* fb) {
	for(int row = 0; row < height; row++) {
		for(int clm = 0; clm < width; clm++) {
			fb[DISPLAY_WIDTH*(y+row) + x+clm] = color;
		}
	}
}


void drawBorder(int size, uint32_t color, uint16_t* fb) {
	for (int ptr = 0; ptr < DISPLAY_WIDTH*DISPLAY_HEIGHT; ptr++) {
		if(ptr < DISPLAY_WIDTH*size || ptr >= (DISPLAY_HEIGHT-size)*DISPLAY_WIDTH
		|| ptr%DISPLAY_WIDTH < size || ptr%DISPLAY_WIDTH > (DISPLAY_WIDTH - size)) fb[ptr] = color;
	}	
}

void resetDisplay(uint16_t* fb) {
	for (int ptr = 0; ptr < DISPLAY_HEIGHT*DISPLAY_WIDTH; ptr++){
		fb[ptr] = 0u;
	}
}