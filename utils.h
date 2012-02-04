#pragma once

/* Don't use this, obviously. Just practicing writing algorithms. */
void quicksort(int *l, int size);

/* Print an array of ints: {<i1>, <i2>, ...} */
void print_i_array(int *l, int size);

/* Free memory and set ptr to NULL. */
void free_n(void *ptr);

/* Allocate memory on the heap and copy data at ptr to it. Return pointer to
 * allocated memory.
 */
void *cp_to_heap(void *ptr, size_t size);
