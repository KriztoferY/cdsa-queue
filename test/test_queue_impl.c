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

#include <stdlib.h>   // EXIT_*, malloc(), free(), memset()
#include <stdio.h>    // printf(), stderr,
#include <string.h>   // strcmp()
#include <assert.h>   // assert()

#include "test_utils.h"   // UnitTest, run_tests()
#include "queue.h"        // Queue, Queue_*()

static int    NUMS[]      = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5 };
static size_t MAX_N_ELEMS = sizeof(NUMS) / sizeof(int);

void print_int(void const* a) { printf("%d", *(int*)a); }

void handle_error(char const* message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

Queue* create_empty_test_queue(size_t elem_sz) {
    Queue* q = Queue_create(sizeof(int));
    if (q == NULL) {
        handle_error("cannot allocate memory to create a queue");
        Queue_destroy(q);
    }
    return q;
}

Queue* create_prefilled_test_queue(size_t elem_sz, size_t n_elems) {
    if (n_elems > MAX_N_ELEMS)
        handle_error(
            "number of prefilled elements exceeded maximum allowed value");

    Queue* q = Queue_create(sizeof(int));
    if (q == NULL) handle_error("cannot allocate memory to create a queue");

    for (size_t i = 0; i < n_elems; ++i) {
        if (!Queue_enqueue(q, &NUMS[i])) {
            handle_error("cannot allocate memory to enqueue an element");
        };
    }
    return q;
}

void test_create_with_positive_elem_sz() {
    //
    Queue* q = create_empty_test_queue(sizeof(int));

    assert(Queue_size(q) == 0 && "queue size is not zero");
    assert(Queue_capacity(q) > 0 && "queue capacity is zero");

    Queue_destroy(q);
}

void test_create_with_nonpositive_elem_sz() {
    //
    Queue* q = create_empty_test_queue(sizeof(int));

    assert(Queue_size(q) == 0 && "queue size is not zero");
    assert(Queue_capacity(q) > 0 && "queue capacity is zero");

    Queue_destroy(q);
}

void test_front_when_empty() {
    //
    Queue* q         = create_empty_test_queue(sizeof(int));

    void* front_elem = malloc(sizeof(int));
    assert(!Queue_front(q, front_elem) &&
           "Queue_front() returns true when queue is empty");
    free(front_elem);
    Queue_destroy(q);
}

void test_front_when_nonempty() {
    //
    Queue* q         = create_empty_test_queue(sizeof(int));

    void* front_elem = malloc(sizeof(int));
    assert(!Queue_front(q, front_elem) &&
           "Queue_front() returns true when queue is empty");
    free(front_elem);
    Queue_destroy(q);
}

void test_enqueue_when_empty() {
    //
    Queue* q = create_empty_test_queue(sizeof(int));
    if (!Queue_enqueue(q, &NUMS[0])) {
        handle_error("cannot allocate memory to enqueue an element");
    }

    void* front_elem = malloc(sizeof(int));
    if (front_elem == NULL) {
        handle_error("cannot allocate memory block to copy front element");
    }

    assert(Queue_front(q, front_elem) &&
           "Queue_front() returns false when queue is not empty");
    assert(*(int*)front_elem == NUMS[0] &&
           "Queue_front() copies wrong data into out parameter");

    free(front_elem);

    assert(Queue_size(q) == 1 &&
           "Queue_size() returns non-one when queue has one element");
    assert(Queue_capacity(q) > 0 && "queue capacity is zero");

    Queue_destroy(q);
}

void test_enqueue_when_nonempty() {
    //
    void* front_elem = malloc(sizeof(int));
    for (size_t init_sz = 1; init_sz < MAX_N_ELEMS; ++init_sz) {
        Queue* q = create_prefilled_test_queue(sizeof(int), init_sz);
        if (!Queue_enqueue(q, &NUMS[init_sz])) {
            handle_error("cannot allocate memory to enqueue an element");
        }

        if (front_elem == NULL) {
            handle_error("cannot allocate memory block to copy front element");
        }

        assert(Queue_front(q, front_elem) &&
               "Queue_front() returns false when queue is not empty");

        assert(*(int*)front_elem == NUMS[0] &&
               "Queue_front() copies wrong data into out parameter");
        assert(Queue_size(q) == init_sz + 1 &&
               "Queue_size() returns wrong number when queue has at least one "
               "element");
        assert(Queue_capacity(q) > 0 && "queue capacity is zero");

        Queue_destroy(q);
    }

    free(front_elem);
}

void test_dequeue_when_empty() {
    Queue* q = create_empty_test_queue(sizeof(int));
    assert(!Queue_dequeue(q) &&
           "Queue_dequeue() returns true when queue is empty");
    assert(Queue_size(q) == 0 &&
           "Queue_size() returns non-zero when queue is empty");
    assert(Queue_capacity(q) > 0 && "queue capacity is zero");
    Queue_destroy(q);
}

void test_dequeue_when_at_least_two() {
    //
    Queue* q         = create_prefilled_test_queue(sizeof(int), MAX_N_ELEMS);

    void* front_elem = malloc(sizeof(int));
    if (front_elem == NULL) {
        handle_error("cannot allocate memory block to copy front element");
    }

    size_t i = 0;
    size_t begin_sz;
    while ((begin_sz = Queue_size(q)) > 1) {
        if (!Queue_front(q, front_elem)) {
            handle_error("Queue_front() returns false when queue is not empty");
        }
        assert(*(int*)front_elem == NUMS[i++] &&
               "Queue_front() copies wrong data into out parameter");

        if (!Queue_dequeue(q)) {
            handle_error(
                "Queue_dequeue() returns false when queue is not empty");
        }

        if (!Queue_front(q, front_elem)) {
            handle_error("Queue_front() returns false when queue is not empty");
        }
        assert(*(int*)front_elem == NUMS[i] &&
               "Queue_front() copies wrong data into out parameter");
        assert(Queue_size(q) == begin_sz - 1 &&
               "Queue_size() returns non-one when queue has one element");
        assert(Queue_capacity(q) > 0 && "queue capacity is zero");
    }

    free(front_elem);

    Queue_destroy(q);
}

void test_dequeue_when_only_one() {
    //
    Queue* q         = create_prefilled_test_queue(sizeof(int), 1);

    void* front_elem = malloc(sizeof(int));
    if (front_elem == NULL) {
        handle_error("cannot allocate memory block to copy front element");
    }

    if (!Queue_front(q, front_elem)) {
        handle_error("Queue_front() returns false when queue is not empty");
    }
    assert(*(int*)front_elem == NUMS[0] &&
           "Queue_front() copies wrong data into out parameter");

    if (!Queue_dequeue(q)) {
        handle_error("Queue_dequeue() returns false when queue is not empty");
    }

    assert(!Queue_front(q, front_elem) &&
           "Queue_front() returns true when queue is empty");

    free(front_elem);

    assert(Queue_size(q) == 0 &&
           "Queue_size() returns non-zero when queue is empty");
    assert(Queue_capacity(q) > 0 && "queue capacity is zero");

    Queue_destroy(q);
}

void test_print_when_empty() {
    //
    Queue* q             = create_empty_test_queue(sizeof(int));

    char const* expected = "";

    size_t const buf_sz  = 0x400;

    char actual[buf_sz];
    memset(actual, 0, buf_sz);

    FILE* f1 = freopen("/dev/null", "a", stdout);
    (void)f1;
    setbuf(stdout, actual);

    // horizontal layout
    Queue_print(q, ",", false, print_int);

    FILE* f2 = freopen("/dev/tty", "a", stdout);
    (void)f2;

    printf(">> actual  : %s\n", actual);
    printf(">> expected: %s\n", expected);

    assert(strcmp(expected, actual) == 0 &&
           "string print to stdout differs from what is expected");

    Queue_destroy(q);
}

void test_print_when_nonempty() {
    //
    Queue* q             = create_prefilled_test_queue(sizeof(int), 5);

    char const* expected = "3,1,4,1,5";

    size_t const buf_sz  = 0x400;

    char actual[buf_sz];
    memset(actual, 0, buf_sz);

    FILE* f1 = freopen("/dev/null", "a", stdout);
    (void)f1;
    setbuf(stdout, actual);

    // horizontal layout
    Queue_print(q, ",", false, print_int);

    FILE* f2 = freopen("/dev/tty", "a", stdout);
    (void)f2;

    printf(">> actual  : %s\n", actual);
    printf(">> expected: %s\n", expected);

    assert(strcmp(expected, actual) == 0 &&
           "string print to stdout differs from what is expected");

    Queue_destroy(q);
}

/**
 * Runs unit tests on a specific implementation of the Queue ADT.
 */
int main(int argc, char** argv) {
    UnitTest utests[] = { test_create_with_positive_elem_sz,
                          test_create_with_nonpositive_elem_sz,
                          test_front_when_empty,
                          test_front_when_nonempty,
                          test_enqueue_when_empty,
                          test_enqueue_when_nonempty,
                          test_dequeue_when_empty,
                          test_dequeue_when_at_least_two,
                          test_dequeue_when_only_one,
                          test_print_when_empty,
                          test_print_when_nonempty,
                          NULL };
    run_tests(utests);

    return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------

// clang-format off

/* === COMPILE & RUN ===
gcc ../src/queue_circ_array.c test_queue_impl.c -o test_circ_array_queue -std=c99 -g -Og -Wall -pedantic -march=native -I../src && ./test_circ_array_queue

gcc ../src/queue_linked_list.c test_queue_impl.c -o test_linked_list_queue -std=c99 -g -Og -Wall -pedantic -march=native -I../src && ./test_linked_list_queue

gcc ../src/queue_circ_array.c test_queue_impl.c -o test_circ_array_queue -std=c99 -O3 -march=native -DNDEBUG -I../src && ./test_circ_array_queue

gcc ../src/queue_linked_list.c test_queue_impl.c -o test_linked_list_queue -std=c99 -O3 -march=native -DNDEBUG -I../src && ./test_linked_list_queue
*/

/* === OUTPUT ===
Running...
Test 1 passed 👍
Running...
Test 2 passed 👍
Running...
Test 3 passed 👍
Running...
Test 4 passed 👍
Running...
Test 5 passed 👍
Running...
Test 6 passed 👍
Running...
Test 7 passed 👍
Running...
Test 8 passed 👍
Running...
Test 9 passed 👍
Running...
>> actual  : 
>> expected: 
Test 10 passed 👍
Running...
>> actual  : 3,1,4,1,5
>> expected: 3,1,4,1,5
Test 11 passed 👍
ALL PASSED
*/