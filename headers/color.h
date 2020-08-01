#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} color;

/* Parses a hex color like ff00cc to the color type */
int16_t hextocolor(char *hex, color *color);

#endif