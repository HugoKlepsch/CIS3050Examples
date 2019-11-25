/*
 * A C file with a couple of simple functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "simple.h"


/*
 * allocate a structure
 */
simple_data *
simpleAlloc(float value)
{
    simple_data *r;

    r = malloc(sizeof(simple_data));
	r->value_ = value;

    return r;
}

/*
 * delete a structure
 */
void
simpleFree(simple_data *s)
{
    if (s != NULL) free(s);
}

/*
 * accessor to get a value (really just to make the
 * name work without underscores)
 */
float
simpleGetValue(simple_data *s)
{
    if (s != NULL)
		return s->value_;

    return (-log(0));
}

/*
 * accessor to get a value (really just to make the
 * name work without underscores)
 */
void
simpleSetValue(simple_data *s, float newValue)
{
    if (s != NULL)
		s->value_ = newValue;
}

