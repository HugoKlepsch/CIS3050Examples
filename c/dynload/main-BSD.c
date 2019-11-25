#include <errno.h> 
#include <dlfcn.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 


# define	SHARED_OBJ		void *

	/*
	 * The BSD family defines dlfunc() to return a function pointer
	 * specific accessor specifically for this case
	 */
# define	DL_FUNC_TYPE	dlfunc_t


int
main(int argc, char *argv[])
{
	char *libname, *funcname;
	SHARED_OBJ lib;
	DL_FUNC_TYPE dlfunction;
	int result;

	libname = "./lib/libTbsd.so";


	/*
	 * Open the dynamic library, and prepare for calls.  Force
	 * execution of dependency checking (dependencies on other
	 * libs), as opposed to RTLD_LAZY, which will do this only
	 * as each call is done, and may therefore raise errors if
	 * dependencies on other libraries occurs
	 */
	dlib = dlopen(libname, RTLD_NOW);

	/*
	 * If we didn't get a handle back, then use dlerror()
	 * to report what happened
	 */
	if (dlib == NULL)
	{
		fprintf(stderr, "Cannot open dynamic library '%s' : %s\n",
				libname, dlerror());
		exit (-1);
	}


	/**
	 * "simple" function
	 *
	 * Get a function-pointer type argument for this symbol;
	 * if we were accessing data only, we could use dlsym
	 */
	funcname = "simple";

	dlfunction = dlfunc(dlib, funcname);
	if (dlfunction == NULL)
	{
		fprintf(stderr, "Cannot locate function symbol '%s' : %s\n",
				funcname, dlerror());
		exit(-1);
	}

	printf("Calling function %s() in library %s\n", funcname, libname);
	printf(">>>\n");

	/**
	 * call the function -- note that the arg list we pass here cannot
	 * be checked by the compiler; it is up to us to know how to call
	 * the function correctly
	 */
	{
		int (*fPtr)();

		fPtr = (int (*)()) dlfunction;
		result = (*fPtr)();
	}

	printf("<<<\n");
	printf("Back from call to function %s(), got result %d\n",
			funcname, result);
	





	/**
	 * "another" function
	 *
	 * Get a function-pointer type argument for this symbol;
	 * if we were accessing data only, we could use dlsym
	 */
	funcname = "another";

	/**
	 * +++ do the same thing for function "another", but now
	 * we need to pass more arguments
	 */




	/*
	 * Clean up and close the library
	 */
	dlclose(dlib);
}
