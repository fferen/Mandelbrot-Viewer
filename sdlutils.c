#include <stdlib.h>

#include <SDL/SDL.h>

#include "geometry.h"
#include "sdlutils.h"
#include "colorutils.h"

SDL_Surface *sdl_init_all(int w, int h, Uint32 init_flags, Uint32 video_flags) {
    SDL_Surface *screen;

    if (SDL_Init(init_flags) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    }

    screen = SDL_SetVideoMode(w, h, 0, video_flags);
    if (screen == NULL) {
        printf("SDL_SetVideoMode failed: %s\n", SDL_GetError());
        exit(1);
    }

    return screen;
}

Bounds rect_to_bounds(SDL_Rect r) {
    return (Bounds) {r.x, r.y, r.x + r.w, r.y + r.h};
}

SDL_Rect bounds_to_rect(Bounds b) {
    return (SDL_Rect) {b.x_min, b.y_min, bounds_w(b), bounds_h(b)};
}

SDL_Rect rect_set_top(SDL_Rect r, Sint16 coord, bool resize) {
    if (resize) {
        return (SDL_Rect) {r.x, coord, r.w, r.h + (r.y - coord)};
    } else {
        return (SDL_Rect) {r.x, coord, r.w, r.h};
    }
}

SDL_Rect rect_set_left(SDL_Rect r, Sint16 coord, bool resize) {
    if (resize) {
        return (SDL_Rect) {coord, r.y, r.w + (r.x - coord), r.h};
    } else {
        return (SDL_Rect) {coord, r.y, r.w, r.h};
    }
}

SDL_Rect rect_set_right(SDL_Rect r, Sint16 coord, bool resize) {
    if (resize) {
        return (SDL_Rect) {r.x, r.y, coord - r.x, r.h};
    } else {
        return (SDL_Rect) {coord - r.w, r.y, r.w, r.h};
    }
}

SDL_Rect rect_set_bottom(SDL_Rect r, Sint16 coord, bool resize) {
    if (resize) {
        return (SDL_Rect) {r.x, r.y, r.w, coord - r.h};
    } else {
        return (SDL_Rect) {r.x, coord - r.h, r.w, r.h};
    }
}

RGBColor pixel_to_color(Uint32 pixel, SDL_PixelFormat *fmt) {
    RGBColor color;
    SDL_GetRGB(pixel, fmt, &color.r, &color.g, &color.b);
    return color;
}

RGBColor get_pixel(SDL_Surface *surface, int x, int y) {
    RGBColor color;
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
        case 1:
            return pixel_to_color(*p, surface->format);

        case 2:
            return pixel_to_color(*(Uint16 *)p, surface->format);

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return pixel_to_color(((Uint32)p[0]) << 16 | ((Uint32)p[1]) << 8 | p[2], surface->format);
            else
                return pixel_to_color(p[0] | ((Uint32)p[1]) << 8 | ((Uint32)p[2]) << 16, surface->format);

        case 4:
            return pixel_to_color(*(Uint32 *)p, surface->format);

        default:
            /* shouldn't happen, but avoids warnings */
            return pixel_to_color(0, surface->format);
    }
}

void set_pixel(SDL_Surface *surface, int x, int y, RGBColor color) {
    Uint32 pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}
