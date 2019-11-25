#include <stdio.h>

int
main(int argc, char **argv)
{
	const char *s = "Hello";
	float v1 = 12.345, v2 = 1234.5;

	printf("Hello, World!\n");

	printf("%s %f %f\n", s, v1, v2);
	printf("%12s (%9.1f) (%9.1f)\n", s, v1, v2);

	return 0;
}

