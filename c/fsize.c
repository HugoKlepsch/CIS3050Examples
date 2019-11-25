
/*----------------------------------------------------------------
 * List the entries and sizes of various files
 *
 * Adapted from pp 180-182 of K+R (1988)
 *
 * $Author: andrew $
 * $Date: 2009-03-01 16:18:15 -0400 (Sun, 01 Mar 2009) $
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#if defined( _WIN32 )
#include <io.h>
//#include <stlib.h>
#else
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#endif


/*
 * Take into account the MS-specific naming as the FAT based
 * struct _stat is not the same as the UFS derived struct stat
 */
# define STATBUF struct stat
# define STATFUNC(n,b) stat(n, b)
# define STAT_ISDIR(m)		(((m) & S_IFDIR) == S_IFDIR)
# define DIRWALK(n, f)		dirwalkUnix(n, f)


/* forward reference for dirwalk function */
int DIRWALK(const char *, int (*fsizeFunc)(const char *));


/* fsize: print size of file "name */
int
fsize(const char *name)
{
	STATBUF stbuf;

	if (STATFUNC(name, &stbuf) < 0) {
		fprintf(stderr, "fsize: can't access %s\n", name);
		return -1;
	}

	if (STAT_ISDIR(stbuf.st_mode))
		if (DIRWALK(name, fsize) < 0)
			return -1;

	printf("%8ld %s\n", (long) stbuf.st_size, name);

	return 0;
}


/* dirwalk: apply fsizeFunc to all files in dir */
int dirwalkUnix(const char *dir, int (*fsizeFunc)(const char *))
{
	char name[FILENAME_MAX];
	DIR *directory;
	struct dirent *dentry;

	if ((directory = opendir(dir)) == NULL)
		return -1;

	while ((dentry = readdir(directory)) != NULL) {

		/* skip self and parent */
		if (strcmp(dentry->d_name, ".") == 0
				|| strcmp(dentry->d_name, "..") == 0)
			continue;

		if (strlen(dir) + strlen(dentry->d_name) + 2 > sizeof(name))
			fprintf(stderr, "dirwalk: name %s/%s too long\n",
					dir, dentry->d_name);
		else {
			sprintf(name, "%s/%s", dir, dentry->d_name);
			(*fsizeFunc)(name);
		}
	}

	closedir(directory);

	return 0;
}



int
main(int argc, char **argv)
{
	if (argc == 1)
		fsize(".");
	else
		while (--argc > 0)
			(void) fsize(*++argv);
	return 0;
}


