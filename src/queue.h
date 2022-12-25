/*
BSD 3-Clause License

Copyright (c) 2022, KriztoferY <https://github.com/KriztoferY>
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
 * @file      queue.h
 * @author    KriztoferY
 * @version   0.1.0
 * @copyright Copyright (c) 2022 KriztoferY. All rights reserved.
 *
 * @brief     The abstract data type (ADT) queue.
 *
 * Queue is a sequential ADT that emulates the first-in-first-out behavior of a
 * queue in real world. This modules define the interface of the ADT queue.
 *
 * Use @ref `Queue_create()` to create a queue, which should be destroyed when
 * it is no longer needed using @ref `Queue_destroy()`. To add an element at
 * the end of a queue, use @ref `Queue_enqueue()`. To remove the front element,
 * i.e, the least recently added element, from a queue, use @ref
 * `Queue_dequeue()`.
 *
 * All functions that accept a pointer to a queue asserts at compile time that
 * the queue is not `NULL`.
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>    // size_t
#include <stdbool.h>   // bool

/** An opaque type representing a generic queue.*/
typedef struct queue Queue;

/**
 * @brief Creates an empty, heap-allocated queue.
 *
 * It's the caller's responsibility to
 * -# call @ref `Queue_destroy()` to free all allocated memory associated
 *    with the queue created; and
 * -# ensure `elem_sz` is a proper positive integer.
 *
 * @param[in] elem_sz Size of each queue elements in bytes.
 * @return The queue created on success, `NULL` otherwise.
 */
Queue* Queue_create(size_t elem_sz);

/**
 * @brief Destroys a heap-allocated queue.
 *
 * It is a no-op if the `queue` is `NULL`.
 *
 * @param queue The queue to destroy.
 */
void Queue_destroy(Queue* queue);

/**
 * @brief Queries the capacity of a queue.
 *
 * For node-based implementations, it always return `ULONG_MAX` to suggest that
 * the queue can hold as many elements as system memory allows.
 *
 * @param queue The queue to query.
 * @return Maximum number of elements that can be stored by the queue.
 */
size_t Queue_capacity(Queue* queue);

/**
 * @brief Determines whether a queue is empty.
 *
 * @param queue The queue to query.
 * @return `true` if the queue is empty, `false` otherwise.
 */
bool Queue_empty(Queue* queue);

/**
 * @brief Queries the size of a queue.
 *
 * @param queue The queue to query.
 * @return Number of elements in the queue.
 */
size_t Queue_size(Queue* queue);

/**
 * @brief Accesses the front element of a queue.
 *
 * @param[in] queue The query to query.
 * @param[out] elem The front element if the queue is not empty,
 *      undefined otherwise.
 * @return `false` if the queue is empty, `true` otherwise (on success).
 */
bool Queue_front(Queue* queue, void* elem);

/**
 * @brief Adds an element to the end of a queue.
 *
 * @param queue The queue to which the element is to add.
 * @param elem The element to add.
 * @return `false` if the system cannot allocate sufficient memory to complete
 *      the operation; `true` otherwise (on success).
 */
bool Queue_enqueue(Queue* queue, void const* elem);

/**
 * @brief Removes the front element from a queue.
 *
 * @param queue The queue from which its least recent element is to remove.
 * @return `false` if the queue is empty, `true` otherwise (on success).
 */
bool Queue_dequeue(Queue* queue);

#endif /* QUEUE_H */