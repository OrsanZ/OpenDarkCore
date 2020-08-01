#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "../headers/color.h"

/* Parses a hex color like ff00cc to the color type */
int16_t hextocolor(char *hex, color *color)
{
  int8_t hues[3] = {};

  /* Loops through all 3 parts of a hex color string */
  for (size_t i = 0; i < 3; i++)
  {
    /* Parses 2 int8_tacters to long */
    int8_t huestring[3];

    /* Equivalent to hues.substr(i*2, 2) */
    snprintf(huestring, 3, "%.*s", 2, hex + i * 2);

    if (strcmp(huestring, "00") == 0)
    {
      hues[i] = 0;
      continue;
    }

    uint32_t hue = strtoul(huestring, 0, 16);

    /* If hue value is bigger than 255 or smaller than 1 (because we already checked for 0), quit */
    if (hue < 1 || hue > 255)
    {
      return -1;
    }
    hues[i] = hue;
  }

  /* Finish the color struct */
  color->red = hues[0];
  color->green = hues[1];
  color->blue = hues[2];

  return 0;
}