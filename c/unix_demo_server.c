#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

/*
 * This program creates a socket and then begins an infinite loop.  Each time
 * through the loop it accepts a connection and prints out messages from it.
 * When the connection breaks, or a termination message comes through, the
 * program accepts a new connection.
 *
 * To generate the messages for this server, use the code in unix_demo_client.c
 */

/* static to be shared with the signal handler */
static char sSocketPath[24];

/** remove the socket and exit */
void
signalHandler()
{
    fprintf(stderr, "Cleaning up on signal\n");
    unlink(sSocketPath);
    exit(1);
}

int
main(argc, argv)
    int argc;
    char *argv[];
{
    struct sockaddr_un server; /* UNIX domain socket address structure */
    int sock, msgsock; /* our sockets (one for listening, one for the message) */

    char messagebuffer[1024]; 
    int rval;
    int keepGoing;

    /** get us a name to use for the socket */
    strncpy(sSocketPath, "/tmp/myUDS.XXXXXX", 20);
    if (mktemp(sSocketPath) == NULL) {
        perror("getting temp name for socket");
        exit(1);
    }

    /* register the signal handler for HUP, INTR, and TERM */
    signal(SIGHUP, signalHandler); /* signal 1 */
    signal(SIGINT, signalHandler); /* signal 2 */
    signal(SIGTERM, signalHandler); /* signal 15 */

    /* Create socket */
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("opening stream socket");
        exit(1);
    }

    /* Name socket using filename */
    server.sun_family = AF_UNIX;
    strncpy(server.sun_path, sSocketPath, sizeof(server.sun_path));

    /* bind the socket to the filesystem */
    if (bind(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("binding stream socket");
        exit(1);
    }

    printf("Socket has name %s\n", server.sun_path);


    /* Start accepting connections */
    listen(sock, 5);

    /* loop until told to "quit" */
    keepGoing = 1;
    do {
        /* we get a _second_ socket to read the message from */
        msgsock = accept(sock, NULL, NULL);
        if (msgsock == -1)
            perror("accept");
        else do {

            memset(messagebuffer, 0, sizeof(messagebuffer));
            /*
             * if we want to use fancier socket tools, we can use recv()
             * instead of read() here, but for simple data processing,
             * either will do
             */
            rval = read(msgsock, messagebuffer, sizeof(messagebuffer));
            if (rval < 0) {
                perror("reading stream message");
            } else if (rval == 0) {
                printf("Ending connection\n");
            } else {
                printf("got -->%s", messagebuffer);

                if (strcmp(messagebuffer, "quit\n") == 0)
                    keepGoing = 0;
            }

        } while (rval > 0 && keepGoing);

        /* close the message socket and go back to waiting on the stream socket */
        close(msgsock);

    } while (keepGoing);

    printf("Told to quit, so cleaning up...\n");
    close(sock);
    unlink(sSocketPath);

    return 0;
}

