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

#include <stdlib.h>

#include "utils.h"
#include "zoomstack.h"
#include "linkedlist.h"

ZoomStack *zs_create_stack(void) {
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

void zs_free_state(ZoomState *state) {
    if (state->in_set != NULL) {
        free(state->in_set);
    }
}

void zs_free_stack(ZoomStack *stack) {
    unsigned int i;
    for (i = 0; i < stack->len; i++) {
        zs_free_state(ll_at(stack, 0));
    }
    ll_free(stack);
}
