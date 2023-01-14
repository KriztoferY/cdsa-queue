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

#include "test_utils.h"   // UnitTest, run_tests(), handle_error()
#include "queue.h"        // Queue, Queue_*()
#include "algos.h"        // merge_queues()

bool greater(void const* a, void const* b) { return *(int*)a > *(int*)b; }
bool less(void const* a, void const* b) { return *(int*)a < *(int*)b; }

void test_merging_two_null_queues(void) {
    // Stable-merge: larger the element value, higher the priority
    Queue* q_gt = merge_queues(NULL, NULL, sizeof(int), greater);
    assert(q_gt == NULL &&
           "merge_queues() returns non-NULL value when both queues are empty");

    // Stable-merge: smaller the element value, higher the priority
    Queue* q_lt = merge_queues(NULL, NULL, sizeof(int), less);
    assert(q_lt == NULL &&
           "merge_queues() returns non-NULL value when both queues are empty");
}

void test_merging_two_empty_queues(void) {
    Queue* q1   = Queue_create(sizeof(int));
    Queue* q2   = Queue_create(sizeof(int));

    // Stable-merge: larger the element value, higher the priority
    Queue* q_gt = merge_queues(q1, q2, sizeof(int), greater);
    assert(q_gt == NULL &&
           "merge_queues() returns non-NULL value when both queues are empty");

    // Stable-merge: smaller the element value, higher the priority
    Queue* q_lt = merge_queues(q1, q2, sizeof(int), less);
    assert(q_lt == NULL &&
           "merge_queues() returns non-NULL value when both queues are empty");

    Queue_destroy(q1);
    Queue_destroy(q2);
}

void test_merging_first_null_second_nonnull(void) {
    Queue*       q1      = NULL;
    Queue*       q2      = Queue_create(sizeof(int));
    int          nums2[] = { 3, 6, 8, 9, 5, 1 };
    size_t const n2      = sizeof(nums2) / sizeof(int);
    for (size_t i = 0; i < n2; ++i) {
        if (!Queue_enqueue(q2, &nums2[i])) {
            handle_error("cannot allocate memory to complete enqueue()");
        }
    }

    // Stable-merge: larger the element value, higher the priority
    Queue* q_gt = merge_queues(q1, q2, sizeof(int), greater);
    assert(q_gt == q2 && "merge_queues() does not return second queue "
                         "when the first is empty");

    // Stable-merge: smaller the element value, higher the priority
    Queue* q_lt = merge_queues(q1, q2, sizeof(int), less);
    assert(q_lt == q2 && "merge_queues() does not return second queue "
                         "when the first is empty");

    Queue_destroy(q1);
    Queue_destroy(q2);
}

void test_merging_first_empty_second_nonempty(void) {
    Queue*       q1      = Queue_create(sizeof(int));
    Queue*       q2      = Queue_create(sizeof(int));
    int          nums2[] = { 3, 6, 8, 9, 5, 1 };
    size_t const n2      = sizeof(nums2) / sizeof(int);
    for (size_t i = 0; i < n2; ++i) {
        if (!Queue_enqueue(q2, &nums2[i])) {
            handle_error("cannot allocate memory to complete enqueue()");
        }
    }

    // Stable-merge: larger the element value, higher the priority
    Queue* q_gt = merge_queues(q1, q2, sizeof(int), greater);
    assert(
        q_gt == q2 &&
        "merge_queues() does not return second queue when the first is empty");

    // Stable-merge: smaller the element value, higher the priority
    Queue* q_lt = merge_queues(q1, q2, sizeof(int), less);
    assert(
        q_lt == q2 &&
        "merge_queues() does not return second queue when the first is empty");

    Queue_destroy(q1);
    Queue_destroy(q2);
}

