/**
 *
 * Adapted from t_flock.c by Michael Kerrisk, 2019, from his example in
 * "The Linux Programming Interface", No Starch Press, 2010.
 * ISBN 978-1-59327-220-3.
 *
 * This program is free software. You may use, modify, and redistribute it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 or (at your option) any
 * later version. This program is distributed without any warranty.
 */


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <sys/file.h>
#include <fcntl.h>

/* print command line option help */
static void
printHelpAndExit(char *progname)
{
    fprintf(stderr, "%s <file> <lock> [sleep-time]\n", progname);
    fprintf(stderr, "    'lock' is 's' (shared) or 'x' (exclusive)\n");
    fprintf(stderr, "        optionally followed by 'n' (nonblocking)\n");
    fprintf(stderr, "    'sleep-time' specifies time to hold lock\n");
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
main(int argc, char *argv[])
{
    const char *lname;
    int fd, lock, timeToSleep;

    /** check that the arguments are correct */
    if (argc < 3 || strncmp(argv[1], "-h", 2) == 0
            || strchr("sx", argv[2][0]) == NULL) {
        printHelpAndExit(argv[0]);
    }

    if (argc == 4)
        timeToSleep = atoi(argv[3]);
    else
        timeToSleep = 10;

    /** determine lock type from first letter, and optionally second */
    lock = (argv[2][0] == 's') ? LOCK_SH : LOCK_EX;
    if (argv[2][1] == 'n')
        lock |= LOCK_NB;

    /** open the file to be locked */
    fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit (-1);
    }

    /** set lock name variable to report type of lock we are using */
    lname = (lock & LOCK_SH) ? "LOCK_SH" : "LOCK_EX";

    /** print out the identity and time, noting that ctime(3) adds a \n */
    printf("PID %ld: requesting %s at %s", (long) getpid(), lname, curtimeString());

    if (flock(fd, lock) == -1) {
        if (errno == EWOULDBLOCK)
            fprintf(stderr, "PID %ld: already locked - bye!", (long) getpid());
        else {
            fprintf(stderr, "PID %d got error on lock : %s\n",
                    getpid(), strerror(errno));
        }
        exit (-1);
    }

    /** acknowledge our success in getting the lock */
    printf("PID %ld: granted    %s and sleeping %ds at %s",
            (long) getpid(), lname, timeToSleep, curtimeString());

    /** go to sleep for the specified period */
    sleep(timeToSleep);

    /** let the world know that we are back from the sleep */
    printf("PID %ld: releasing  %s at %s\n",
            (long) getpid(), lname, curtimeString());

    /** unlock the file */
    if (flock(fd, LOCK_UN) == -1) {
        fprintf(stderr, "PID %d got error on unlock : %s\n",
                    getpid(), strerror(errno));
        exit (-1);
    }

    close(fd);

    exit(0);
}
