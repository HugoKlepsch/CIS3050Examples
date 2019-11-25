
/**
 * declare function that exists in Fortran file.
 * Note the underscore in the name.
 *
 * (This could be put in a header.)
 */
void fortfunc_(int *ii, float *ff);


/**
 * Our main program, that calls the Fortran function
 */
int
main()
{
	int ii = 5;
	float ff = 5.5;

	fortfunc_(&ii, &ff);

	return 0;
}

