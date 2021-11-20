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

#ifndef SSA_H
#define SSA_H

#ifdef SSA_OVERRIDE
#define malloc(s) ssa_malloc(s)
#define free(p) ssa_free(p)
#define calloc(n, s) ssa_calloc(n, s)
#define realloc(p, s) ssa_realloc(p, s)
#endif

/**
 * Reserves and returns a pointer to bytes on the heap.
 *
 * @param size Number of bytes to allocate on the heap
 * @return A pointer to the beginning of the bytes on the heap
 * @note The allocated buffer is not initialized
 * @note NULL is returned if no space could be allocated on the heap
 */
void *ssa_malloc(unsigned size);

/**
 * Frees up the allocated bytes on the heap pointed to by ptr.
 *
 * @param ptr The address of the buffer on the heap to be deallocated
 * @note Invalid or unallocated addresses are ignored
 */
void ssa_free(void *ptr);

/**
 * Reserves and returns a pointer to bytes on heap, all initialized to zero.
 *
 * @param count Number of members to allocate on the heap
 * @param size Size in bytes of each member
 * @return A pointer to the beginning of the bytes on the heap
 * @note The total number of bytes reserved on the heap is simply count*size
 * @note NULL is returned if no space could be allocated on the heap
 */
void *ssa_calloc(unsigned count, unsigned size);

/**
 * Changes the size of the allocated block pointed to by ptr.
 *
 * @param ptr The address of the buffer on the heap to be resized
 * @param size Number of bytes to resize the buffer to
 * @return The address of the resized buffer
 * @note Invalid or unallocated addresses are ignored and return NULL
 */
void *ssa_realloc(void *ptr, unsigned size);

/**
 * Prints the list of currently allocated blocks on the heap.
 * Define SSA_PRINT when compiling to enable.
 */
void ssa_print_blocks(void);

#endif
