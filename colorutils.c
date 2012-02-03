#include <stdio.h>
#include <math.h>

#include <SDL/SDL.h>

#include "colorutils.h"

RGBColor hsv_to_rgb(float h, float s, float v) {
    int h_i = floor(h);
    float h_f = h - h_i;
    if (!(h_i & 1)) {
        /* h_i is even */
        h_f = 1 - h_f;
    }
    float m = v * (1 - s);
    float n = v * (1 - s * h_f);

    v *= 255;
    m *= 255;
    n *= 255;
    switch (h_i) {
        case 6:
        case 0:
            return (RGBColor) {v, n, m};
        case 1:
            return (RGBColor) {n, v, m};
        case 2:
            return (RGBColor) {m, v, n};
        case 3:
            return (RGBColor) {m, n, v};
        case 4:
            return (RGBColor) {n, m, v};
        case 5:
            return (RGBColor) {v, m, n};
        default:
            /* shouldn't happen, but avoids warnings */
            return (RGBColor) {0, 0, 0};
    }
}

void print_rgb(RGBColor color) {
    printf("{%d, %d, %d}\n", color.r, color.g, color.b);
}
