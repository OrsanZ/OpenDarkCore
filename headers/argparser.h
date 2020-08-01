#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <argp.h>
#include <stdint.h>

/* Structure for the arguments used to communicate with main */
struct arguments
{
  int8_t verbose, zones, speed, lighting_mode, random, dpi_profile, lift_height, angle_snapping;
  int16_t dpi;
  color color1, color2;
};

/* Parser for single options */
error_t parse_opt(int key, char *arg, struct argp_state *state);

/* The argp parser. */
extern struct argp parser;

#endif