# Simple Static Allocator

A small project to demonstrate allocating and deallocating arbitrarily sized blocks
of memory in O(n) time. Internally uses a priority queue of allocated blocks to
determine free slots between blocks.

Build the library with `make`. Build some test programs with `make all`.

Licensed under GPLv3\
Ted Johnson 2021
