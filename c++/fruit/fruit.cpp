#include <stdio.h>
#include <stdlib.h>

#include "fruit.h"

const char *
Fruit::getTaste() const
{
	return "fruity taste";
}

Fruit::~Fruit()
{
	if (texture_ != NULL)	free(texture_);
	if (colour_ != NULL)	free(colour_);
}


Apple::~Apple()
{
	if (type_ != NULL)	free(type_);
}

const char *
Apple::getType() const
{
	return type_;
}

const char *
Apple::getTaste() const
{
	if (strcmp(type_, "delicious") == 0)
		return "sort of mealy";

	if (strcmp(type_, "granny smith") == 0)
		return "sour";

	return "crisp";
}

