/* ------------------------------------------------------------
 * list allocation tools
 * ------------------------------------------------------------
 * $Id: growList.h 492 2012-04-02 14:49:41Z andrew $
 */

#ifndef         LIST_ALLOCATION_HEADER__
#define         LIST_ALLOCATION_HEADER__

#ifndef MAKEDEPEND
# include       <stdio.h>
#endif

/**
 ** PROTOTYPES
 **/

# if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
# endif

int growListToSize(
				int size_needed,
				void **list,
				int *cur_blocks,
				int blksize,
				int tilesize
			);

# if defined(__cplusplus) || defined(c_plusplus)
}
# endif

#endif  /* LIST_ALLOCATION_HEADER__     */

