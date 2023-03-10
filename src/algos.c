/*** Out-of-line definitions ***/
#include "algos.h"

#include <stdbool.h>   // bool
#include <stdlib.h>    // malloc()
#include <assert.h>    // assert()

Queue* merge_queues(Queue* queue1, Queue* queue2, size_t elem_sz,
                    bool (*compare)(void const*, void const*)) {
    if (queue1 == NULL && queue2 == NULL) return NULL;
    if (queue1 == NULL) return queue2;
    if (queue2 == NULL) return queue1;
    assert(compare != NULL && "compare is not a binary predicate");

    bool q1_is_empty = Queue_empty(queue1);
    bool q2_is_empty = Queue_empty(queue2);

    if (q1_is_empty && q2_is_empty) return NULL;
    if (q1_is_empty && !q2_is_empty) return queue2;
    if (!q1_is_empty && q2_is_empty) return queue1;

    void* elem1 = malloc(elem_sz);
    void* elem2 = malloc(elem_sz);
    bool  res;

    Queue* merged     = Queue_create(elem_sz);
    Queue* curr_queue = NULL;
    void*  curr_elem  = NULL;

    // Compare the elements at the front of two queues
    while (!Queue_empty(queue1) && !Queue_empty(queue2)) {
        res = Queue_front(queue1, elem1);
        assert(res && "Queue_front() failed when queue not empty");
        res = Queue_front(queue2, elem2);
        assert(res && "Queue_front() failed when queue not empty");

        if (compare(elem1, elem2)) {
            curr_queue = queue1;
            curr_elem  = elem1;
        } else {
            curr_queue = queue2;
            curr_elem  = elem2;
        }

        res = Queue_enqueue(merged, curr_elem);
        assert(res && "cannot allocate memory to filful enqueue()");

        res = Queue_dequeue(curr_queue);
        assert(res && "Queue_dequeue() failed when queue not empty");
    }

    // Retain elem1 on the heap to copy front element into for rest of algo
    free(elem2);

    // Find out which queue has unprocessed elements
    if (!Queue_empty(queue1)) {
        curr_queue = queue1;
    } else {
        curr_queue = queue2;
    }

    // Sequentially move unprocessed elements into the merged queue
    while (!Queue_empty(curr_queue)) {
        res = Queue_front(curr_queue, elem1);
        assert(res && "Queue_front() failed when queue not empty");

        res = Queue_enqueue(merged, elem1);
        assert(res && "cannot allocate memory to filful enqueue()");

        res = Queue_dequeue(curr_queue);
        assert(res && "Queue_dequeue() failed when queue not empty");
    }

    free(elem1);

    return merged;
}
