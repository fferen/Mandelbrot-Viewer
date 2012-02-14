/*
Copyright (c) 2012, Kevin Han
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
