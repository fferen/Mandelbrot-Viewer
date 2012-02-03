#include <stdio.h>
#include <math.h>
#include <stdbool.h>

#include <SDL/SDL.h>

#include "zoomstack.h"
#include "utils.h"

#include "sdlutils.h"
#include "colorutils.h"
#include "geometry.h"

#define WIDTH 640
#define HEIGHT 480
#define DEPTH 32

#define START_ITERS 1000
#define ZOOM_SCALE 0.2
#define SHIFT_PROP 0.1
#define MAX_ITER_CHG 0.2

/* flags for testing if point was in previous state */
#define PT_IN_SET 0
#define PT_NOT_IN_SET 1
#define PT_OUT_OF_RANGE 2

/* Return the number of iterations it takes for a number to escape, or -1 if it
 * is exceeds max and is considered to be in the set.
 */
float esc_iters(LongDbl real, LongDbl imag, int max) {
    register LongDbl real_temp;
    register LongDbl c_real = real;
    register LongDbl c_imag = imag;
    register int iters;

    for (iters = 0; real * real + imag * imag < 9 && iters < max; iters++) {
        real_temp = real * real - imag * imag + c_real;
        imag = 2 * imag * real + c_imag;
        real = real_temp;
    }
    if (iters == max) {
        return -1;
    } else {
        return iters + 1 - log(log(sqrt(real * real + imag * imag))) / log(2);
    }
}

/* Return one of: PT_IN_SET, PT_NOT_IN_SET, or PT_OUT_OF_RANGE. */
char pt_in_state(Point set_pt, ZoomState *state, Bounds surf_bnds, SDL_Surface *surf) {
    if (state == NULL) {
        return PT_OUT_OF_RANGE;
    }

    Point prev_screen_pt = scale_pt(set_pt, state->bnds, surf_bnds);
    prev_screen_pt.y = surf->h - prev_screen_pt.y;

    int low_x = floor(prev_screen_pt.x);
    int low_y = floor(prev_screen_pt.y);

    if (low_x < 0 || low_x + 1 >= surf->w || low_y < 0 || low_y + 1 >= surf->h) {
        return PT_OUT_OF_RANGE;
    }

    int i, j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            if (!state->in_set[(low_y + i) * surf->w + (low_x + j)]) {
                return PT_NOT_IN_SET;
            }
        }
    }
    return PT_IN_SET;
}

/* Draw the mandelbrot set bounded by <state.bnds>, clipped by <clip_rect>,
 * using iters_to_color() to get the color.
 *
 * If <clip_rect> is NULL, use the whole surface and update the ZoomState with
 * prop_in_set and in_set.
 */
void draw_mandelbrot(
        SDL_Surface *surf,
        ZoomState *state,
        SDL_Rect *clip_rect,
        RGBColor (*iters_to_color)(float iters, int max_iters),
        ZoomStack *hist
        ) {
    int i;
    LongDbl x_off = (state->bnds.x_max - state->bnds.x_min) / surf->w;
    LongDbl y_off = (state->bnds.y_max - state->bnds.y_min) / surf->h;
    int p_x, p_y; // pixel x, y
    LongDbl x, y; // real x, y
    float iters;
    int pixels_in_set = 0;

    const Bounds surf_bnds = {0, surf->w, 0, surf->h};
    SDL_Rect clip = bounds_to_rect(surf_bnds);
    if (clip_rect != NULL) {
        clip = *clip_rect;
    }

    if (SDL_MUSTLOCK(surf) && SDL_LockSurface(surf) < 0) {
        return;
    }

    for (p_x = clip.x; p_x < clip.x + clip.w; p_x++) {
        x = state->bnds.x_min + p_x * x_off;
        for (p_y = clip.y; p_y < clip.y + clip.h; p_y++) {
            y = state->bnds.y_max - p_y * y_off;

            char flag = PT_NOT_IN_SET;
            for (i = hist->len - 1; i >= 0; i--) {
                flag = pt_in_state((Point) {x, y}, zs_at(hist, i), surf_bnds, surf);
                if (flag != PT_OUT_OF_RANGE) {
                    break;
                }
            } 
            if (flag == PT_IN_SET) {
                iters = -1;
            } else {
                iters = esc_iters(x, y, state->max_iters);
            }

            if (clip_rect == NULL) {
                if (iters == -1) {
                    pixels_in_set++;
                    state->in_set[surf->w * p_y + p_x] = true;
                } else {
                    state->in_set[surf->w * p_y + p_x] = false;
                }
            }
            set_pixel(surf, p_x, p_y, (*iters_to_color)(iters, state->max_iters));
        }
    }

    if (SDL_MUSTLOCK(surf)) {
        SDL_UnlockSurface(surf);
    }

    if (clip_rect == NULL) {
        state->prop_in_set = (float) pixels_in_set / (clip.w * clip.h);
    }
}

RGBColor iters_to_color(float iters, int max_iters) {
    if (iters == -1) {
        return (RGBColor) {0, 0, 0};
    } else {
        /* between 0 and 1 */
//        float norm_iters = fmod(iters, 1);
//        if (norm_iters < 0) {
//            norm_iters++;
//        }
//        float norm_iters = iters / max_iters;
        float norm_iters = exp(-10 * (float) iters / max_iters);
        float h = (1 - norm_iters) * 6;
        float s = norm_iters;
        float v = norm_iters;
        return hsv_to_rgb(h, s, v);
    }
}

