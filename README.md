# Simple Static Allocator

![GitHub Build Workflow](https://github.com/tedski999/ssa/actions/workflows/build.yml/badge.svg)

A small project to demonstrate allocating and deallocating arbitrarily sized blocks
of memory in O(n) time. Internally uses a priority queue of allocated blocks to
determine free slots between blocks.

Build the library with `make`.
Test the library with `make check`.
Build the example programs with `make all`.

Licensed under GPLv3\
Ted Johnson 2021
