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

#define SSA_OVERRIDE
#include "ssa.h"
#include <stdio.h>

int main(int argc, char **argv) {

	// Allocate a couple hundred bytes over 4 blocks
	char *ptr1 = malloc(100);
	char *ptr2 = malloc(25);
	char *ptr3 = malloc(200);
	char *ptr4 = malloc(5);
	ssa_print_blocks();

	// Deallocate the 200 byte block
	free(ptr3);
	ssa_print_blocks();

	// Allocate an additional 3 blocks
	char *ptr5 = malloc(50);
	char *ptr6 = malloc(50);
	char *ptr7 = malloc(200);
	ssa_print_blocks();

	// Deallocate the first two blocks of 125 bytes and allocate a single 125 byte block
	free(ptr1);
	free(ptr2);
	char *ptr8 = malloc(125);
	ssa_print_blocks();

	// Deallocate everything
	free(ptr4);
	free(ptr5);
	free(ptr6);
	free(ptr7);
	free(ptr8);
	ssa_print_blocks();

	// Allocate the entire heap
	char *ptr9 = malloc(65536 - 2 * 16); // 2 * 16 bytes of space needed for the ptr9 and ptr10 blocks
	ssa_print_blocks();

	// Try allocate a single additional byte
	char *ptr10 = malloc(1);
	ssa_print_blocks();

	free(ptr9);
	free(ptr10); // ptr10 is NULL, so this will be ignored
	ssa_print_blocks();

	ptr1 = malloc(10);
	ptr1[0] = 1; ptr1[1] = 2; ptr1[2] = 3;
	free(ptr1);

	ptr2 = calloc(10,1);
	if (ptr2[0])
		return 1;
	free(ptr2);

	ptr1 = malloc(10);
	ptr2 = malloc(10);
	ptr3 = malloc(10);
	free(ptr2);
	ssa_print_blocks();

	ptr2 = realloc(ptr1, 15);
	ssa_print_blocks();
	if (ptr1 != ptr2)
		return 2;
	ptr1 = realloc(ptr2, 19);
	ssa_print_blocks();
	if (ptr1 != ptr2)
		return 3;
	ptr2 = realloc(ptr1, 20);
	ssa_print_blocks();
	ptr1 = realloc(ptr2, 0);
	ssa_print_blocks();

	return 0;
}
