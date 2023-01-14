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
 * @brief Implementation of the Queue ADT as an unbounded queue using a singly
 * linked list.
 *
 * Each element in the queue is conceptually a node, but logically a contiguous
 * memory block in which the first `sizeof(void*)` bytes (i.e. 8 bytes in
 * 64-bit architecture) stores the address of its succeeding element in the
 * queue, followed by the value of the element. As a result, the queue elements
 * have **value semantics**.
 */

#include "queue.h"

#include <stddef.h>   // size_t
#include <stdlib.h>   // malloc(), free()
#include <string.h>   // memcpy()
#include <limits.h>   // ULONG_MAX
#include <assert.h>   // assert()
#include <stdio.h>    // printf()

struct queue
{
    size_t elemsz;   // Size of each element in bytes
    size_t nelems;   // Number of elements in the queue
    void*  front;    // Element at the front of the queue
    void*  back;     // Element at the end of the queue
};

Queue* Queue_create(size_t elem_sz) {
    Queue* q = malloc(sizeof(Queue));
    if (q == NULL) return NULL;

    q->elemsz = elem_sz;
    q->nelems = 0;
    q->front  = NULL;
    q->back   = NULL;
    return q;
}

void Queue_destroy(Queue* queue) {
    assert(queue != NULL);

    // Deallocate all nodes
    void* elem = queue->front;
    void* next = NULL;
    while (elem != NULL) {
        next = *(void**)elem;   // get the addr of succeeding element
        free(elem);
        elem = next;
    }

    // Deallocate queue
    free(queue);
}

size_t Queue_capacity(Queue* queue) {
    assert(queue != NULL);

    return ULONG_MAX;
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

    memcpy(elem, (char*)queue->front + sizeof(void*), queue->elemsz);
    return true;
}

bool Queue_enqueue(Queue* queue, void const* elem) {
    assert(queue != NULL);

    void* node = malloc(sizeof(void*) + queue->elemsz);
    if (node == NULL) return false;

    // Set next of new node to null
    *(void**)node = NULL;
    // Copy element data to new node
    memcpy((char*)node + sizeof(void*), elem, queue->elemsz);

    if (queue->back == NULL) {
        assert(queue->front == NULL && "front is not null when queue empty");
        // Set new node as front node of queue
        queue->front = node;
    } else {
        // Point current back node to new node
        *(void**)queue->back = node;
    }

    // Set new node as back node of queue
    queue->back   = node;
    // Increment queue size
    queue->nelems += 1;

    return true;
}

bool Queue_dequeue(Queue* queue) {
    assert(queue != NULL);

    if (queue->nelems == 0) return false;

    void* front     = queue->front;     // current front node
    void* new_front = *(void**)front;   // current front node's successor
    if (new_front == NULL) {
        assert(queue->back == front &&
               "front and back not identitical when queue has 1 element");
        // Nullify queue's back
        queue->back = NULL;
    }

    // Set queue's new front
    queue->front  = new_front;
    // Decrement queue size
    queue->nelems -= 1;
    // Deallocate old front node
    free(front);

    return true;
}

void Queue_print(Queue* queue, char const* sep, bool vertical,
                 void (*print_element)(void const*)) {
    assert(queue != NULL);
    if (sep == NULL) sep = ",";

    size_t const n_elems = queue->nelems;

    void* node           = queue->front;
    if (node == NULL) return;

    void*  elem;
    size_t i = 0;
    do {
        if (vertical) printf("[%lu] ", i);
        elem = (char*)node + sizeof(void*);
        print_element(elem);
        vertical ? printf("\n")
                 : ((i == n_elems - 1) ? printf("%s", "") : printf("%s", sep));
        // Advance to next node
        node = *(void**)node;
        ++i;
    } while (node);
}