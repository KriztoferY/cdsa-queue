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
 * @file queue_demo.c
 * @version 0.1.0
 * @copyright Copyright (c) 2022 KriztoferY. All rights reserved.
 *
 * @brief Demo program for using queue.
 */

#include <stdlib.h>   // EXIT_*
#include <stdio.h>    // printf()
#include <assert.h>   // assert()

#include "queue.h"   // Queue, Queue_*()

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void print_usage(char* executable) {
    printf("USAGE: %s <num_elems(<=16)>\n", executable);
    exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
    if (argc != 2) print_usage(argv[0]);

    char*  cend      = NULL;
    size_t num_elems = strtoull(argv[1], &cend, 10);

    if (num_elems > 16) {
        printf("%s\n", "ERROR: program argument 1 (num_elems) exceeds 16");
        exit(EXIT_FAILURE);
    }

    double x = M_PI;

    Queue* q = Queue_create(sizeof(int));

    printf("Queuing up the first %lu significant digits of pi...\n\n",
           num_elems);

    printf("queue (q) created :: size: %lu | cap: %lu \n\n", Queue_size(q),
           Queue_capacity(q));

    printf("%s\n", "Attempt to peek front element of empty queue...");
    int  elem;
    bool res = Queue_front(q, &elem);
    printf("Queue_front(q) returns `%s`\n", res ? "true" : "false");

    int front_elem;
    for (size_t i = 0; i < num_elems; ++i) {
        int elem = (size_t)x % 10;
        x        *= 10;

        res      = Queue_enqueue(q, &elem);
        assert(res && "cannot allocate memory to filful enqueue()");
        res = Queue_front(q, &front_elem);
        assert(res && "Queue_front() failed when queue not empty");
        printf("Queue_enqueue(q, %d) :: front: %d | size: %lu | cap: %lu\n",
               elem, front_elem, Queue_size(q), Queue_capacity(q));
    }
    printf("%s\n", "");

    while (!Queue_empty(q)) {
        res = Queue_front(q, &front_elem);
        assert(res && "Queue_front() failed when queue not empty");

        printf("front: %d | size: %lu | cap: %lu -- Queue_dequeue(q)\n",
               front_elem, Queue_size(q), Queue_capacity(q));
        res = Queue_dequeue(q);
        assert(res && "Queue_dequeue() failed when queue not empty");
    }
    printf("\nsize: %lu | cap: %lu\n", Queue_size(q), Queue_capacity(q));

    printf("%s\n", "Attempt to dequeue from empty queue...");
    res = Queue_dequeue(q);
    assert(!res && "Queue_dequeue() expected to fail when queue empty");
    printf("Queue_dequeue(q) returns `%s`\n", res ? "true" : "false");

    printf("%s\n", "Don't forget to destroy queue when done...");
    Queue_destroy(q);
    printf("%s\n", "Queue_destroy(q) DONE!");

    return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------

/* === COMPILE & RUN ===
gcc queue_circ_array.c queue_demo.c -o queue_circ_array_demo -std=c99 -g -Og
-Wall -pedantic -march=native && ./queue_demo 8 -DQUEUE_INIT_CAP=2

gcc queue_circ_array.c queue_demo.c -o queue_circ_array_demo -std=c99 -O3
-march=native -DNDEBUG && ./queue_demo 8 -DQUEUE_INIT_CAP=2
*/

/* === OUTPUT ===
Queuing up the first 8 significant digits of pi...

queue (q) created :: size: 0 | cap: 2

Attempt to peek front element of empty queue...
Queue_front(q) returns `false`
Queue_enqueue(q, 3) :: front: 3 | size: 1 | cap: 2
Queue_enqueue(q, 1) :: front: 3 | size: 2 | cap: 2
Queue_enqueue(q, 4) :: front: 3 | size: 3 | cap: 4
Queue_enqueue(q, 1) :: front: 3 | size: 4 | cap: 4
Queue_enqueue(q, 5) :: front: 3 | size: 5 | cap: 8
Queue_enqueue(q, 9) :: front: 3 | size: 6 | cap: 8
Queue_enqueue(q, 2) :: front: 3 | size: 7 | cap: 8
Queue_enqueue(q, 6) :: front: 3 | size: 8 | cap: 8

front: 3 | size: 8 | cap: 8 -- Queue_dequeue(q)
front: 1 | size: 7 | cap: 8 -- Queue_dequeue(q)
front: 4 | size: 6 | cap: 8 -- Queue_dequeue(q)
front: 1 | size: 5 | cap: 8 -- Queue_dequeue(q)
front: 5 | size: 4 | cap: 8 -- Queue_dequeue(q)
front: 9 | size: 3 | cap: 8 -- Queue_dequeue(q)
front: 2 | size: 2 | cap: 8 -- Queue_dequeue(q)
front: 6 | size: 1 | cap: 4 -- Queue_dequeue(q)

size: 0 | cap: 4
Attempt to dequeue from empty queue...
Queue_dequeue(q) returns `false`
Don't forget to destroy queue when done...
Queue_destroy(q) DONE!
*/