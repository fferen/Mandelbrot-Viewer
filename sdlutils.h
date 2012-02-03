#pragma once

#include <SDL/SDL.h>
#include <stdbool.h>

#include "geometry.h"
#include "colorutils.h"

/* Initialize SDL and set video mode. Return screen surface. Print error and
 * exit program if init fails.
 */
SDL_Surface *sdl_init_all(int w, int h, Uint32 init_flags, Uint32 video_flags);

/* Conversions */
Bounds rect_to_bounds(SDL_Rect r);
SDL_Rect bounds_to_rect(Bounds b);

/* Set coordinates. If resize is true, don't affect the other coordinates, else
 * simply move the rect.
 */
SDL_Rect rect_set_top(SDL_Rect r, Sint16 coord, bool resize);
SDL_Rect rect_set_bottom(SDL_Rect r, Sint16 coord, bool resize);
SDL_Rect rect_set_left(SDL_Rect r, Sint16 coord, bool resize);
SDL_Rect rect_set_right(SDL_Rect r, Sint16 coord, bool resize);

/* Return an RGBColor from a pixel in given format. */
RGBColor pixel_to_color(Uint32 pixel, SDL_PixelFormat *fmt);

/* Get/set pixels. NOTE: The surface must be locked before calling this!
 *
 * From: http://www.libsdl.org/docs/html/guidevideo.html
 */
RGBColor get_pixel(SDL_Surface *surface, int x, int y);
void set_pixel(SDL_Surface *surface, int x, int y, RGBColor color);
