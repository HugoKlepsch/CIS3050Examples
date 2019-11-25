/**
 **	An example of using globbing within a subdirectory.  Note that
 ** the "files" directory must be present for this program to find
 ** anything.
 **/
#include <stdio.h>
#include <glob.h>
#include <sys/wait.h>
#include <unistd.h>	/* for execvp */
#include <string.h>	/* for strerror and memset */
#include <stdlib.h>	/* for exit */
#include <errno.h>	/* for errno */


/**
 * utility function for printing out glob results
 */
static void
printGlobArgv(int startIndex, glob_t *glob)
{
	int i;

	printf("  path count : %d\n", (int) glob->gl_pathc);
	printf("  offset     : %d\n", (int) glob->gl_offs);
	printf("  flags      : %x\n", glob->gl_flags);

	for (i = startIndex; i < startIndex + glob->gl_pathc; i++)
	{
		printf("  %3d : [%s]>\n", i, glob->gl_pathv[i]);
	}
}

int main()
{
	char *globPattern = "files/[A-Z]*";
	glob_t g;
	pid_t childPid;

	/** make sure that the structure only contains zeros */
	memset(&g, 0, sizeof(glob_t));

	/** leave two spots open at the beginning of the list for later */
	g.gl_offs = 2;

	printf("Calculating glob of '%s'\n", globPattern);

	/** expand all of the indicated files into the glob list */
	if (glob(globPattern, GLOB_DOOFFS, NULL, &g) < 0) {
		printf("error: glob returned negative value!\n");
		printf("error: glob reporting error (%d) '%s'\n", errno, strerror(errno));

		return (-1);
	
	}

	printf("List after expansion:\n");
	printGlobArgv(2, &g);

	
	/** populate the two spots we left open at the beginning of the list */
	g.gl_pathv[0] = "ls";
	g.gl_pathv[1] = "-l";

	childPid = fork();
	if (childPid == 0) {

		execvp("ls", g.gl_pathv);

		perror("exec failed");
		exit (-1);
	}

	/** free up the memory from the globbing */
	globfree(&g);

	if (wait(NULL) != childPid) {
		perror("Unexpected value from wait()");
	}

	return 0;
}
