# cdsa-queue <!-- omit in toc -->

**CDSA - Queue** (`cdsa-queue`) is a C module that provides generic implementations of the Queue ADT and related algorithms.

The Queue ADT (or any implementation of **generic queue**) is presented as the opaque type `Queue`. The interface for the Queue ADT is defined in the `queue.h` header file. Different implementations of the Queue ADT are compiled into 
separate static libraries. There're two implementations of the Queue ADT 
included off the shelf:

* `queue_circ_array.c` : Circular array based queue -- compiled as the 
  `libqueuearr` static library

* `queue_linked_list.c` : Singly linked list based queue -- compiled as the 
  `libqueuenode` static library

Here's a little program to familiarize you with the interface.

```c
...
#include "queue.h"   // Queue, Queue_*()

int main() {
    Queue* q = Queue_create(sizeof(int));

    double x = 3.14159265358979323846;
    size_t const num_elems = 5;
    int elem, front_elem;
    bool res;
    for (size_t i = 0; i < num_elems; ++i) {
        elem = (size_t)x % 10;
        x    *= 10;

        res = Queue_enqueue(q, &elem);
        res = Queue_front(q, &front_elem);
        printf("Queue_enqueue(q, %d) :: front: %d | size: %lu | cap: %lu\n",
               elem, front_elem, Queue_size(q), Queue_capacity(q));
    }
    ...
    while (!Queue_empty(q)) {
        res = Queue_front(q, &front_elem);
        printf("front: %d | size: %lu | cap: %lu -- Queue_dequeue(q)\n",
               front_elem, Queue_size(q), Queue_capacity(q));
        res = Queue_dequeue(q);
    }
    ...
    Queue_destroy(q);
    
    return EXIT_SUCCESS;
}
```
Suppose we'd like to use the circular array based implementation of the Queue
ADT in the above sample program `myprog.c`. Let's say the corresponding static 
library is located at `/path/to/lib` and is named `libqueuearr` (as specified 
in the Makefile included). To compile the program into an executable `myprog`, 
at the minimum, run

```bash
$ gcc -std=c99 -L/path/to/lib -lqueuearr myprog.c -o myprog
```

A collection of ADT-implementation-agnostic algorithms on the Queue ADT is included in a dedicated header file `algos.h`.

```c
...
#include "algos.h"    // merge_queues(), print_queue()

bool greater(void const* a, void const* b) { return (int*)a > (int*)b; }
void print_int(void const* a) { printf("%d", *(int*)a); }

...

// Create a queue
Queue* q1       = Queue_create(sizeof(int));
int nums1[]     = { 4, 7, 2, 10 };          // num = priority
size_t const n1 = sizeof(nums1) / sizeof(int);
bool res;
for (size_t i = 0; i < n1; ++i) {
    res = Queue_enqueue(q1, &nums1[i]);
}

// Create another queue
Queue* q2       = Queue_create(sizeof(int));
int nums2[]     = { 3, 6, 8, 9, 5, 1 };     // num = priority
size_t const n2 = sizeof(nums2) / sizeof(int);
for (size_t i = 0; i < n2; ++i) {
    res = Queue_enqueue(q2, &nums2[i]);
}

// Stable-merge: larger the element value, higher the priority
Queue* q = merge_queues(q1, q2, greater); 

// Print merged queue elements in horizontal layout
Queue_print(q, ",", false, print_int);     // prints "4,7,3,6,8,9,5,2,10,1"

// Deallocate memory on the heap
Queue_destroy(q1);
Queue_destroy(q2);
Queue_destroy(q);
```

The module is designed to be **extensible** that developers can write their own implementations and make it a standalone static library with a standardized interface for the Queue ADT, as well as being compatible with the generic algorithms defined in the `algos.h` header.

For more details, visit the <a href="https://kriztofery.github.io/cdsa-queue" target="_blank">documentation site</a>.

Here's what you need to get started.