void test_merging_first_nonnull_second_null(void) {
    Queue*       q1      = Queue_create(sizeof(int));
    Queue*       q2      = NULL;
    int          nums1[] = { 4, 7, 2, 10 };
    size_t const n1      = sizeof(nums1) / sizeof(int);
    for (size_t i = 0; i < n1; ++i) {
        if (!Queue_enqueue(q1, &nums1[i])) {
            handle_error("cannot allocate memory to complete enqueue()");
        }
    }

    // Stable-merge: larger the element value, higher the priority
    Queue* q_gt = merge_queues(q1, q2, sizeof(int), greater);
    assert(
        q_gt == q1 &&
        "merge_queues() does not return first queue when the second is empty");

    // Stable-merge: smaller the element value, higher the priority
    Queue* q_lt = merge_queues(q1, q2, sizeof(int), less);
    assert(
        q_lt == q1 &&
        "merge_queues() does not return first queue when the second is empty");

    Queue_destroy(q1);
    Queue_destroy(q2);
}

void test_merging_first_nonempty_second_empty(void) {
    Queue*       q1      = Queue_create(sizeof(int));
    Queue*       q2      = Queue_create(sizeof(int));
    int          nums1[] = { 4, 7, 2, 10 };
    size_t const n1      = sizeof(nums1) / sizeof(int);
    for (size_t i = 0; i < n1; ++i) {
        if (!Queue_enqueue(q1, &nums1[i])) {
            handle_error("cannot allocate memory to complete enqueue()");
        }
    }

    // Stable-merge: larger the element value, higher the priority
    Queue* q_gt = merge_queues(q1, q2, sizeof(int), greater);
    assert(
        q_gt == q1 &&
        "merge_queues() does not return first queue when the second is empty");

    // Stable-merge: smaller the element value, higher the priority
    Queue* q_lt = merge_queues(q1, q2, sizeof(int), less);
    assert(
        q_lt == q1 &&
        "merge_queues() does not return first queue when the second is empty");

    Queue_destroy(q1);
    Queue_destroy(q2);
}

void print_int(void const* a) { printf("%d", *(int*)a); }

void test_merging_two_nonempty(void) {
    Queue*       q1      = Queue_create(sizeof(int));
    int          nums1[] = { 4, 7, 2, 10 };
    size_t const n1      = sizeof(nums1) / sizeof(int);
    for (size_t i = 0; i < n1; ++i) {
        if (!Queue_enqueue(q1, &nums1[i])) {
            handle_error("cannot allocate memory to complete enqueue()");
        }
    }

    Queue*       q2      = Queue_create(sizeof(int));
    int          nums2[] = { 3, 6, 8, 9, 5, 1 };
    size_t const n2      = sizeof(nums2) / sizeof(int);
    for (size_t i = 0; i < n2; ++i) {
        if (!Queue_enqueue(q2, &nums2[i])) {
            handle_error("cannot allocate memory to complete enqueue()");
        }
    }

    size_t const n         = n1 + n2;
    void*        curr_elem = malloc(sizeof(int));

    // Stable-merge: larger the element value, higher the priority
    int    gt_expected[]   = { 4, 7, 3, 6, 8, 9, 5, 2, 10, 1 };
    Queue* q_gt            = merge_queues(q1, q2, sizeof(int), greater);

    Queue_print(q_gt, ",", false, print_int);
    printf("\n");

    assert(q_gt != NULL &&
           "merge_queues() returns NULL when both queues are not empty");

    for (size_t i = 0; i < n; ++i) {
        if (!Queue_front(q_gt, curr_elem)) {
            handle_error("Queue_front() returns false when queue is not empty");
        }
        printf("actual: %d, expected: %d\n", *(int*)curr_elem, gt_expected[i]);
        assert(*(int*)curr_elem == gt_expected[i] &&
               "merge_queues() orders elements incorrectly");

        if (!Queue_dequeue(q_gt)) {
            handle_error(
                "Queue_dequeue() returns false when queue is not empty");
        }
    }

    // Deallocate memory on the heap
    Queue_destroy(q1);
    Queue_destroy(q2);

    // Rebuild two queues
    q1 = Queue_create(sizeof(int));
    for (size_t i = 0; i < n1; ++i) {
        if (!Queue_enqueue(q1, &nums1[i])) {
            handle_error("cannot allocate memory to complete enqueue()");
        }
    }

    q2 = Queue_create(sizeof(int));
    for (size_t i = 0; i < n2; ++i) {
        if (!Queue_enqueue(q2, &nums2[i])) {
            handle_error("cannot allocate memory to complete enqueue()");
        }
    }

    // Stable-merge: smaller the element value, higher the priority
    int    lt_expected[] = { 3, 4, 6, 7, 2, 8, 9, 5, 1, 10 };
    Queue* q_lt          = merge_queues(q1, q2, sizeof(int), less);

    Queue_print(q_lt, ",", false, print_int);
    printf("\n");

    assert(q_lt != NULL &&
           "merge_queues() returns NULL when both queues are not empty");

    for (size_t i = 0; i < n; ++i) {
        if (!Queue_front(q_lt, curr_elem)) {
            handle_error("Queue_front() returns false when queue is not empty");
        }
        printf("actual: %d, expected: %d\n", *(int*)curr_elem, lt_expected[i]);
        assert(*(int*)curr_elem == lt_expected[i] &&
               "merge_queues() orders elements incorrectly");

        if (!Queue_dequeue(q_lt)) {
            handle_error(
                "Queue_dequeue() returns false when queue is not empty");
        }
    }

    // Deallocate memory on the heap
    free(curr_elem);
    Queue_destroy(q1);
    Queue_destroy(q2);
    Queue_destroy(q_gt);
    Queue_destroy(q_lt);
}

