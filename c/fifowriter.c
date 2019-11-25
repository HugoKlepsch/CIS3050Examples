#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h> /* for getlogin() */
#include <fcntl.h> /* for open() */
#include <sys/types.h> /* for mkfifo */
#include <sys/stat.h> /* for mkfifo */

/*
 * Creates a FIFO, and copies everything from standard input into it.
 * Expects there to be a companion fiforeader to read the data.
 */


/* copies everything from stdin to the file descriptor */
int
copyAllLinesToFD(int destFD)
{
    /** where does BUFSIZ come from?  /usr/include . . . */
    char buffer[BUFSIZ];

    while (fgets(buffer, BUFSIZ, stdin) != NULL) {
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
    char fifopath[20];
    int fifoFD;
    
    /** use mktemp() to get us an unused name in a temporary directory */
    strncpy(fifopath, "/tmp/myFIFO.XXXXXX", 20);
    if (mktemp(fifopath) == NULL) {
        fprintf(stderr, "Error: failed creating temporary name : %s\n",
                strerror(errno));
        return -1;
    }

    /*
     * create the FIFO so that it exists in the file system with perms rwxr--r-- 
     */
    if (mkfifo(fifopath, 0644) < 0) {
        fprintf(stderr, "Error: failed creating FIFO %s : %s\n",
                fifopath, strerror(errno));
        return -1;
    }

    printf("Succesfully created FIFO: %s\n", fifopath);


    /* now open the FIFO for writing */
    if ((fifoFD = open(fifopath, O_WRONLY)) < 0) {
        fprintf(stderr, "Error: failed opening FIFO %s : %s\n",
                fifopath, strerror(errno));
        return -1;
    }

    if (isatty(0))
        printf("Data from stdin now being copied to FIFO.  Use ^D to quit\n");

    /* now simply copy data */
    if (copyAllLinesToFD(fifoFD) < 0) {
        fprintf(stderr, "Error: failed copying data to FIFO %s : %s\n",
                fifopath, strerror(errno));
        return -1;
    }

    /* close the FIFO */
    close(fifoFD);

    /* remove the FIFO from the file system */
    if (unlink(fifopath) < 0) {
        fprintf(stderr, "Error: could not unlink FIFO %s : %s\n",
                fifopath, strerror(errno));
        return -1;
    }

    return 0;
}
