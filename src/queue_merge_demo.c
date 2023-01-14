/*
BSD 3-Clause License

Copyright (c) 2023, KriztoferY (https://github.com/KriztoferY)
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
 * @file queue_merge_demo.c
 * @version 0.1.0
 * @copyright Copyright (c) 2023 KriztoferY. All rights reserved.
 *
 * @brief Demo program for using a generic algorithm on the Queue ADT.
 *
 * Concretely, it takes circular array queue as example to show how to use the
 * `merge_queues()` function to stable merges two non-empty queues.
 */

#include <stdlib.h>   // EXIT_*
#include <stdio.h>    // printf()
#include <assert.h>   // assert()

#include "queue.h"   // Queue, Queue_*()
#include "algos.h"   // merge_queues()

bool greater(void const* a, void const* b) { return *(int*)a > *(int*)b; }

void print_int(void const* a) { printf("%d", *(int*)a); }

int main() {
    // Create a queue
    Queue*       q1      = Queue_create(sizeof(int));
    int          nums1[] = { 4, 7, 2, 10 };   // num = priority
    size_t const n1      = sizeof(nums1) / sizeof(int);
    bool         res;
    for (size_t i = 0; i < n1; ++i) {
        res = Queue_enqueue(q1, &nums1[i]);
        assert(res && "cannot allocate memory to complete enqueue()");
    }
    printf("%s", "q1 : ");
    Queue_print(q1, ",", false, print_int);
    printf("\n");

    // Create another queue
    Queue*       q2      = Queue_create(sizeof(int));
    int          nums2[] = { 3, 6, 8, 9, 5, 1 };   // num = priority
    size_t const n2      = sizeof(nums2) / sizeof(int);
    for (size_t i = 0; i < n2; ++i) {
        res = Queue_enqueue(q2, &nums2[i]);
        assert(res && "cannot allocate memory to complete enqueue()");
    }
    printf("%s", "q2 : ");
    Queue_print(q2, ",", false, print_int);
    printf("\n");

    // Stable-merge: larger the element value, higher the priority
    printf("%s\n", "merging q1 and q2...");
    Queue* q = merge_queues(q1, q2, sizeof(int), greater);

    printf("%s", "q  : ");
    Queue_print(q, ",", false, print_int);
    printf("\n");

    // Deallocate memory on the heap
    Queue_destroy(q1);
    Queue_destroy(q2);
    Queue_destroy(q);

    return EXIT_FAILURE;
}

// -----------------------------------------------------------------------------

// clang-format off

/* === COMPILE & RUN ===
gcc queue_merge_demo.c algos.c queue_circ_array.c -std=c99 -g -Og -Wall -pedantic -march=native -DQUEUE_INIT_CAP=10 && ./a.out
./a.out gcc queue_merge_demo.c algos.c queue_circ_array.c -std=c99 -O3 -march=native -DNDEBUG -DQUEUE_INIT_CAP=10 && ./a.out
*/

/* === OUTPUT ===
q1 : 4,7,2,10
q2 : 3,6,8,9,5,1
merging q1 and q2...
q  : 4,7,3,6,8,9,5,2,10,1
*/