/**
 * Runs all tests on merge_queues().
 */
int main(int argc, char** argv) {
    UnitTest utests[] = { test_merging_two_empty_queues,
                          test_merging_first_empty_second_nonempty,
                          test_merging_first_nonempty_second_empty,
                          test_merging_two_nonempty, NULL };

    run_tests(utests);
    return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------

// clang-format off

/* === COMPILE & RUN ===
gcc ../src/queue_circ_array.c ../src/algos.c test_merge_queues.c -o test_merge_queues_circ_array -std=c99 -g -Og -Wall -pedantic -march=native -I../src && ./test_merge_queues_circ_array

gcc ../src/queue_linked_list.c ../src/algos.c test_merge_queues.c -o test_merge_queues_linked_list -std=c99 -g -Og -Wall -pedantic -march=native -I../src && ./test_merge_queues_linked_list

gcc ../src/queue_circ_array.c ../src/algos.c test_merge_queues.c -o test_merge_queues_circ_array -std=c99 -O3 -march=native -DNDEBUG -I../src && ./test_merge_queues_circ_array

gcc ../src/queue_linked_list.c ../src/algos.c test_merge_queues.c -o test_merge_queues_linked_list -std=c99 -O3 -march=native -DNDEBUG -I../src && ./test_merge_queues_linked_list
*/

/* === OUTPUT ===
Running...
Test 1 passed 👍
Running...
Test 2 passed 👍
Running...
Test 3 passed 👍
Running...
4,7,3,6,8,9,5,2,10,1
actual: 4, expected: 4
actual: 7, expected: 7
actual: 3, expected: 3
actual: 6, expected: 6
actual: 8, expected: 8
actual: 9, expected: 9
actual: 5, expected: 5
actual: 2, expected: 2
actual: 10, expected: 10
actual: 1, expected: 1
3,4,6,7,2,8,9,5,1,10
actual: 3, expected: 3
actual: 4, expected: 4
actual: 6, expected: 6
actual: 7, expected: 7
actual: 2, expected: 2
actual: 8, expected: 8
actual: 9, expected: 9
actual: 5, expected: 5
actual: 1, expected: 1
actual: 10, expected: 10
Test 4 passed 👍
ALL PASSED
*/