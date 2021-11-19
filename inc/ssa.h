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
#define malloc(x) alloc(x)
#define free(x) dealloc(x)
#endif

/**
 * Reserves and returns a pointer to bytes on the heap.
 *
 * @param size Number of bytes to allocate on the heap
 * @return A pointer to the beginning of the bytes on the heap
 * @note The allocated buffer is not initialized
 * @note NULL is returned if no space could be allocated on the heap
 */
void *alloc(unsigned size);

/**
 * Frees up the allocated bytes on the heap pointed to by ptr.
 *
 * @param ptr The address of the buffer on the heap to be deallocated
 * @note Invalid or unallocated addresses are ignored
 */
void dealloc(void *ptr);

/**
 * Prints the list of currently allocated blocks on the heap.
 * Define SSA_PRINT when compiling to enable.
 */
void print_blocks(void);

#endif
