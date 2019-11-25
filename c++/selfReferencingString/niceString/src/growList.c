/* ------------------------------------------------------------
 * list allocation and scaling routines
 * ------------------------------------------------------------
 * $Id: growList.c 492 2012-04-02 14:49:41Z andrew $
 */

#ifndef MAKEDEPEND
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "growList.h"

/*
 * ---------------------------------------------
 * grow a memory list to the needed size
 * ---------------------------------------------
 */
int
growListToSize(size_needed, list, cur_blocks, blksize, tilesize)
	int size_needed;
	void **list;
	int *cur_blocks,
	blksize,
	tilesize;
{
	int calcblocks, calcbytes;
	void *old_data;

	/** check whether we are just in the simple case **/
	if (size_needed <= (*cur_blocks) * blksize) {
		return (1);
	}

	/**
	 ** calculate the number of blocks we will need,
	 ** allowing for truncation of the number
	 **/
	calcblocks = (size_needed / blksize) + 1;

	/** convert to number of bytes      **/
	calcbytes = calcblocks * blksize * tilesize;

	/** allocate and copy old values **/
	old_data = (*list);
	(*list) = (void *) malloc(calcbytes);
	if ( (*list) == NULL) {
		(*list) = old_data;
		return (0);
	}
	memset((*list), 0, calcbytes);

	if (old_data != NULL) {
		/** copy over old size data **/
		(void) memcpy((*list), old_data,
				(*cur_blocks) * blksize * tilesize);
		free(old_data);
	}

	(*cur_blocks) = calcblocks;

	return (1);
}

