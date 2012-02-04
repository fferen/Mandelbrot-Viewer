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