int main(int argc, char *argv[]) {
    SDL_Surface *screen = sdl_init_all(
            0,
            0,
            SDL_INIT_VIDEO,
            SDL_FULLSCREEN | SDL_SWSURFACE | SDL_ANYFORMAT
            );
    const Bounds screen_bounds = {0, screen->w, 0, screen->h};

    ZoomStack *hist = zs_create(); /* a LIFO stack of previous states */
    ZoomState cur_state = zs_create_state(
        norm_aspect((Bounds) {-2, 0.5, -1.2, 1.21}, screen_bounds),
        START_ITERS,
        screen->w,
        screen->h
    );

    bool done = false;
    bool just_moved = false;
    SDL_Event evt;
    double move_x_prop, move_y_prop;

    void redraw(SDL_Rect *clip_rect) {
        draw_mandelbrot(screen, &cur_state, clip_rect, &iters_to_color, hist);
        SDL_Flip(screen);
    }

    void hist_push() {
        if (!just_moved) {
            zs_push(hist, cur_state, screen->w * screen->h);
        }
        just_moved = false;
    }

    redraw(NULL);

    while (!done) {
        while (SDL_PollEvent(&evt)) {
            switch (evt.type) {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    move_x_prop = move_y_prop = 0;
                    switch (evt.key.keysym.sym) {
                        case SDLK_q:
                            done = true;
                            break;
                        case SDLK_UP:
                            move_x_prop = 0;
                            move_y_prop = SHIFT_PROP;
                            break;
                        case SDLK_DOWN:
                            move_x_prop = 0;
                            move_y_prop = -SHIFT_PROP;
                            break;
                        case SDLK_RIGHT:
                            move_x_prop = SHIFT_PROP;
                            move_y_prop = 0;
                            break;
                        case SDLK_LEFT:
                            move_x_prop = -SHIFT_PROP;
                            move_y_prop = 0;
                            break;
                        case SDLK_EQUALS:
                            hist_push();
                            cur_state.max_iters *= 1 + MAX_ITER_CHG;
                            redraw(NULL);
                            break;
                        case SDLK_MINUS:
                            hist_push();
                            cur_state.max_iters *= 1 - MAX_ITER_CHG;
                            redraw(NULL);
                            break;
                    }
                    if (move_x_prop || move_y_prop) {
                        int start_x, start_y;
                        SDL_Rect crop_old, crop_new;

                        hist_push();
                        cur_state.bnds = shift_bounds_by_prop(cur_state.bnds, move_x_prop, move_y_prop);
                        crop_old = bounds_to_rect(shift_bounds_by_prop(screen_bounds, -move_x_prop, move_y_prop));
                        switch (evt.key.keysym.sym) {
                            case SDLK_UP:
                                crop_new = (SDL_Rect) {0, 0, screen->w, crop_old.y};
                                break;
                            case SDLK_DOWN:
                                start_y = crop_old.y + crop_old.h;
                                crop_new = (SDL_Rect) {0, start_y, screen->w, screen->h - start_y};
                                break;
                            case SDLK_LEFT:
                                crop_new = (SDL_Rect) {0, 0, crop_old.x, screen->h};
                                break;
                            case SDLK_RIGHT:
                                start_x = crop_old.x + crop_old.w;
                                crop_new = (SDL_Rect) {start_x, 0, screen->w - start_x, screen->h};
                                break;
                        }
//                        printf("%d %d %d %d\n", crop_old.x, crop_old.y, crop_old.w, crop_old.h);
//                        printf("%d %d %d %d\n", crop_new.x, crop_new.y, crop_new.w, crop_new.h);
                        SDL_BlitSurface(screen, NULL, screen, &crop_old);
                        redraw(&crop_new);
                        just_moved = true;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    switch (evt.button.button) {
                        case SDL_BUTTON_LEFT:
                            ; // case statements can't be followed by declarations. WTF
                            Point zoom_center = scale_pt(
                                    (Point) {evt.button.x, screen->h - evt.button.y},
                                    screen_bounds,
                                    cur_state.bnds
                                    ); // the center set point when zooming

                            hist_push();

                            printf("Zoom center: ");
                            print_pt(zoom_center);

                            printf("Old bounds: ");
                            print_bounds(cur_state.bnds);

                            cur_state.bnds = scale_and_center(cur_state.bnds, ZOOM_SCALE, &zoom_center);
                            printf("New bounds: ");
                            print_bounds(cur_state.bnds);

                            cur_state.max_iters = (int)(cur_state.max_iters * (1 + 0.5 * cur_state.prop_in_set));
                            redraw(NULL);
                            printf("Prop in set: %f\n", cur_state.prop_in_set);
                            break;
                        case SDL_BUTTON_RIGHT:
                            if (hist->len > 0) {
                                zs_pop(hist, &cur_state);
                                redraw(NULL);
                            }
                            break;
                    }
                    break;
            }
        }
        SDL_Delay(1);
    }
    return 0;
}
