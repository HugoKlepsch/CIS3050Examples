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

#define		NDATA	3

static char *sData[NDATA] = {
		"My dog has no nose.",
		"    How does he smell?",
		"Terrible!"
	    };

/*
 * This program creates a socket and initiates a connection with the socket
 * given in the command line.  A set of messages are sent over the connection
 * and then the socket is closed, ending the connection.  The form of the
 * command line is "a.out <hostname> <portnumber>"
 */

main(argc, argv)
    int argc;
    char *argv[];
{
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;
    char aSize;
    int i;

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
	perror("opening stream socket");
	exit(1);
    }
    /* Connect socket using name specified by command line. */
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp == 0) {
	fprintf(stderr, "%s: unknown host\n", argv[1]);
	exit(2);
    }
    bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
	perror("connecting stream socket");
	exit(1);
    }

    /* first, indicate how many elements we will send */
    aSize = NDATA;

    if (write(sock, &aSize, sizeof(char)) < 0)
	perror("writing number of elements on stream socket");
    for (i = 0; i < NDATA; i++) {
	aSize = (char) strlen(sData[i]);
	if (write(sock, &aSize, sizeof(char)) < 0)
	    perror("writing data size on stream socket");
	if (write(sock, sData[i], aSize * sizeof(char)) < 0)
	    perror("writing data on stream socket");
    }
    close(sock);
}

