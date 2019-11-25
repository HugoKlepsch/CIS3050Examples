#include <stdio.h>

#include "fruit.h"

void
printFruit(Fruit *afruit)
{
	printf("Printing fruit information:\n");
	printf("  colour  : %s\n", afruit->getColour());
	printf("  taste   : %s\n", afruit->getTaste());
	printf("  texture : %s\n", afruit->getTexture());
}

void
printApple(Apple *anApple)
{
	printFruit(anApple);
	printf("  type : %s\n", anApple->getType());
}

int
main()
{
	Apple appleInstance("delicious", "red"), *anAppleRef = NULL;
	Apple *grannySmith = NULL,
			*macintosh = NULL;
	Orange *orange = NULL;


	anAppleRef = &appleInstance;

	orange = new Orange();

	grannySmith = new Apple("granny smith", "green");
	macintosh = new Apple("macintosh", "red");

	printf("Printing orange:\n");
	printFruit(orange);

	printf("Printing appleInstance:\n");
	printFruit(&appleInstance);

	printf("\n");

	printf("Printing granny smith:\n");
	printFruit(grannySmith);

	printf("Printing macintosh:\n");
	printFruit(macintosh);

}

