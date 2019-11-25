/**
 **	An example using globbing to list all of the C source files in the
 ** current directory
 **/
#include <glob.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>



/**
 * utility function for printing out glob results
 */
static void
printGlobArgv(glob_t *glob)
{
	int i;

	printf("  path count : %d\n", (int) glob->gl_pathc);
	printf("  offset     : %d\n", (int) glob->gl_offs);
	printf("  flags      : %x\n", glob->gl_flags);

	for (i = 0; i < glob->gl_pathc; i++)
	{
		printf("  %3d : [%s]>\n", i, glob->gl_pathv[i]);
	}
}

int main()
{
	glob_t g;

	/** make sure that the structure only contains zeros */
	memset(&g, 0, sizeof(glob_t));

	printf("Calculating glob of \"*.c *.md\"\n");

	/** expand all of the .c files into the glob list */
	if (glob("*.c", 0, NULL, &g) < 0) {

		printf("Error listing .c files\n");
		return 1;
	}

	printf("List after *.c:\n");
	printGlobArgv(&g);

	/** expand all of the .md files into the list */
	if (glob("*.md", GLOB_APPEND, NULL, &g) < 0) {
		printf("Error listing .md files\n");
		return 1;
	}

	printf("List after *.md:\n");
	printGlobArgv(&g);

	/** free up the memory from the globbing */
	globfree(&g);

	return 0;
}
