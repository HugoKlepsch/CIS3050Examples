
#include "header.h"

static int anotherfunc(int i);


float myfunc(int i, double d)
{
	return (2.5 * d) / (float) anotherfunc(i);
}

static int anotherfunc(int i)
{
	return i * 2;
}


