/**
 * Companion to "flock.c" but based on record locking, as opposed to
 * whole file locking
 */


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#include <sys/file.h>
#include <fcntl.h>

/* print command line option help */
static void
printHelpAndExit(char *progname, char *errormessage)
{
    if (errormessage != NULL)
        fprintf(stderr, "Error: %s\n\n", errormessage);

    fprintf(stderr, "%s <datafile> [ <commandfile> ]\n", progname);
    fprintf(stderr, "    Commands come in the form:\n");
    fprintf(stderr, "        OPN OFF LEN SLEEP\n");
    fprintf(stderr, "    where:\n");
    fprintf(stderr, "        OPN (operation) is one of:\n");
    fprintf(stderr, "           L : lock of region, waiting for availbility \n");
    fprintf(stderr, "           T : lock of region or immediate return if locked\n");
    fprintf(stderr, "           E : exampine lock state\n");
    fprintf(stderr, "           U : unlock region\n");
    fprintf(stderr, "        OFF, and LEN are the offset and length in bytes\n");
    fprintf(stderr, "        SLEEP is the time to sleep in seconds\n");
    fprintf(stderr, "    If <commandfile> is not given, stdin is read\n");

    exit (-1);
}

/* wrap ctime(3) to get the current time in a string */
static char *
curtimeString()
{
    time_t curtime;

    /** get the time right now in seconds since the epoch */
    curtime = time(NULL);

    /* ctime(3) inconveniently takes the address of a time value */
    return ctime(&curtime);
}

int
doLockAction(int lockFD, int operation, off_t offset, off_t length, int sleepTime)
{
    char *operationTag;

    operationTag = (operation == F_ULOCK ? "UNLOCK":
                    operation == F_LOCK ? "LOCK/WAIT" :
                        operation == F_TLOCK ? "LOCK/TEST" :
                            operation == F_TEST ? "TEST" : "???");

    /** move to the right location */
    if (lseek(lockFD, offset, SEEK_SET) < 0) {
        fprintf(stderr, "PID %d: failed seeking to position %lld : %d %s\n",
                getpid(), offset, errno, strerror(errno));
        return -1;
    }

    /** perform the lock operation */
    printf("PID %ld: requesting %s lock at %s",
            (long) getpid(), operationTag, curtimeString());

    if (lockf(lockFD, operation, length) < 0) {
        fprintf(stderr, "PID %d: failed     on %s lock of %lld bytes at %lld : %d %s\n",
                getpid(), operationTag,
                length, offset, errno, strerror(errno));
        return -1;
    } else {
        printf("PID %ld: granted    %s at %lld for %lld bytes at %s",
                (long) getpid(), operationTag, offset, length,
                curtimeString());
    }

    if (sleepTime > 0) {
        printf("PID %ld: sleeping   %ds at %s",
                (long) getpid(), sleepTime, curtimeString());
        (void) sleep(sleepTime);
    }

    return 0;
}

int
main(int argc, char *argv[])
{
    char commandLine[BUFSIZ];
    FILE *commandFP = stdin;
    int lockFD, nItemsRead, sleepTime;
    long offset, length;
    char operation, type;

    /** check that the arguments are correct */
    if (argc != 2 && argc != 3)  printHelpAndExit(argv[0], "incorrect arguments");

    if (argc == 3) {
        if ((commandFP = fopen(argv[2], "r")) == NULL) {
            fprintf(stderr, "Cannot open command file '%s' : %s\n",
                    argv[2], strerror(errno));
            exit (-1);
        }
    }

    /**
     * Open the file to be locked.
     * The file needs to be open for writing if we want to perform
     * commands O_LOCK or O_TLOCK (i.e.; any locking)
     */
    lockFD = open(argv[1], O_RDWR);
    if (lockFD < 0) {
        fprintf(stderr, "Failed opening '%s' : %s\n", argv[1], strerror(errno));
        exit (-1);
    }
    printf("Processing locks on '%s'\n", argv[1]);

    /** print first prompt */
    if (isatty(0))
        printf("CMD> ") >= 0 && fflush(stdout);

    /**
     * loop reading commands
     * we use fgets() to ensure that we are reading whole lines
     */
    while (fgets(commandLine, BUFSIZ, commandFP) != NULL) {

        nItemsRead = sscanf(commandLine, "%c %ld %ld %d", 
                &operation, &offset, &length, &sleepTime);
        if (nItemsRead < 3
                    || strchr("LTEU", toupper(operation)) == NULL)
            fprintf(stderr, "Invalid operation - need \"OPN OFF LEN SLEEP\"\n");
        else {
            if (nItemsRead == 3)
                sleepTime = 0;

            printf("CMD %c %2d %2ld %2ld\n",
                    toupper(operation), toupper(type),
                    offset, length);

            switch(toupper(operation)) {
            case 'L':
                doLockAction(lockFD, F_LOCK, offset, length, sleepTime);
                break;
            case 'T':
                doLockAction(lockFD, F_TLOCK, offset, length, sleepTime);
                break;
            case 'E':
                doLockAction(lockFD, F_TEST, offset, length, sleepTime);
                break;
            case 'U':
                doLockAction(lockFD, F_ULOCK, offset, length, sleepTime);
                break;
            }
        }

        /* prompt again */
        if (isatty(0))
            printf("CMD> ") >= 0 && fflush(stdout);
    }

    close(lockFD);

    exit(0);
}
