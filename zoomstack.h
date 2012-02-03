#pragma once

#include <stdbool.h>

#include "geometry.h"
#include "linkedlist.h"

typedef struct _ZoomState {
    Bounds bnds;
    int max_iters;
    bool *in_set;
    float prop_in_set;
} ZoomState;

typedef LinkedList ZoomStack;

ZoomStack *zs_create(void);
ZoomState zs_create_state(Bounds bnds, int max_iters, int surf_w, int surf_h);

void zs_push(ZoomStack *stack, ZoomState state, int in_set_size);
void zs_pop(ZoomStack *stack, ZoomState *dest);
ZoomState *zs_at(ZoomStack *stack, int i);
ZoomState *zs_top(ZoomStack *stack);
