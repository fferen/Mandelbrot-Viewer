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
