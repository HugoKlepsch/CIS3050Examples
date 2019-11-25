#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> /* for write() */

/*
 * Opens a file expected to be a FIFO, and copies everything read there to stdout.
 */


/* copies everything from the open FILE * to the file descriptor */
int
copyAllLinesToFD(FILE *srcFP, int destFD)
{
    /** where does BUFSIZ come from?  /usr/include . . . */
    char buffer[BUFSIZ];

    while (fgets(buffer, BUFSIZ, srcFP) != NULL) {
        if (write(destFD, buffer, strlen(buffer)) < 0) {
            perror("Failed writing to FIFO");
            return -1;
        }
    }

    /** return non-negative number for success */
    return 0;
}


/* get us set up to copy the data */
int
main(int argc, char **argv)
{
    char *fifopath;
    FILE *fifoFP;

    /** get FIFO name from command line */
    if (argc != 2) {
        fprintf(stderr,
                "Expecting name of FIFO created by fifowriter as argument\n");
        return -1;
    }
    
    fifopath = argv[1];

    /* now open the FIFO for reading */
    if ((fifoFP = fopen(fifopath, "r")) == NULL) {
        fprintf(stderr, "Error: failed opening FIFO %s : %s\n",
                fifopath, strerror(errno));
        return -1;
    }

    /* now simply copy data to stdout (fd 1) */
    if (copyAllLinesToFD(fifoFP, 1) < 0) {
        fprintf(stderr, "Error: failed copying data to FIFO %s : %s\n",
                fifopath, strerror(errno));
        return -1;
    }

    /* close the FIFO */
    fclose(fifoFP);

    return 0;
}

