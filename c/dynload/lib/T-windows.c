#include <stdio.h>

/* define these to the decoration that MS C requires to declare a dynamic object */
#define OS_EXPORT __declspec(dllexport)
#define OS_C_DECL __cdecl

OS_EXPORT int OS_C_DECL
simple()
{
	printf("I am in the \"simple\" function, returning 42\n");
	return 42;
}

OS_EXPORT int OS_C_DECL
another(int i, char *s)
{
	printf("I am in 'another' function, and I got these values:\n");
	printf("    i = %d\n", i);
	printf("    s = '%s'\n", s);
	printf("I am returning 789\n");
	return 789;
}


