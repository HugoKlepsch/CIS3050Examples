/*
 * Standard "intro" C program
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
countLen(char *s)
{
	int l = 0;

	/**
	 * '\0' is a character 'nul' (ASCII character 0); commonly you will
	 * also see people use simply 0 (integer 0)
	 *
	 * Note the combination of pointer dereference and post-increment
	 * to get the value the pointer currently points at and update
	 * the address in the pointer to the next byte
	 */
	while (*s++ != '\0')
		l++;

	return l;
}

int
main()
{
	char *strings[] = {
			"My dog has no nose",
			"Then how does he smell?",
			"Terrible!",
			NULL
		};
	int i, len;

	for (i = 0; strings[i] != NULL; i++) {
		len = countLen(strings[i]);
		printf("%d : %2d : %s\n", i, len, strings[i]);
	}

	exit (0);
}

