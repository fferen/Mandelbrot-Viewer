#pragma once

/* Define a linked list structure and related operations.
 *
 * Access length with list->len.
 *
 * All functions that take an index argument accept negative indices, which wrap
 * around to the end.
 */

typedef struct ll_node {
    void *value;
    struct ll_node *next;
    struct ll_node *prev;
    size_t size;
} LLNode;

typedef struct linked_list {
    struct ll_node *first;
    struct ll_node *last;
    unsigned int len;
} LinkedList;

/*** CREATION ***/

/* Create and return an empty list. */
LinkedList *ll_create_empty(void);

/* Create a linked list from an array, pointing to the elements in the array. */
LinkedList *ll_create_from_array(void *arr, unsigned int len, size_t type_size);

/* Create a new linked list with the same elements as another. */
LinkedList *ll_create_from_list(LinkedList *list);

/*** DELETION ***/

/* Remove and return element at index i, or NULL if i is too large. */
void *ll_pop(LinkedList *list, int i);

/* Deallocate the memory taken up by the list. Also deallocate the memory
 * used by the values if the size is -1.
 */
void ll_free(LinkedList *list);

/*** ADDING ELEMENTS ***/

///* Concatenate l1 with l2 and return the new list. */
//LinkedList *ll_concat(LinkedList *l1, LinkedList *l2);

/* Append data to the list, and return a pointer to the stored value.
 *
 * <size> is the size of the data. If <size> is -1, simply store the pointer
 * passed in. This will return the same pointer.
 *
 * If not, store a copy of the value instead. This allows you to pass in
 * pointers to data on the stack without pointer invalidation.
 */
void *ll_append(LinkedList *list, void *value);

/*** ACCESSING ELEMENTS ***/

/* Return a new list made from the elements with indices above and including i1
 * but below i2.
 */
LinkedList *ll_slice(LinkedList *list, int i1, int i2);

/* Return the value at index i or NULL if index is invalid. */
void *ll_at(LinkedList *list, int i);

/*** I/O ***/

/* Print the linked list in this format: {<val1>, <val2>, ...}.
 *
 * *print_val is a function that should take a value in the list and print it.
 */
void ll_print(LinkedList *list, void (*print_val)(void *));
