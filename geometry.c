#include <stdio.h>

#include "geometry.h"

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

void print_pt(Point p) {
    printf("{%Lf, %Lf}\n", p.x, p.y);
}

void print_bounds(Bounds b) {
    printf("{%Lf, %Lf, %Lf, %Lf}\n", b.x_min, b.x_max, b.y_min, b.y_max);
}

LongDbl bounds_w(Bounds b) {
    return b.x_max - b.x_min;
}

LongDbl bounds_h(Bounds b) {
    return b.y_max - b.y_min;
}

Bounds shift_bounds(Bounds b, LongDbl x_off, LongDbl y_off) {
    Bounds new = {b.x_min + x_off, b.x_max + x_off, b.y_min + y_off, b.y_max + y_off};
    return new;
}

Bounds shift_bounds_by_prop(Bounds b, double x_prop, double y_prop) {
    return shift_bounds(b, bounds_w(b) * x_prop, bounds_h(b) * y_prop);
}

/* Center x position. */
LongDbl bounds_xc(Bounds b) {
    return (b.x_min + b.x_max) / 2;
}

/* Center y position. */
LongDbl bounds_yc(Bounds b) {
    return (b.y_min + b.y_max) / 2;
}

/* If center is NULL, keep current center. */
Bounds scale_and_center(Bounds b, LongDbl scale, Point *center) {
    LongDbl half_w = bounds_w(b) / 2;
    LongDbl half_h = bounds_h(b) / 2;
    Point center_p;

    if (center == NULL) {
        center_p = (Point) {half_w + b.x_min, half_h + b.y_min};
    } else {
        center_p = *center;
    }

    Bounds new = {
        center->x - half_w * scale,
        center->x + half_w * scale,
        center->y - half_h * scale,
        center->y + half_h * scale
    };
    return new;
}

/* Scale a Point within src to a point in dest. */
Point scale_pt(Point p, Bounds src, Bounds dest) {
    Point new = {
        (p.x - src.x_min) / bounds_w(src) * bounds_w(dest) + dest.x_min,
        (p.y - src.y_min) / bounds_h(src) * bounds_h(dest) + dest.y_min
    };
    return new;
}

/* Return Bounds enclosing min_bounds that has the same aspect ratio as
 * screen_bounds.
 */
Bounds norm_aspect(Bounds min_bounds, Bounds screen_bounds) {
    LongDbl units_per_pixel_x = bounds_w(min_bounds) / bounds_w(screen_bounds);
    LongDbl units_per_pixel_y = bounds_h(min_bounds) / bounds_h(screen_bounds);

    if (units_per_pixel_x < units_per_pixel_y) {
        LongDbl center_x = bounds_xc(min_bounds);
        LongDbl half_screen_w = bounds_w(screen_bounds) / 2;
        LongDbl x_min = center_x - half_screen_w * units_per_pixel_y;
        LongDbl x_max = center_x + half_screen_w * units_per_pixel_y;
        Bounds new = {x_min, x_max, min_bounds.y_min, min_bounds.y_max};
        return new;
    } else {
        LongDbl center_y = bounds_yc(min_bounds);
        LongDbl half_screen_h = bounds_h(screen_bounds) / 2;
        LongDbl y_min = center_y - half_screen_h * units_per_pixel_x;
        LongDbl y_max = center_y + half_screen_h * units_per_pixel_x;
        Bounds new = {min_bounds.x_min, min_bounds.x_max, y_min, y_max};
        return new;
    }
}

Bounds clip_bounds(Bounds src, Bounds dest) {
    Bounds new = {
            max(src.x_min, dest.x_min),
            min(src.x_max, dest.x_max),
            max(src.y_min, dest.y_min),
            min(src.y_max, dest.y_max)
    };
    if (new.x_min > new.x_max || new.y_min > new.y_max) {
        new.x_min = new.x_max = new.y_min = new.y_max = 0;
    }
    return new;
}
