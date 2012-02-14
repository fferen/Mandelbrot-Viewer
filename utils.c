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
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.'
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void quicksort(int *l, int size) {
    if (size <= 1) {
        return;
    }

    int pivot = l[size / 2];
    int i = 0;
    int j = size - 1;
    int temp;

    do {
        while (l[i] < pivot) {
            i++;
        }
        while (l[j] > pivot) {
            j--;
        }
        temp = l[i];
        l[i] = l[j];
        l[j] = temp;
    } while (l[i] != l[j]);

    quicksort(l, j);
    quicksort(l + j, size - j);
}

void print_i_array(int *l, int size) {
    int i;
    printf("{");
    for (i = 0; i < size - 1; i++) {
        printf("%d, ", *(l + i));
    }
    printf("%d}\n", l[size - 1]);
}

void free_n(void *ptr) {
    free(ptr);
    ptr = NULL;
}

void *cp_to_heap(void *ptr, size_t size) {
    void *dest = malloc(size);
    if (dest == NULL) {
        return NULL;
    }

    return memcpy(dest, ptr, size);
}
