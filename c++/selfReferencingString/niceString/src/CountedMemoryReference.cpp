/**
 * Body of reference counter implementation
 *
 * $Id: CountedMemoryReference.cpp 492 2012-04-02 14:49:41Z andrew $
 */


#ifndef MAKEDEPEND
# include <stdio.h>
#endif

#include "CountedMemoryReference.h"

CountedMemoryReference::CountedMemoryReference()
{
	referenceCount = 0;
	wasTouched = 0;
}

CountedMemoryReference::~CountedMemoryReference()
{
}

char *
CountedMemoryReference::clsId() const
{
	return (char *) "CountedMemoryReference";
}

