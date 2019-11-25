#include <stdio.h>

#include "NiceString.h"


/**
 * Example program demonstrating some string tricks
 */
void
concatenateAndPrintArgs(int argc, char **argv)
{
	NiceString s, a;
	int i;

	printf("Prints arguments out as a string:\n");

	for (i = 0; i < argc; i++) {
		a = argv[i];
		if (i != 0) {
			s = s + " ";
			a = a.getQuoted();
		}
		s = s + a;
	}

	puts(s.getValue());
}

void
testEqualities()
{
	NiceString a("abc");
	NiceString b("abc");
	NiceString c("def");


	if (a == b) {
		printf("A(%s) = B(%s)\n", a.getValue(), (const char *) b);
	} else {
		printf("A(%s) != B(%s)\n", a.getValue(), (const char *) b);
	}

	if (b == c) {
		printf("A(%s) = C(%s)\n", (const char *) b, (const char *) c);
	} else {
		printf("A(%s) != C(%s)\n", (const char *) b, (const char *) c);
	}

	b = c;

	if (b == c) {
		printf("A(%s) = C(%s)\n", (const char *) b, (const char *) c);
	} else {
		printf("A(%s) != C(%s)\n", (const char *) b, (const char *) c);
	}

	a.sprintf("A is now '%s' and '%s'", (const char *) a, (const char *) b);

	printf("a: [%s]\n", a.getValue());
}

/*
 * Declare a NiceString as a local variable and then pass it back!
 * Do you see why this does not break?
 */
NiceString
getAString()
{
	NiceString s("Hello!");
	return s;
}

int
main(int argc, char **argv)
{
	NiceString s, n;

	s.sprintf("HI there -- %d %f", 12, 123.45);

	printf("[%s]\n", s.getValue());

	concatenateAndPrintArgs(argc, argv);

	testEqualities();

	s = getAString();

	printf("Value in 's' is [%s]\n", s.getValue());
	printf("Value in 'n' is [%s]\n", n.getValue());

	return 0;
}

