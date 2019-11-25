#include <stdio.h>
#include <stdarg.h>

/*
 * ---------------------------------------------
 * roll our own printf-like function
 */
void
ourOwnPrintf(const char *fmt, ...)
{
    va_list vargs; /* the list of arguments */

    printf(">>>");

    /* set up argument list */
    va_start(vargs, fmt);

    /* pass argument list to printf-style handler */
    vfprintf(stdout, fmt, vargs);

    /* clean up from traversal */
    va_end(vargs);
}


int
main(int argc, char **argv)
{
    printf("About to try our own printf:\n");

    ourOwnPrintf("How did it go?\n");
    ourOwnPrintf("%d %lf 0x%x '%s'\n", 1, 2.0, 3, "four");
    
    printf("DONE!\n");

    return 0;
}

