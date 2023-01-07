/*
BSD 3-Clause License

Copyright (c) 2022, KriztoferY (https://github.com/KriztoferY)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * @brief Implementation of the ADT queue as an unbounded queue using a circular
 * array with dynamic resizing strategy.
 *
 * @note Use the compiler flag `QUEUE_INIT_CAP` and `QUEUE_GROW_FACTOR` to
 *      override the default initial capacity of the underlying array and
 *      default underlying array growth factor respectively.
 */

#include "queue.h"

#include <assert.h>   // assert()
#include <stdlib.h>   // malloc(), free()
#include <string.h>   // memcpy()
#include <stdio.h>    // printf()

// clang-format off
#ifdef QUEUE_INIT_CAP
size_t const INIT_CAP = QUEUE_INIT_CAP; /** Initial underlying array capacity */
#else
size_t const INIT_CAP = 1024; /** Initial underlying array capacity */
#endif

#ifdef QUEUE_GROW_FACTOR
/** Underlying array resizing factor */
unsigned int const grow_factor = QUEUE_GROW_FACTOR; 
#else
unsigned int const grow_factor = 2; /** Underlying array resizing factor */
#endif
// clang-format on

// -----------------------------------------------------------------------------

struct queue
{
    size_t elemsz;   // Element size in bytes.
    size_t nelems;   // Number of elements in the queue.
    size_t cap;      // Maximum number of elements can be stored without malloc.
    size_t start;    // Position of the front element in the underlying array.
    void*  elems;    // Underlying array that stores the queue elements.
};

Queue* Queue_create(size_t elem_sz) {
    // Allocate queue
    Queue* q = malloc(sizeof(Queue));
    if (q == NULL) return NULL;

    // Allocate underlying array
    void* arr = malloc(INIT_CAP * elem_sz);
    if (arr == NULL) return NULL;

    // Initial data members
    q->elems  = arr;
    q->elemsz = elem_sz;
    q->nelems = 0;
    q->cap    = INIT_CAP;
    q->start  = 0;
    return q;
}

void Queue_destroy(Queue* queue) {
    assert(queue != NULL);

    free(queue->elems);
    free(queue);
}

size_t Queue_capacity(Queue* queue) {
    assert(queue != NULL);

    return queue->cap;
}

bool Queue_empty(Queue* queue) {
    assert(queue != NULL);

    return queue->nelems == 0;
}

size_t Queue_size(Queue* queue) {
    assert(queue != NULL);

    return queue->nelems;
}

bool Queue_front(Queue* queue, void* elem) {
    assert(queue != NULL);

    if (queue->nelems == 0) return false;

    memcpy(elem, (char*)queue->elems + (queue->start * queue->elemsz),
           queue->elemsz);
    return true;
}

/**
 * Computes the underlying array position that corresponds to one past the last
 * element in a queue.
 */
static size_t end(Queue* queue) {
    return (queue->start + queue->nelems) % queue->cap;
}

enum resize_dir
{
    SHRINK,
    GROW
}; /** Array resizing direction. */

/** Grows or shrinks the underlying array of a queue. */
static bool resize(Queue* queue, enum resize_dir dir) {
    size_t new_cap = 0;   // new capacity

    if (dir == GROW) {
        new_cap = queue->cap * grow_factor;
    } else if (dir == SHRINK) {
        new_cap = queue->cap / grow_factor;
    }

    assert(new_cap > 0 && "zero new capacity");

    // Allocate new block
    void* arr = malloc(new_cap * queue->elemsz);
    if (arr == NULL) return false;

    // Copy data from old block into new block and free the old
    // CASE 1: no wrapping around
    if (end(queue) > queue->start) {
        void* src = (char*)queue->elems + (queue->start * queue->elemsz);
        memcpy(arr, src, queue->nelems * queue->elemsz);
    }
    // CASE 2: Need to split into two ops -- tail first, then head
    else {
        void* tail = (char*)queue->elems + (queue->start * queue->elemsz);
        size_t const ntail  = queue->cap - queue->start;   // number of elems
        size_t const tailsz = ntail * queue->elemsz;       // number of bytes

        memcpy(arr, tail, tailsz);
        memcpy((char*)arr + tailsz, queue->elems,
               (queue->nelems * queue->elemsz) - tailsz);
    }
    free(queue->elems);

    queue->elems = arr;
    queue->cap   = new_cap;
    queue->start = 0;
    return true;
}

bool Queue_enqueue(Queue* queue, void const* elem) {
    assert(queue != NULL);

    // Grow underlying array if it is full
    if (queue->nelems == queue->cap) {
        if (!resize(queue, GROW)) return false;
    }

    // Copy element data into next available slot in underlying array
    memcpy((char*)queue->elems + (end(queue) * queue->elemsz), elem,
           queue->elemsz);
    queue->nelems += 1;
    return true;
}

bool Queue_dequeue(Queue* queue) {
    assert(queue != NULL);

    if (queue->nelems == 0) return false;

    queue->nelems -= 1;
    queue->start  = (queue->start + 1) % queue->cap;

    // Shrink underlying array if its size falls below a quarter of its full
    // capacity
    if (queue->nelems > 0 && queue->cap / grow_factor >= 2 &&
        queue->nelems * 4 < queue->cap) {
        if (!resize(queue, SHRINK)) return false;
    }

    return true;
}

void Queue_print(Queue* queue, char const* sep, bool vertical,
                 void (*print_element)(void const*)) {
    assert(queue != NULL);
    if (sep == NULL) sep = ",";

    size_t const n_elems = queue->nelems;

    void* elem           = NULL;
    for (size_t i = 0; i < n_elems; ++i) {
        if (vertical) printf("[%lu] ", i);
        elem = (char*)queue->elems +
               (((queue->start + i) % queue->cap) * queue->elemsz);
        print_element(elem);
        vertical
            ? printf("\n")
            : ((i == n_elems - 1) ? printf("%s", "\n") : printf("%s", sep));
    }
}