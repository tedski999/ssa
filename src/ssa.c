/**
 * Simple Static Allocator
 * Copyright (C) 2021 Ted Johnson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define NULL ((void *) 0)
#define HEAP_CAPACITY (65536)
#define BLOCK_CAPACITY (1024)

char heap[HEAP_CAPACITY]; // Pointers to within this array will be returned to our client. It will serve as our system memory.
struct block { char *ptr; unsigned size; }; // A series of allocated bytes on the heap is called a block.
struct block blocks[BLOCK_CAPACITY] = { { heap, 0 } }; // A priority queue of blocks sorted by address.
int blocks_count = 1; // We always have at least one block of size 0 at the beginning of the heap to make the logic simplier.

#include "ssa.h"

/**
 * Reserves and returns a pointer to bytes on the heap.
 *
 * Internally, this finds a gap between two blocks large enough to
 * fit n bytes. Then it inserts a new block between them of size n,
 * and returns the corresponding pointer to the bytes in the heap.
 * This function has a time complexity of O(n) where n is the number
 * of blocks currently allocated.
 *
 * @param size Number of bytes to allocate on the heap
 * @return A pointer to the beginning of the bytes on the heap
 * @note The allocated buffer is not initialized
 * @note NULL is returned if no space could be allocated on the heap
 */
void *alloc(unsigned size) {

	// Allocate at least 1 byte to ensure the returned pointer is unique
	if (size == 0)
		size = 1;

	// Can't handle any more blocks
	if (blocks_count >= BLOCK_CAPACITY)
		return NULL;

	// Find a gap between two blocks that can fit this new block
	char *block_start, *block_end;
	struct block *new_block = NULL;
	for (int i = 0; i < blocks_count; i++) {
		struct block *prev_block = blocks + i;
		block_start = prev_block->ptr + prev_block->size;
		block_end = (i < blocks_count - 1) ? (prev_block + 1)->ptr : heap + HEAP_CAPACITY;
		if (block_end - block_start >= size) {
			new_block = prev_block + 1;
			break;
		}
	}

	// No space found
	if (!new_block)
		return NULL;

	// Insert the new block into the list
	struct block *shift_block = blocks + blocks_count;
	while (shift_block-- > new_block)
		*(shift_block+1) = *shift_block;
	*new_block = (struct block) { block_start, size };

	blocks_count++;
	return new_block->ptr;
}

/**
 * Frees up the allocated bytes on the heap pointed to by ptr.
 *
 * Internally, this finds the block with a pointer equal to ptr and
 * removes it from the list of blocks.
 * This function has a time complexity of O(n) where n is the number
 * of blocks currently allocated.
 *
 * @param ptr The address of the buffer on the heap to be deallocated
 * @note Invalid or unallocated addresses are ignored
 */
void dealloc(void *ptr) {
	int i;

	// Find a block with a pointer equal to ptr
	for (i = 1; i < blocks_count; i++) {
		if (blocks[i].ptr == ptr) {
			blocks_count--;
			break;
		}
	}

	// Shift all blocks after i back one to remove i
	do blocks[i] = blocks[i+1]; while (i++ < blocks_count);
}

/**
 * Prints the list of currently allocated blocks on the heap.
 * Define SSA_PRINT when compiling to enable.
 */
#ifdef SSA_PRINT
#include <stdio.h> // printf
void print_blocks(void) {
	printf("\nAllocated blocks:\n");
	for (int i = 0; i < blocks_count; i++) {

		// Print block details if not the first zero-sized block
		if (blocks[i].size) {
			printf(
				"  %p - %p (%d bytes)\n",
				(void *) blocks[i].ptr,
				(void *) (blocks[i].ptr + blocks[i].size - 1),
				blocks[i].size);
		}

		// Print the size of a gap if there is one
		struct block *prev_block = blocks + i;
		char *block_start = prev_block->ptr + prev_block->size;
		char *block_end = (i < blocks_count - 1) ? (prev_block + 1)->ptr : heap + HEAP_CAPACITY;
		int gap = block_end - block_start;
		if (gap)
			printf("  < Gap of %d bytes >\n", gap);
	}
}
#else
void print_blocks(void) {}
#endif
