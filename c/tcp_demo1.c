/*
 * The little demo program inplements a client/server pair of processes
 * that communicate via TCP/IP. (TCP stands for Transport Control
 * Protocol and is the reliable virtual circuit protocol of the Internet Protocol
 * suite. It delivers a byte stream of data reliably from endpoint to
 * endpoint.)
 * Compile with "cc tcp_demo.c" or "cc -DDEBUG tcp_demo.c" to include
 * debugging printfs.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

/*
 * Define the number of tries and port number.
 */
#define	NUM_TRY		10
#define	SUM_PORT	2999

/*
 * The request and reply packet structures.
 * (Although there is no need to use structures, it can be convenient.
 *  However, care must be taken to ensure that the elements are tightly
 *  packed.)
 */
struct req {
	int	reqnum;
	short	add1;
	short	add2;
};

struct reply {
	int	reqnum;
	int	sum;
};

/*
 * The main program sets up the server side for connection reception and
 * then forks off a child to act as the client. (Normally, the child
 * would be a separate program that is usually running on a different
 * machine, but for local testing, this is sufficient.)
 * The server adds the two short numbers together and returns the sum as
 * a long int.
 */
main()
{
	int server_socket, request_size, i, sub_socket, sum;
	short add1, add2;
	struct req req;
	struct reply reply;
	int reqcnt = 0;
	struct sockaddr_in socket_name;
	char *cp;

	/*
	 * First, check structures are packed.
	 */
	if (sizeof (struct req) != (sizeof (int) + 2 * sizeof (short))) {
		fprintf(stderr, "Request structure not packed\n");
		exit(0);
	}
	if (sizeof (struct reply) != (2 * sizeof (int))) {
		fprintf(stderr, "Reply structure not packed\n");
		exit(0);
	}

	/*
	 * Create the TCP sockets for the server.
	 */
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	sub_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0 || sub_socket < 0) {
		fprintf(stderr, "Server can't create socket\n");
		exit(0);
	}

	/*
	 * Set up the address of the server in the socket address
	 * structure and bind it to the server's socket.
	 */
#ifdef BSD
	socket_name.sin_len = sizeof (struct sockaddr_in);
#endif
	socket_name.sin_family = AF_INET;
	socket_name.sin_addr.s_addr = INADDR_ANY;	/* From anyone */
	socket_name.sin_port = htons(SUM_PORT);		/* at sum port */
	if (bind(server_socket, (struct sockaddr *)&socket_name,
		 sizeof (socket_name)) < 0) {
		fprintf(stderr, "Server, can't bind socket address\n");
		exit(0);
	}

	/*
	 * And listen on the well known port for connection requests.
	 */
	if (listen(server_socket, 1) < 0) {
		fprintf(stderr, "Listen for connection\n");
		exit(0);
	}

#ifdef DEBUG
	fprintf(stderr, "Start the client by forking a child\n");
