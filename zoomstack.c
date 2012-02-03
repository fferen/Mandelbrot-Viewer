#include <stdlib.h>

#include "utils.h"
#include "zoomstack.h"
#include "linkedlist.h"

ZoomStack *zs_create(void) {
    return ll_create_empty();
}

ZoomState zs_create_state(Bounds bnds, int max_iters, int surf_w, int surf_h) {
    ZoomState state = {bnds, max_iters, malloc(surf_w * surf_h * sizeof(bool)), 0};
    return state;
}

void zs_push(ZoomStack *stack, ZoomState state, int in_set_size) {
    state.in_set = cp_to_heap(state.in_set, in_set_size);
    ZoomState *heap_state = cp_to_heap(&state, sizeof(ZoomState));
    ll_append(stack, heap_state);
}

void zs_pop(ZoomStack *stack, ZoomState *dest) {
    if (stack->len == 0) {
        return;
    }

    ZoomState *state = ll_pop(stack, -1);
    if (dest != NULL) {
        if (dest->in_set != NULL) {
            free(dest->in_set);
        }
        *dest = *state;
    } else {
        free(state->in_set);
    }
    free(state);
}

ZoomState *zs_at(ZoomStack *stack, int i) {
    return ll_at(stack, i);
}

ZoomState *zs_top(ZoomStack *stack) {
    return ll_at(stack, -1);
}
