#ifndef __FRUIT_HEADER__
#define __FRUIT_HEADER__

#include <stdio.h>	// for NULL

#include <string.h>	// for strdup

class Fruit {
public:
	const char *getColour() const { return colour_; }
	const char *getTexture() const { return texture_; }
	virtual const char *getTaste() const;

public:
	Fruit() { colour_ = texture_ = NULL; }
	Fruit(const char *colour, const char *texture) {
			colour_ = strdup(colour);
			texture_ = strdup(texture);
		}

	virtual ~Fruit();

private:
	char *colour_;
	char *texture_;
};

class Apple : public Fruit
{
public:
	Apple(const char *type, const char *colour) : Fruit(colour, "smooth")
	{
		type_ = strdup(type);
	}
	virtual ~Apple();

	const char *getTaste() const;
	const char *getType() const;

	char *type_;
};

class Orange : public Fruit
{
public:
	Orange() : Fruit("orange", "rough") { }
	virtual ~Orange() {}
};

#endif /* __FRUIT_HEADER__ */
