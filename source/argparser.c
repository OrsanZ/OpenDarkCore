#include <argp.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdint.h>

#include "../headers/color.h"
#include "../headers/constants.h"
#include "../headers/enums.h"
#include "../headers/argparser.h"

/* Argp Globals */

/* Program name and version */
const char *argp_program_version = "OpenDarkCore 2.0";

/* Custom documentation */
static char doc[] = "OpenDarkCore - A CLI for the CORSAIR DARK CORE\n"
                    "This program currently only supports the DARK CORE SE,"
                    "but adding the regular DARK CORE likely requires nothing more than running lsusb while it is connected and opening an issue with it"
                    "and submitting the result as an issue on https://github.com/OrsanZ/OpenDarkCore.";

static struct argp_option options[] = {
    {"verbose", 'v', 0, 0, "Produce verbose output"},
    {"static", 's', 0, 0, "Use static lighting mode"},
    {"pulse", 'p', 0, 0, "Use pulse lighting mode"},
    {"shift", 'S', 0, 0, "Use shift lighting mode"},
    {"rainbow", 'r', 0, 0, "Use rainbow lighting mode"},
    {"speed", 'V', "SPEED", 0, "The speed of the lighting effect (Valid from 1-3)"},
    {"zones", 'z', "ZONES", 0, "The zones to be affected by the lighting change."},
    {"color", 'c', "COLOR", 0, "The primary color for the chosen lighting effect [Default: FF0000]"},
    {"color2", 'C', "COLOR", 0, "The secondary color for the chosen lighting effect [Default: FF0000]"},
    {"random", 'R', 0, 0, "Whether the colors for the chosen effect should be random (Only valid for shift and pulse)"},
    {"dpi", 'd', "DPI", 0, "Use DPI changing mode (Valid from 100 to 16000)"},
    {"profile", 'P', "PROFILE", 0, "Selects the DPI profile (0: Sniper) [Default: 1]"},
    {"anglesnapping", 'a', "SNAPPING", 0, "Turns angle snapping on or off. (Off / On)"},
    {"liftheight", 'l', "HEIGHT", 0, "Changes the lift height. (1: Low, 2: Normal, 3: High)"},
    {0}};

/* Parser for single options */
error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  /* Error number for hextocolor */
  uint16_t err = 0;

  /* Switch for the various options */
  switch (key)
  {
  /* --verbose, -v */
  case 'v':
    arguments->verbose = 1;
    break;

  /* --static, -s */
  case 's':
    if (arguments->lighting_mode != DARK_CORE_UNSPECIFIED)
    {
      argp_error(state, "You can only specify one lighting mode.");
    }
    arguments->lighting_mode = DARK_CORE_LIGHTING_MODE_STATIC;
    break;

  /* --pulse, -p */
  case 'p':
    if (arguments->lighting_mode != DARK_CORE_UNSPECIFIED)
    {
      argp_error(state, "You can only specify one lighting mode.");
    }
    arguments->lighting_mode = DARK_CORE_LIGHTING_MODE_PULSE;
    break;

  /* --shift, -S */
  case 'S':
    if (arguments->lighting_mode != DARK_CORE_UNSPECIFIED)
    {
      argp_error(state, "You can only specify one lighting mode.");
    }
    arguments->lighting_mode = DARK_CORE_LIGHTING_MODE_SHIFT;
    break;

  /* --rainbow, -r */
  case 'r':
    if (arguments->lighting_mode != DARK_CORE_UNSPECIFIED)
    {
      argp_error(state, "You can only specify one lighting mode.");
    }
    arguments->lighting_mode = DARK_CORE_LIGHTING_MODE_RAINBOW;
    break;

  /* --speed, -V */
  /* Semicolon used as a workaround, because I cant make a declaration after the case otherwise */
  case 'V':;
    unsigned long speed = strtoul(arg, 0, 10);
    if (speed < 1 || speed > 3)
    {
      argp_error(state, "Please enter a speed value from 1 to 3.");
    }
    arguments->speed = speed;
    break;

  /* --zones, -z */
  case 'z':;
    unsigned long zones = strtoul(arg, 0, 10);
    if (zones < 1 || zones > 7)
    {
      argp_error(state, "Please enter a zones value from 1 to 7.");
    }
    arguments->zones = zones;
    break;

  /* --color, -c */
  case 'c':;
    err = hextocolor(arg, &(arguments->color1));

    if (err != 0)
    {
      argp_error(state, "Bad color format.");
    }
    break;

  /* --color2, -C */
  case 'C':;
    err = hextocolor(arg, &(arguments->color2));

    if (err != 0)
    {
      argp_error(state, "Bad color format.");
    }
    break;

  /* --random, -R */
  case 'R':
    arguments->random = 1;
    break;

  /* --dpi, -d */
  case 'd':;
    unsigned long dpi = strtoul(arg, 0, 10);
    if (dpi < 100 || dpi > 16000)
    {
      argp_error(state, "Please enter a DPI value from 100 to 16000.");
    }
    arguments->dpi = dpi;
    break;

  /* --profile, -P */
  case 'P':
    /* If clause to distringuish between "0" and an unparsable string */
    if (strcmp(arg, "0") == 0)
    {
      arguments->dpi_profile = 0;
      break;
    }

    unsigned long dpi_profile = strtoul(arg, 0, 10);
    if (dpi_profile < 1 || dpi_profile > 3)
    {
      argp_error(state, "Please enter a valid DPI profile from 0 to 3.");
    }
    arguments->dpi_profile = dpi_profile;
    break;

  /* --anglesnapping, -a */
  case 'a':;
    if (strcasecmp(arg, "on") == 0)
    {
      arguments->angle_snapping = 1;
      break;
    }

    if (strcasecmp(arg, "off") == 0)
    {
      arguments->angle_snapping = 0;
      break;
    }
    argp_error(state, "Please enter a valid value for angle snapping.");
    break;

  /* -liftheight, -l */
  case 'l':;
    unsigned long height = strtoul(arg, 0, 10);

    if (height < 1 || height > 3)
    {
      argp_error(state, "Please enter a valid lift height from 1 to 3.");
    }

    arguments->lift_height = height;
    break;

  /* We are not expecting any arguments */
  case ARGP_KEY_ARG:
    argp_usage(state);
    break;

  /* Checks if the combination of options is valid */
  case ARGP_KEY_END:
    /* If neither DPI or lighting options were given, quit */
    if (arguments->lighting_mode == DARK_CORE_UNSPECIFIED && arguments->dpi == DARK_CORE_UNSPECIFIED &&
        arguments->lift_height == DARK_CORE_UNSPECIFIED && arguments->angle_snapping == DARK_CORE_UNSPECIFIED)
      argp_error(state, "No action was specified.");

    /* Add alternating random mode */
    if ((arguments->lighting_mode == DARK_CORE_LIGHTING_MODE_PULSE || arguments->lighting_mode == DARK_CORE_LIGHTING_MODE_SHIFT) && arguments->random != 1)
      arguments->random = 3;

    /* If the lighting mode is static and speed was defined, quit */
    if (arguments->lighting_mode == DARK_CORE_LIGHTING_MODE_STATIC)
    {
      if (arguments->speed != DARK_CORE_UNSPECIFIED)
        argp_error(state, "Speed option incompatible with static lighting mode.");

      /* Set speed to 0 if mode is static */
      arguments->speed = 0;
    }
    else
    {
      /* Otherwise set speed to 2 (Normal) */
      arguments->speed = 2;
    }
  default:
    return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

/* The argp parser. */
struct argp parser = {options, parse_opt, NULL, doc};