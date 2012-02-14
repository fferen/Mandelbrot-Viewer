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

/* Deallocate the memory taken up by the list. */
void ll_free(LinkedList *list);

/*** ADDING ELEMENTS ***/

///* Concatenate l1 with l2 and return the new list. */
//LinkedList *ll_concat(LinkedList *l1, LinkedList *l2);

/* Append data to the list, and return a pointer to the stored value. */
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
