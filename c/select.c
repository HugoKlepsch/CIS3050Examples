#include <stdio.h>
#include <sys/select.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <ctype.h>  /* for iscntrl() */
#include <stdlib.h> /* for exit() */
#include <unistd.h> /* for read() */

#if ! defined(STDIN_FILENO)
#   define STDIN_FILENO 0
#endif

#define NSECONDS_PAUSE  3

/* static variable to carry the original termio data */
static struct termios termOrigState;


/* put us back in "cooked" mode */
void
cookedMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &termOrigState) < 0)
    {
        fprintf(stderr, "Error from tcsetattr() : %s\n", strerror(errno));
        exit (-1);
    }
}

void
rawMode()
{
    struct termios termRawState;

    /** get the original terminal settings */
    if (tcgetattr(STDIN_FILENO, &termOrigState) < 0)
    {
        fprintf(stderr, "Error from tcsetattr() : %s\n", strerror(errno));
        exit (-1);
    }

    /** register switch back to cooked mode at exit */
    atexit(cookedMode);

    /** copy contents of original state setup to new struct termios */
    termRawState = termOrigState;

    /**
     * set ourselves up with lots of flag changes to get ourselves into
     * raw mode: we are bitwise ANDing with the NOT (bit inversion) of
     * the OR of ECHO (print to terminal) and ICANON (canonical input
     * buffering).  So: get the ECHO and ICANON bits ORed into a bitmask,
     * invert it, and then use the resulting AND mask to keep everything
     * but these bits.
     */
    termRawState.c_lflag &= ~(ECHO | ICANON);

    /** use the new termios to go to raw mode */
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &termRawState) < 0)
    {
        fprintf(stderr, "Error from tcsetattr() : %s\n", strerror(errno));
        exit (-1);
    }
}

int
main(int argc, char **argv)
{
    struct timeval timeout;
    fd_set inReadySet;
    int fdToExamine, maxFDtoExamine;
    int readStatus, nReady, exitStatus = (-1);
    int keepGoing = 1;
    int useRawmode = 0;
    unsigned char ch = 0;

    if (argc > 1 && strcmp(argv[1], "-r") == 0) {
        useRawmode = 1;
    }

    printf("Select demo:\n");
    printf("Will print keys typed but also keep typing dots every %d seconds\n",
            NSECONDS_PAUSE);

    /* we will only examine stdin */
    fdToExamine = STDIN_FILENO;

    /* one larger than the max FD to examine (0) */
    maxFDtoExamine = fdToExamine + 1;

    if (useRawmode) {
        /* turn off buffering and echo on input */
        rawMode();
        printf("raw mode ON - keys will be processed as typed\n");
    } else {
        printf("raw mode OFF - keys will be processed after [ENTER]\n");
    }

    readStatus = 1;
    while (keepGoing) {
        memset(&timeout, 0, sizeof(struct timeval));
        timeout.tv_sec = NSECONDS_PAUSE;

        FD_ZERO(&inReadySet);
        FD_SET(fdToExamine, &inReadySet); /* examine fd 0 (stdin) */

        nReady = select(maxFDtoExamine, 
                &inReadySet,   /* set of read file descriptors */
                NULL,       /* set of write file descriptors */
                NULL,       /* set of exception file descriptors */
                &timeout);
        if (nReady < 0) {
            fprintf(stderr, "Error from select() : %s\n", strerror(errno));
            break;
        }

        //printf("nReady = %d\n", nReady);
        if (nReady > 0) {

            /** now examine a particular FD - in our case, only one */
            if (FD_ISSET(fdToExamine, &inReadySet)) {

                /**
                 * stdin is ready for reading, so get a byte - we 
                 * are assured that we will not block
                 */
                readStatus = read(fdToExamine, &ch, sizeof(unsigned char));
                if (readStatus < 0) {
                    fprintf(stderr, "Error from read() : %s\n", strerror(errno));
                    break;
                }

                /** if we hit EOF, break */
                if (readStatus == 0)
                    exitStatus = keepGoing = 0;
                else {

                    if (iscntrl(ch))
                        printf("ch=%d\n", ch);
                    else
                        printf("ch=%d '%c'\n", ch, ch);

                    if (ch == 'q' || ch == 'Q') {
                        exitStatus = keepGoing = 0;
                    }
                }
            }
        }

        /* print a dot */
        printf(".\n");
    }

    /** go back to cooked mode */
    if (useRawmode) {
        cookedMode();
    }

    printf("\ndone!\n");


    return exitStatus;
}
