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

#pragma once

typedef long double LongDbl;

typedef struct _Bounds {
    LongDbl x_min, x_max, y_min, y_max;
} Bounds;

typedef struct _Point {
    LongDbl x, y;
} Point;

void print_pt(Point p);

void print_bounds(Bounds b);

LongDbl bounds_w(Bounds b);

LongDbl bounds_h(Bounds b);

Bounds shift_bounds(Bounds b, LongDbl x_off, LongDbl y_off);

Bounds shift_bounds_by_prop(Bounds b, double x_prop, double y_prop);

/* Center x position. */
LongDbl bounds_xc(Bounds b);

/* Center y position. */
LongDbl bounds_yc(Bounds b);

/* If center is NULL, keep current center. */
Bounds scale_and_center(Bounds b, LongDbl scale, Point *center);

/* Scale a Point within src to a point in dest. */
Point scale_pt(Point p, Bounds src, Bounds dest);

/* Return Bounds enclosing min_bounds that has the same aspect ratio as
 * screen_bounds.
 */
Bounds norm_aspect(Bounds min_bounds, Bounds screen_bounds);

/* Return source Bounds clipped to be entirely inside dest. If they don't
 * overlap, Bound (0, 0, 0, 0) is returned */
Bounds clip_bounds(Bounds src, Bounds dest);
