#include <stdio.h>
#include <math.h>
#include "header.h"

int
main()
{
	int i;
	double d;
	float f;

	d = 37.5;
	i = 37;

	f = myfunc(i, d);

	printf("d = %lf\n", d);
	printf("i = %d\n", i);
	printf("f = %f\n", f);

	printf("sin(0) = %f\n", sin(0));


	return 0;
}
