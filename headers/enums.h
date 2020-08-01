#ifndef ENUM_H
#define ENUM_H

/* Corresponds to internal values for the Dark Core */
enum DARK_CORE_SPEED
{
  /* Slow color cycling speed */
  DARK_CORE_SPEED_SLOW = 0x01,

  /* Normal color cycling speed */
  DARK_CORE_SPEED_NORMAL = 0x02,

  /* Fast color cycling speed */
  DARK_CORE_SPEED_FAST = 0x03
};

/* Lighting modes correspond to internal values for the Dark Core */
enum DARK_CORE_LIGHTING_MODE
{
  /* Two colors shifting from one to another */
  DARK_CORE_LIGHTING_MODE_SHIFT = 0x00,

  /* Two alternating colors pulsing */
  DARK_CORE_LIGHTING_MODE_PULSE = 0x01,

  /* Rainbow cycle */
  DARK_CORE_LIGHTING_MODE_RAINBOW = 0x03,

  /* Static color */
  DARK_CORE_LIGHTING_MODE_STATIC = 0x07,
};

#endif