#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
 * This program attaches to a UNIX domain socket given on the command line.
 * Anything typed on standard input is then send to the server.
 */

int
main(argc, argv)
    int argc;
    char *argv[];
{
    char linebuffer[BUFSIZ];
    struct sockaddr_un server;
    int sock;

    if (argc != 2) {
        fprintf(stderr, "Expecting the name of the socket on command line\n");
        exit(1);
    }

    /* Create socket */
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("opening stream socket");
        exit(1);
    }

    /* Connect socket using name specified by command line. */
    server.sun_family = AF_UNIX;
    strncpy(server.sun_path, argv[1], sizeof(server.sun_path));

    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        perror("connecting stream socket");
        exit(1);
    }


    /* loop until we are told to quit (or the input closes) */
    printf("Now copying stdin into the socket.  Type \"quit\" to stop\n>");
    fflush(stdout); /* force > prompt to screen */

    while (fgets(linebuffer, BUFSIZ, stdin) != NULL) {
        if (write(sock, linebuffer, strlen(linebuffer)) < 0)
            perror("writing data to server");

        if (strcmp(linebuffer, "quit\n") == 0)
            break;
        printf(">");
        fflush(stdout); /* force > prompt to screen */
    }

    close(sock);
}

