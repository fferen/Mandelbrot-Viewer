#pragma once

#include <SDL/SDL.h>

typedef struct _RGBColor {
    Uint8 r, g, b;
} RGBColor;

/* Print {r, g, b}. */
void print_rgb(RGBColor color);

/* Return RGBColor from hue [0-6], saturation [0-1] and value [0-1]. */
RGBColor hsv_to_rgb(float h, float s, float v);
