# 1 "main.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 170 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "main.c" 2


# 1 "./header.h" 1



float myfunc(int, double);
float otherfunc(int, double);
# 4 "main.c" 2

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