#endif
	/*
	 * Fork off the child to act as the client, while the main()
	 * parent process acts as the server.
	 */
	if (fork() == 0) {
		/* the child */
		int client_socket, reply_size;
		struct hostent *hp;

		/*
		 * Create the TCP socket for the client.
		 */
		client_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (client_socket < 0) {
			fprintf(stderr, "Client can't create socket\n");
			exit(0);
		}
	
		/*
		 * Set up the address of the server in the socket address
		 * structure. "localhost" is a generic address that always
		 * refers to the machine the process is running on.
		 * (It always is assigned IP #127.0.0.1)
		 * Copy the host address (h_addr) into the structure, plus
		 * fill in the port# and address family.
		 */
		hp = gethostbyname("localhost");
		if (hp == NULL) {
			fprintf(stderr, "Child can't get host name\n");
			exit(2);
		}
		if (hp->h_length != 4) {
			fprintf(stderr, "EEK! size should be 4\n");
			exit(0);
		}
#ifdef BSD
		socket_name.sin_len = sizeof (struct sockaddr_in);
#endif
		socket_name.sin_family = AF_INET;
		bcopy(hp->h_addr, &socket_name.sin_addr, hp->h_length);
		socket_name.sin_port = htons(SUM_PORT);

		/*
		 * Establish a connection with the server.
		 */
		if (connect(client_socket, (struct sockaddr *)&socket_name,
			sizeof (socket_name)) < 0) {
			fprintf(stderr, "Can't connect to server\n");
			exit(0);
		}

		/*
		 * Now, just loop around for a while, sending requests
		 * to add two random numbers to-gether.
		 */
		for (i = 0; i < NUM_TRY; i++) {
			int datalen;

			/*
			 * Make sure the fields of the request are in an
			 * architecture neutral form. To do this, I've
			 * used htonl() and htons() which converts the
			 * numbers to network byte order, which is
			 * Big Endian.
			 */
			req.reqnum = htonl(++reqcnt);
			add1 = random() % 32767;
			add2 = random() % 32767;
			req.add1 = htons(add1);
			req.add2 = htons(add2);

			/*
			 * Send the data to the server.
			 */
			if (send(client_socket, &req, sizeof (req), 0) < 0) {
				fprintf(stderr, "Client send failed\n");
				exit(0);
			}

			/*
			 * and get reply.
			 * For TCP there are no record boundaries, so we
			 * must loop around until we have received an entire
			 * message, based upon its length.
			 */
			datalen = sizeof (reply);
			cp = (char *)&reply;
			while (datalen > 0) {
				reply_size = recv(client_socket,cp,datalen,0);
				if (reply_size < 1) {
					fprintf(stderr, "Client rcv err\n");
					exit(0);
				}
				datalen -= reply_size;
				cp += reply_size;
			}

			/*
			 * And print out the reply.
			 * Convert the result from network byte order to
			 * this architecture's format using ntohl().
			 */
			reply.reqnum = ntohl(reply.reqnum);
			reply.sum = ntohl(reply.sum);
			printf("add1=%d add2=%d sum=%d\n", add1, add2,
				reply.sum);
		}

		/*
		 * And send a request number == -1 to indicate the client
		 * is done.
		 */
		req.reqnum = htonl(-1);
		if (send(client_socket, &req, sizeof (req), 0) < 0)
			fprintf(stderr, "Client terminate failed\n");
		exit(0);
	}

	/*
	 * Accept a connection from a client.
	 */
	if ((sub_socket = accept(server_socket, 0, 0)) < 0) {
		fprintf(stderr, "Server accept failed\n");
		exit(0);
	}

	/*
	 * Just loop around getting requests and
	 * return a reply with the sum.
	 */
	while (1) {
		int request_size, rcvsize;

		/*
		 * Since TCP virtual circuits are byte streams with no
		 * record boundaries, you must loop on recv() until an
		 * entire message is received, based upon length (or an
		 * End Of Message mark).
		 */
		request_size = sizeof (req);
		cp = (char *)&req;
		while (request_size > 0) {
		    rcvsize = recv(sub_socket, cp, request_size, 0);
		    if (rcvsize < 1) {
			fprintf(stderr, "Rcv failed\n");
			exit(0);
		    }
		    request_size -= rcvsize;
		    cp += rcvsize;
		}

		/*
		 * Convert the request from the architecture neutral form
		 * to native byte ordering.
		 */
		req.reqnum = ntohl(req.reqnum);
		if (req.reqnum == -1) {
#ifdef DEBUG
			fprintf(stderr, "Server is done\n");
#endif
			exit(0);
		}

		/*
		 * and generate the reply in an architecture neutral form.
		 */
		reply.reqnum = htonl(req.reqnum);
		add1 = ntohs(req.add1);
		add2 = ntohs(req.add2);
		sum = add1 + add2;
		reply.sum = htonl(sum);

		/*
		 * Send the reply to the client.
		 */
		send(sub_socket, &reply, sizeof (reply), 0);
	}
}
