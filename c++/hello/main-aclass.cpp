#include <stdio.h>

class Fruit
{
private:
	double dval;

public:
	Fruit();

	float aMethod(int arg) { return (float) arg / 2.5; }
	void setVal(float v);
	float getVal();
};

Fruit::Fruit()
{
	dval = 0.5;
}

void
Fruit::setVal(float newVal)
{
	dval = newVal;
}

float
Fruit::getVal()
{
	return dval;
}


float
aFunction(int arg)
{
	return (float) arg * 2.5f;
}

int
main(int argc, char **argv)
{
	Fruit f, *fp;

	printf("AFunction(2) -> %f\n", aFunction(2));

	f.setVal(9.0);
	fp = new Fruit();
	fp->setVal(12.5);

	printf("f  = %g\n", f.aMethod( (int) f.getVal() ) );
	printf("fp = %g\n", fp->aMethod( (int) fp->getVal() ) );

	delete fp;

	return 0;
}

