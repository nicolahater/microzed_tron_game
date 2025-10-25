#ifndef DISPLAY_H
#define DISPLAY_H

#include <string.h>

#include "font_types.h"

#define DISPLAY_WIDTH 480
#define DISPLAY_HEIGHT 320

#define DISPLAY_COLOR(r, g, b) ((r << 11) | (g << 5) | (b << 0))

/**
 * Draws a character in the framebuffer at the specified position.
 *
 * @param x         The x-coordinate of the top-left corner of the character.
 * @param y         The y-coordinate of the top-left corner of the character.
 * @param ch        The character to be drawn.
 * @param fontSize  The size of the character.
 * @param color     The color of the character.
 * @param fdes      Pointer to the font descriptor containing information about the font.
 * @param fb        Pointer to the framebuffer where the character should be drawn.
 */
void drawChar(int x, int y, char ch, int fontSize, uint16_t color,
              font_descriptor_t* fdes, uint16_t *fb);

/**
 * Displays a string in the framebuffer at the specified position.
 *
 * @param x          The x-coordinate of the top-left corner of the string.
 * @param y          The y-coordinate of the top-left corner of the string.
 * @param str        The string to be drawn.
 * @param fontSize   The size of the font.
 * @param color      The color of the string.
 * @param fdes       Pointer to the font descriptor containing information about the font.
 * @param fb         Pointer to the framebuffer where the character should be drawn.
 */
void displayPrint(int x, int y, char* str, int fontSize, uint16_t color,
                  font_descriptor_t* fdes, uint16_t *fb);

/**
 * Draws a filled block with the specific color at the given position in the framebuffer.
 *
 * @param x       The x-coordinate of the top-left corner of the block.
 * @param y       The y-coordinate of the top-left corner of the block.
 * @param width   The width of the block.
 * @param height  The height of the block.
 * @param color   The color of the block in 16-bit RGB565 format.
 * @param fb      Pointer to the framebuffer array.
 */
void drawBlock(int x, int y, int width, int height, uint16_t color, uint16_t* fb);

/**
 * Draws a border of the specified size and color around the edges of the framebuffer.
 *
 * @param size    The size of the border, in pixels.
 * @param color   The color of the border in 32-bit RGB format.
 * @param fb      Pointer to the framebuffer array.
 */
void drawBorder(int size, uint32_t color, uint16_t* fb);


/**
 * Resets the display by setting all pixels in the framebuffer to 0 (black).
 *
 * @param fb  Pointer to the framebuffer to be reset.
 */
void resetDisplay(uint16_t* fb);

#endif