- [Dependencies](#dependencies)
- [Building the Project](#building-the-project)
- [For Developers \& Contributors](#for-developers--contributors)
- [License](#license)

## Dependencies

To build the project, you will need
- gcc (version 4.5+) or equivalent compiler that supports C99 and above
- Make (or equivalent build tool)

## Building the Project

A Makefile is included in this project to simplify the build process.

For all of the following commands, it's assumed that you're at the project root. If not, `cd` into it like


```bash
$ cd /path/to/project/root
```

or modify the commands with the right path accordingly.

### Full/partial build <!-- omit in toc -->

To make the first build or a clean build, run:

```bash
$ make
```

On success, it builds both static libraries and demo programs. In that case, you'll find two newly created subdirectories (in the first build) under the project root.
1. `lib/` --- contains the static libraries (`*.a` archive files).
2. `bin/` --- contains the executable demo programs (`*_demo`) and test programs (`test_*`).

**ASIDE** : The header files for the Queue ADT and the related generic algorithms are located in the `src/` subdirectory.

Alternatively, you may build only the demo programs, test programs, or libraries individually like so:

```bash
$ make libs     # all static libraries
$ make *_demo   # only the *_demo program
$ make test_*   # only the test_* unit test
```

### Clean <!-- omit in toc -->

If you'd like to have a clean build starting from scratch, you may do so by first running the following a priori:

```bash
$ make clean
```

⚠️ **WARNING** : It permanently removes all demo programs built using the Makefile, all object files under `bin/`, and also all archive files under `lib/`. So use it with caution if you choose to save any other files at those locations. Read the `clean` target in the Makefile if you're in doubt.

## For Developers & Contributors

### Project structure <!-- omit in toc -->

```bash
.
├── src/
├── test/
├── docs/                   
├── bin/                # to be created in the first build
├── lib/                # to be created in the first build
├── .clang-format
├── .gitignore
├── Doxyfile
├── LICENSE
└── README.md
```
Header and source files for the library and demo programs are located in the `src/` subdirectory, whereas those for unit tests are located in the `test/` subdirectory.

### Code formatting <!-- omit in toc -->

Install `clang-format` and run it with the included `.clang-format` config file at the project root.

If you use an IDE, you're strongly revised to configure it to automatically run `clang-format` on each save.

### Documentation <!-- omit in toc -->

#### Style <!-- omit in toc -->

All documentation text are written in the Javadoc style `/** ... */` with `@` as command marker. In multiline form (typically for classes and functions), include aligned leading asterisks `*` in each sandwiched lines. For text that can fit in a single line not exceeding 80 characters (including the comment delimiting characters), use the inline form, either succeeding a statement or on the line preceding the code block to document.

#### Site generation <!-- omit in toc -->

To build the documentation site for the project, you will need
- Doxygen 1.8.8+

With a Doxygen config file included, `cd` into the project root and then simply run

```bash
$ mkdir -p docs/doxygen
$ doxygen
```

The HTML version of the documentation will be saved in the `docs/doxygen/html` subdirectory.

## License

The `cdsa-queue` project is licensed under the [BSD 3-Clause License](https://github.com/KriztoferY/cppdsa-queue/blob/main/LICENSE).

## Also Want It In Another Language?  <!-- omit in toc -->

- C++ : <a href="https://github.com/KriztoferY/cppdsa-queue" target="_blank">Repository</a> | <a href="https://KriztoferY.github.io/cppdsa-queue" target="_blank">Documentation</a>
- Go : <a href="https://github.com/KriztoferY/godsa-queue" target="_blank">Repository</a> | <a href="https://KriztoferY.github.io/godsa-queue" target="_blank">Documentation</a> [coming soon]
- Python : <a href="https://github.com/KriztoferY/pydsa-queue" target="_blank">Repository</a> | <a href="https://KriztoferY.github.io/pydsa-queue" target="_blank">Documentation</a>
- TypeScript : <a href="https://github.com/KriztoferY/tsdsa-queue" target="_blank">Repository</a> | <a href="https://KriztoferY.github.io/tsdsa-queue" target="_blank">Documentation</a>

The C++ equivalent -- <a href="https://github.com/KriztoferY/cppdsa-queue" target="_blank">`cppdsa-queue`</a> -- is the closest cousin of `cdsa-queue`. It is the object-oriented version written in C++20 and also supports 100% compile-time polymorphism via template programming. 

## Acknowledgement <!-- omit in toc -->

This project is bootstrapped using [Cookiecutter](https://cookiecutter.io) with the [cpp-lib-cookiecutter](https://github.com/KriztoferY/cpp-lib-cookiecutter) template (built by the same author of this project).

Copyright &copy; 2022 - 2023 <a href="https://github.com/KriztoferY" target="_blank">KriztoferY</a>. All rights reserved.

