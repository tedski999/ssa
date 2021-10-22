/**
 * Ski's Simple Allocator
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

#define NULL (0)
#define HEAP_CAPACITY (65536)
#define BLOCK_CAPACITY (1024)

char heap[HEAP_CAPACITY]; // Pointers to within this array will be returned to our client. It will serve as our system memory.
struct block { char *ptr; unsigned size; }; // A series of allocated bytes on the heap is called a block.
struct block blocks[BLOCK_CAPACITY] = { { heap, 0 } }; // A priority queue of blocks sorted by address.
int blocks_count = 1; // We always have at least one block of size 0 at the beginning of the heap to make the logic simplier.

#include "ssa.h"
#include <stdio.h> // printf needed for print_blocks

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
	struct block *new_block = NULL;
	for (int i = 0; i < blocks_count; i++) {
		struct block *prev_block = blocks + i;
		char *after_prev_block = prev_block->ptr + prev_block->size;

		// Try fit the new block in the gap between prev_block and the next.
		// If there is no next block, try fit the new block before the end of the heap.
		if ((i != blocks_count-1 && (prev_block+1)->ptr - after_prev_block >= size) ||
			(i == blocks_count-1 && after_prev_block + size <= heap + HEAP_CAPACITY)) {
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
	new_block->ptr = (new_block-1)->ptr + (new_block-1)->size;
	new_block->size = size;

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
	do blocks[i] = blocks[i + 1]; while (i++ < blocks_count);
}

/**
 * Prints the list of currently allocated blocks on the heap.
 */
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

		// Print the size of a gap if their is one.
		// Also deal with the case that this is the last block in the list
		int gap = (i < blocks_count - 1)
			? blocks[i+1].ptr - (blocks[i].ptr + blocks[i].size)
			: (heap + HEAP_CAPACITY) - (blocks[i].ptr + blocks[i].size);
		if (gap)
			printf("  < Gap of %d bytes >\n", gap);
	}
}
