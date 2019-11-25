/** ------------------------------------------------------------
 ** Concatenate a number of strings together into a newly
 ** allocated buffer
 ** ------------------------------------------------------------
 ** $Id: strconcat.c 11 2008-04-24 22:13:19Z andrew $
 **/

#ifndef MAKEDEPEND
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#endif

/*
 * ---------------------------------------------
 * Concatenate a set of strings together,
 * malloc'ing memory for the purpose.
 *
 * End of list is indicated by a NULL pointer argument
 * ---------------------------------------------
 */
char *
strconcat(const char *leadptr,...)
{
    va_list vargs; /* the list of arguments */
    const char *argptr;
    char *curptr;
    char *retptr = NULL;
    int curlen;
    int     total_len = 1;


    /**
     ** sum the lengths
     **/

    /* set vargs to be the argument after our last known argument */
    va_start(vargs, leadptr);
    argptr = leadptr;
    while (argptr != NULL)
    {
        total_len += strlen(argptr);

        /* pull the next argument from the list */
        argptr = va_arg(vargs, char *);
    }

    /* clean up any memory set during the argument list traveral */
    va_end(vargs);


    if ((retptr = (char *) malloc(total_len)) == NULL)
        return (NULL);

    curptr = retptr;

    /**
     ** now paste in the strings
     **/

    /* traverse the argument list again, starting at the beginning */
    va_start(vargs, leadptr);
    argptr = leadptr;
    while (argptr != NULL)
    {
        curlen = strlen(argptr);
        strncpy(curptr, argptr, total_len);
        curptr += curlen;

        /* go to next argument */
        argptr = va_arg(vargs, char *);
    }
    /* clean up from traversal */
    va_end(vargs);


    return (retptr);
}


int
main(int argc, char **argv)
{
    char *result;


    result = strconcat("this", " ", "is", " ", "a", " ", "test", NULL);
    printf("First result: '%s'\n", result);
    free(result);

    result = strconcat("hello", NULL);
    printf("Second result: '%s'\n", result);
    free(result);

    printf("About to try with an unterminated sequence!\n");
    result = strconcat("unterminated hello");
    printf("third result: '%s'\n", result);
    free(result);

}

