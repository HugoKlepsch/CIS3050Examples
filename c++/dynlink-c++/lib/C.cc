#include <stdio.h>

#include "C.h"

C::C()
{
	printf("In constructor C::C()\n");
	value = -1;
}

C::~C()
{
	printf("In destructor C::~C()\n");
}

void C::setValue(float f)
{
	printf("In set -- setValue(%f)\n", f);
	value = f;
}

float C::getValue() const
{
	printf("In accessor -- %f <- getValue()\n", value);
	return value;
}

