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
#include <stdio.h>
#include <stdbool.h>

#include "linkedlist.h"
#include "utils.h"

typedef struct ll_node {
    void *value;
    struct ll_node *next;
    struct ll_node *prev;
} LLNode;

/* ALWAYS CALL THIS before doing any comparisons using index, as unsigned/signed
 * comparisons with negative numbers are a bitch.
 */
static unsigned _ll_get_real_ind(long i, unsigned len) {
    if (len == 0) {
        return 0;
    }

    if (i < 0) {
        i = i % len;
        if (i < 0) {
            i += len;
        }
    }
    return i;
}

/* Return the node at index i. May be NULL if i is too large. */
static LLNode *_ll_node_at(LinkedList *list, long i) {
    unsigned new_i = _ll_get_real_ind(i, list->len);
    if (list->len && new_i == list->len - 1) {
        return list->last;
    } else if (new_i >= list->len) {
        return NULL;
    }

    unsigned j;
    if (new_i > list->len / 2) {
        LLNode *node = list->last;
        for (j = list->len - 1; j > new_i; j--) {
            node = node->prev;
            if (node == NULL) {
                return NULL;
            }
        }
        return node;
    } else {
        LLNode *node = list->first;
        for (j = 0; j < new_i; j++) {
            node = node->next;
            if (node == NULL) {
                return NULL;
            }
        }
        return node;
    }
}

//void ll_merge(LinkedList *l1, LinkedList *l2) {
//    if (l1->len == 0) {
//        ll_free(l1);
//        l1 = l2;
//        l2 = NULL;
//        return;
//    } else if (l2->len == 0) {
//        ll_free(l2);
//        return;
//    }
//
//    l1->last->next = l2->first;
//    l2->first->prev = l1->last;
//    l1->last = l2->last;
//    l1->len += l2->len;
//
//    free_n(&l2);
//}
//
//void ll_concat_ip(LinkedList *l1, LinkedList *l2) {
//    if (l2->len == 0) {
//        return;
//    }
//
//    LinkedList *new_list = ll_create_from_list(l2);
//
//    if (l1->len == 0) {
//        ll_free(l1);
//        l1 = new_list;
//        return;
//    }
//
//    l1->last->next = new_list->first;
//    new_list->first->prev = l1->last;
//    l1->last = new_list->last;
//    l1->len += new_list->len;
//    free(new_list);
//}
//
//LinkedList *ll_concat(LinkedList *l1, LinkedList *l2) {
//    LinkedList *new = ll_create_from_list(l1);
//    ll_concat_ip(new, l2);
//    return new;
//}

/* Remove and return element at index i, or NULL if i is too large. */
void *ll_pop(LinkedList *list, long i) {
    LLNode *removed = _ll_node_at(list, i);
    if (removed == NULL) {
        return NULL;
    }
    LLNode *prev = removed->prev;
    LLNode *next = removed->next;
    if (prev == NULL) {
        /* first element or only element */
        if (next == NULL) {
            list->first = NULL;
            list->last = NULL;
        } else {
            list->first = next;
            next->prev = NULL;
        }
    } else if (next == NULL) {
        /* last element, with at least one other one */
        list->last = prev;
        prev->next = NULL;
    }

    list->len -= 1;

    void *value = removed->value;
    free(removed);
    return value;
}

/* Append data to the list, and return the value passed in. */
void *ll_append(LinkedList *list, void *value) {
    LLNode *newNode = malloc(sizeof(LLNode));
    if (newNode == NULL) {
        return NULL;
    }

    newNode->value = value;
    newNode->next = NULL;
    newNode->prev = list->last;

    if (list->len == 0) {
        list->first = newNode;
        list->last = newNode;
    }
    else {
        list->last->next = newNode;
        list->last = newNode;
    }
    list->len++;
    return value;
}

/* Create and return an empty list. */
LinkedList *ll_create_empty(void) {
    LinkedList *list = malloc(sizeof(LinkedList));
    if (list == NULL) {
        return NULL;
    }

    list->first = NULL;
    list->last = NULL;
    list->len = 0;
    return list;
}

/* Return a new list made from the elements with indices above and including i1
 * but below i2.
 */
LinkedList *ll_slice(LinkedList *list, long i1, long i2) {
    i1 = _ll_get_real_ind(i1, list->len);
    i2 = _ll_get_real_ind(i2, list->len);
    if (i1 >= i2 || i1 >= list->len) {
        /* return empty list */
        return ll_create_empty();
    }
    if (i2 > list->len) {
        i2 = list->len;
    }

    LinkedList *new_list = ll_create_empty();

    LLNode *node = _ll_node_at(list, i1);
    for (unsigned i = 0; i < i2 - i1; i++) {
        ll_append(new_list, node->value);
        node = node->next;
    }
    return new_list;
}

/* Create a new linked list with the same elements as another. */
LinkedList *ll_create_from_list(LinkedList *list) {
    LinkedList *new_list = ll_create_empty();
    LLNode *node = list->first;

    for (unsigned i = 0; i < list->len; i++) {
        ll_append(new_list, node->value);
        node = node->next;
    }
    return new_list;
}

/* Return the value at index i or NULL if index is invalid. */
void *ll_at(LinkedList *list, long i) {
    LLNode *node = _ll_node_at(list, i);
    return (node == NULL) ? NULL : node->value;
}

/* Create a linked list from an array, pointing to the elements in the array. */
LinkedList *ll_create_from_array(void *arr, unsigned len, size_t type_size) {
    unsigned char *arr_head = arr; /* so that pointer arithmetic is defined */
    LinkedList *list = ll_create_empty();
    for (unsigned i = 0; i < len; i++) {
        ll_append(list, arr_head + i * type_size);
    }
    return list;
}

/* Print the linked list in this format: {<val1>, <val2>, ...}.
 *
 * *print_val is a function that should take a value in the list and print it.
 */
void ll_print(LinkedList *list, void (*print_val)(void *)) {
    printf("{");
    LLNode *node = list->first;
    unsigned iters = list->len == 0 ? 0 : list->len - 1;
    for (unsigned i = 0; i < iters; i++) {
        (*print_val)(node->value);
        node = node->next;
        printf(", ");
    }
    if (list->len) {
        (*print_val)(node->value);
    }
    printf("}\n");
}

/* Deallocate the memory taken up by the list. */
void ll_free(LinkedList *list) {
    LLNode *node = list->first;
    LLNode *next = NULL;
    for (unsigned i = 0; i < list->len; i++) {
        next = node->next;
        free(node); /* like the IRC network */
        node = next;
    }
    free(list);
}

/* Call function on each value in list, without modifying the list.
 *
 * This is useful, for example, for free()ing all elements quickly.
 */
void ll_apply(LinkedList *list, void (*func)(void *)) {
    LLNode *node = list->first;
    for (unsigned i = 0; i < list->len; i++) {
        (*func)(node->value);
        node = node->next;
    }
}

/* Return the first element in list for which test(elem) returns true, or NULL
 * if none do.
 */
void *ll_filter_first(LinkedList *list, bool (*test)(void *)) {
    LLNode *node = list->first;
    for (unsigned i = 0; i < list->len; i++) {
        if ((*test)(node->value)) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}
