/*
 * This code adapted from the Berkeley Systems Introduction
 * to IPC tutorial, Unix Programmer's Manual Supplementary
 * Documents 1
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
 * This program creates a socket and then begins an infinite loop.  Each time
 * through the loop it accepts a connection and prints out messages from it.
 * When the connection breaks, or a termination message comes through, the
 * program accepts a new connection.
 *
 * To generate the messages for this server, use the code in tcp_demo_client.c
 */

main(argc, argv)
    int argc;
    char *argv[];
{
    int sock, length;
    struct sockaddr_in server;
    int msgsock;
    char buf[1024];
    char aSize;
    int rval;
    int nElements, i;

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
	perror("opening stream socket");
	exit(1);
    }
    /* Name socket using wildcards */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = 0;
    if (bind(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
	perror("binding stream socket");
	exit(1);
    }
    /* Find out assigned port number and print it out */
    length = sizeof(server);
    if (getsockname(sock, (struct sockaddr *) &server, &length)) {
	perror("getting socket name");
	exit(1);
    }
    printf("Socket has port #%d\n", ntohs(server.sin_port));

    /* Start accepting connections */
    listen(sock, 5);
    do {
	msgsock = accept(sock, (struct sockaddr *) 0, (int *) 0);
	if (msgsock == -1)
	    perror("accept");
	else do {

	    rval = read(msgsock, &aSize, sizeof(char));
	    if (rval < 0) {
		perror("reading number stream message elements");
		continue;
	    }
	    if (rval > 0) {
		nElements = aSize;
		printf("  Receiving message with %d elements\n", nElements);
		for (i = 0; i < nElements; i++) {

		    if ((rval = read(msgsock, &aSize, sizeof(char))) < 0) {
			perror("reading stream message size");
		    }

		    bzero(buf, sizeof(buf));
		    if ((rval = read(msgsock, buf, aSize)) < 0) {
			perror("reading stream message");
		    }
		    /* print out this line */
		    if (rval != 0)
			printf("  -->%s\n", buf);
		}
	    }
	    if (rval == 0)
		printf("Ending connection\n");
	} while (rval != 0);
	close(msgsock);
    } while (1);
    /*
     * Since this program has an infinite loop, the socket "sock" is
     * never explicitly closed.  However, all sockets will be closed
     * automatically when a process is killed or terminates normally.
     */
}

