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
 * @file      test_utils.h
 * @author    KriztoferY (https://github.com/KriztoferY)
 * @version   0.1.0
 * @copyright Copyright (c) 2023 KriztoferY. All rights reserved.
 *
 * @brief     Testing library.
 *
 * A simple header-only testing library that specifies how to define unit tests
 * and provides an utility function to run an array of defined unit tests passed
 * to it.
 */

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

/**
 * @brief A unit test.
 *
 * Each unit test is defined a function that takes no arguments and returns
 * nothing.
 */
typedef void (*UnitTest)(void);

/**
 * @brief Tests runner.
 *
 * Runs all tests starting from the one pointed by `tests` in a NULL-terminated
 * array of `UnitTest`s.
 *
 * @param tests
 */
void run_tests(UnitTest* tests) {
    for (size_t i = 0; tests[i] != NULL; ++i) {
        printf("%s\n", "Running...");
        tests[i]();
        printf("Test %ld passed 👍\n", i + 1);
    }
    printf("%s\n", "ALL PASSED");
}

/**
 * @brief Error handler.
 *
 * Prints the specified error message to describe the cause of error and then
 * terminate the program in which this function is called.
 *
 * @param message Error message to print to stderr.
 */
void handle_error(char const* message) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

#endif /* TEST_UTILS_H */
