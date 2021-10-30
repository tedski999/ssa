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

int main(int argc, char **argv) {

	// Allocate a couple hundred bytes over 4 blocks
	char *ptr1 = alloc(100);
	char *ptr2 = alloc(25);
	char *ptr3 = alloc(200);
	char *ptr4 = alloc(5);
	print_blocks();

	// Deallocate the 200 byte block
	dealloc(ptr3);
	print_blocks();

	// Allocate an additional 3 blocks
	char *ptr5 = alloc(50);
	char *ptr6 = alloc(50);
	char *ptr7 = alloc(200);
	print_blocks();

	// Deallocate the first two blocks of 125 bytes and allocate a single 125 byte block
	dealloc(ptr1);
	dealloc(ptr2);
	char *ptr8 = alloc(125);
	print_blocks();

	// Deallocate everything
	dealloc(ptr4);
	dealloc(ptr5);
	dealloc(ptr6);
	dealloc(ptr7);
	dealloc(ptr8);
	print_blocks();

	// Allocate the entire heap
	char *ptr9 = alloc(65536);
	print_blocks();

	// Try allocate a single additional byte
	char *ptr10 = alloc(1);
	print_blocks();

	dealloc(ptr9);
	dealloc(ptr10); // ptr10 is NULL, so this will be ignored
	print_blocks();
}
