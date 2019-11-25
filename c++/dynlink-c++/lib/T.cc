#include <stdio.h>

int
simple()
{
	printf("I am in the \"simple\" function, returning 42\n");
	return 42;
}

int
another(int i, char *s)
{
	printf("I am in 'another' function, and I got these values:\n");
	printf("    i = %d\n", i);
	printf("    s = '%s'\n", s);
	printf("I am returning 789\n");
	return 789;
}


