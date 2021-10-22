# Ski's Simple Allocator

A small project to demonstrate allocating and deallocating arbitrarily sized blocks
of memory in O(n) time. Internally uses a priority queue of allocated blocks to
determine free slots between blocks.

Build and run with `make` or `cc src/main.c src/ssa.c -o ssa && ./ssa`.

Licensed under GPLv3\
Ted Johnson 2021
