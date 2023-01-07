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
 * @file      algos.h
 * @author    KriztoferY (https://github.com/KriztoferY)
 * @version   0.1.0
 * @copyright Copyright (c) 2023 KriztoferY. All rights reserved.
 *
 * @brief     Generic algorithms on the Queue ADT.
 *
 * All algorithms are independent of the implementation of the Queue ADT.
 */

#ifndef QUEUE_ALGOS_H
#define QUEUE_ALGOS_H

#include <stdbool.h>   // bool

#include "queue.h"   // Queue, Queue_*()

/**
 * @brief Stable-merges two queues.
 *
 * Elements are compared using the binary predicate `compare` to determine the
 * order in which they appear in the merged queue. Relative order of elements
 * in the original queues are preserved. A new queue is created and returned if
 * both queues to merge are not empty.
 *
 * @param[in] queue1 A queue to merge.
 * @param[in] queue2 Another queue to merge.
 * @param[in] elem_sz Size of each queue elements in bytes. **[IMPORTANT]**
 *      It's the caller's responsibility to ensure `elem_sz` is a proper
 *      positive integer.
 * @param[in] compare A binary predicate that determines the element order in
 *      the merged queue; it has not effect on the relative order of elements
 *      in the original queues.
 * @return The merged queue if both queues to merge are not empty, one of the
 *      queues to merge if the other is empty, `NULL` if both are empty.
 *      **[IMPORTANT]** In the first case, call `Queue_destroy()` when
 *      you're done with the merged queue to free the memory allocated to it.
 * @note The complexity of the merge algorithm is `O(n1 + n2)` in both time and
 *      space, where `n1` and `n2` are the sizes of the two queues to merge.
 */
Queue* merge_queues(Queue* queue1, Queue* queue2, size_t elem_sz,
                    bool (*compare)(void const*, void const*));

// -----------------------------------------------------------------------------

#endif /* QUEUE_ALGOS_H */