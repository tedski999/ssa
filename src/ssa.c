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

#include "ssa.h"

#define NULL ((void *) 0)
#define HEAP_CAPACITY (65536)

struct block { char *ptr; unsigned size; };
static char heap[HEAP_CAPACITY];
static int block_count;

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
void *ssa_malloc(unsigned size) {

	// Allocate at least 1 byte to ensure the returned pointer is unique
	if (size == 0)
		size = 1;

	// Locate space for the new block
	char *end = heap;
	struct block *new_block = (struct block *) (heap + HEAP_CAPACITY) - 1;
	for (int i = 0; i < block_count; i++, new_block--) {
		if (new_block->ptr - end >= size)
			break;
		end = new_block->ptr + new_block->size;
	}

	// Check there is enough free space between allocated memory and the block list including this new block
	struct block *last_block = (struct block *) (heap + HEAP_CAPACITY) - block_count;
	char *free_space_start = block_count ? last_block->ptr + last_block->size : heap;
	free_space_start += (new_block == last_block-1) * size;
	if (free_space_start > (char *) (last_block-1))
		return NULL;

	// Insert the new block into the list
	struct block *shift_block = last_block;
	while (shift_block <= new_block) {
		*(shift_block-1) = *shift_block;
		shift_block++;
	}
	block_count++;
	new_block->ptr = end;
	new_block->size = size;
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
void ssa_free(void *ptr) {

	// Find a block with a pointer equal to ptr
	struct block *last_block = (struct block *) (heap + HEAP_CAPACITY) - block_count;
	struct block *curr_block = (struct block *) (heap + HEAP_CAPACITY) - 1;
	while (curr_block >= last_block && curr_block->ptr != ptr)
		curr_block--;

	// If a block was found, remove it from the list
	if (curr_block >= last_block) {
		block_count--;
		while (curr_block > last_block) {
			*curr_block = *(curr_block-1);
			curr_block--;
		}
	}
}

/**
 * Reserves and returns a pointer to bytes on heap, all initialized to zero.
 *
 * This function simply wraps ssa_malloc and zeros out the returned buffer.
 * As such, it also runs in O(n*m) time, where is the number of blocks
 * currently allocated and m is count*size
 *
 * @param count Number of members to allocate on the heap
 * @param size Size in bytes of each member
 * @return A pointer to the beginning of the bytes on the heap
 * @note The total number of bytes reserved on the heap is simply count*size
 * @note NULL is returned if no space could be allocated on the heap
 */
void *ssa_calloc(unsigned count, unsigned size) {

	// Allocate at least 1 byte to ensure the returned pointer is unique
	if (count == 0 || size == 0)
		count = size = 1;

	// Wrap ssa_malloc and set zero out returned buffer
	char *ptr = ssa_malloc(count * size);
	for (int i = 0; ptr && i < count * size; i++)
		ptr[i] = 0;
	return ptr;
}

/**
 * Changes the size of the allocated block pointed to by ptr.
 *
 * First finds the block corresponding to ptr, if not found it simply returns
 * NULL. Otherwise, if we change set the new size of this block without
 * interfering with any other blocks or overlapping into the block list,
 * we just set the block size to the new size and return the original pointer.
 * Finally, if we can't expand the block we have to completely reallocate the
 * contents of the block into a bigger buffer. To do that, we allocate another
 * block with the correct size and copy the contents of the old block into it.
 * We also then free the old block.
 * In total, this function runs in O(n*m) time, where is the number of blocks
 * currently allocated and m is size of the original block.
 *
 * @param ptr The address of the buffer on the heap to be resized
 * @param size Number of bytes to resize the buffer to
 * @return The address of the resized buffer
 * @note Invalid or unallocated addresses are ignored and return NULL
 */
void *ssa_realloc(void *ptr, unsigned size) {

	// Allocate at least 1 byte to ensure the returned pointer is unique
	if (size == 0)
		size = 1;

	// Find a block with a pointer equal to ptr
	struct block *last_block = (struct block *) (heap + HEAP_CAPACITY) - block_count;
	struct block *curr_block = (struct block *) (heap + HEAP_CAPACITY) - 1;
	while (curr_block >= last_block && curr_block->ptr != ptr)
		curr_block--;

	// Block wasn't found
	if (curr_block < last_block)
		return NULL;

	// Are we able to simple contract or expand reserved memory?
	char *free_space_end = (curr_block == last_block) ? (char *) last_block : (curr_block-1)->ptr;
	if (curr_block->ptr + size <= free_space_end) {
		curr_block->size = size;
		return ptr;
	}

	// Otherwise we have to copy the contents to a new allocation
	// TODO: Is it possible to free before allocating?
	char *new_ptr = ssa_malloc(size);
	if (!new_ptr)
		return NULL;
	for (int i = 0; i < curr_block->size; i++)
		new_ptr[i] = curr_block->ptr[i];
	ssa_free(ptr);
	return new_ptr;
}

/**
 * Prints the list of currently allocated blocks on the heap.
 * Define SSA_PRINT when compiling to enable.
 */
#ifdef SSA_PRINT
#include <stdio.h> // printf
void ssa_print_blocks(void) {
	char *end = heap;
	struct block *block = (struct block *) (heap + HEAP_CAPACITY) - 1;

	printf("\nAllocated blocks:\n");
	for (int i = 0; i < block_count; i++, block--) {
		if (block->ptr - end)
			printf("  < gap of %d bytes >\n", (int) (block->ptr - end));
		end = block->ptr + block->size;
		printf("  %p - %p (%d bytes)\n", (void *) block->ptr, (void *) (end - 1), block->size);
	}
	printf("  < %d bytes remaining >\n", (int) ((char *) (block + 1) - end));
}
#else
void print_blocks(void) {}
#endif
