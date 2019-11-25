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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
 * Define the number of tries and port number.
 */
#define	NUM_TRY		10
#define	YESNO_PORT	1979

/*
 * The main program sets up the server side for connection reception and
 * then forks off a child to act as the client. (Normally, the child
 * would be a separate program that is usually running on a different
 * machine, but for local testing, this is sufficient.)
 * The server is disagreeable, since it always answers "NO" for "YES"
 * and vice versa.
 */
main()
{
	int server_socket, request_size, i, response_len, sub_socket;
	char req[10], *response;
	struct sockaddr_in socket_name;

	/*
	 * Create the TCP sockets for the server.
	 */
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
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
	socket_name.sin_port = htons(YESNO_PORT);	/* at yesno port */
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
		socket_name.sin_port = htons(YESNO_PORT);

		/*
		 * Establish a connection with the server.
		 */
		if (connect(client_socket, (struct sockaddr *)&socket_name,
			sizeof (socket_name)) < 0) {
			fprintf(stderr, "Can't connect to server\n");
			exit(0);
		}

		/*
		 * Now, just loop around for a while, sending yes or no
		 * at random and receiving responses.
		 */
		for (i = 0; i < NUM_TRY; i++) {
			int yes, data_len;
			char *data, reply[10];

			yes = random() % 2;
			if (yes) {
				data = "YES";
				data_len = 4;
			} else {
				data = "NO";
				data_len = 3;
			}

			/*
			 * Send the data to the server.
			 */
			if (send(client_socket, data, data_len, 0) < 0) {
				fprintf(stderr, "Client send failed\n");
				exit(0);
			}

			/*
			 * and get reply.
			 * For TCP there are no record boundaries, so we
			 * must loop around until we have a reply, indicated
			 * by a null byte.
			 */
			reply_size = 0;
			do {
				if (recv(client_socket,
				    &reply[reply_size], 1, 0) != 1) {
					fprintf(stderr, "Client rcv err\n");
					exit(0);
				}
				reply_size++;
				if (reply_size >= 10) {
					fprintf(stderr, "EEK, pkt big\n");
					exit(0);
				}
			} while (reply[reply_size - 1] != '\0');

#ifdef DEBUG
			fprintf(stderr, "client reply l=%d s=%s\n",
				reply_size, reply);
#endif
			/*
			 * And check which reply we got.
			 */
			if (yes) {
				if (strcmp(reply, "NO"))
					fprintf(stderr, "Wrong reply\n");
			} else if (strcmp(reply, "YES"))
				fprintf(stderr, "Wrong reply\n");
		}

		/*
		 * And send an "X" to indicate the client is done.
		 */
		if (send(client_socket, "X", 2, 0) < 0)
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
	 * always reply disagreeably. (no for yes or yes for no)
	 */
	while (1) {
		request_size = 0;
		do {
		    if (recv(sub_socket, &req[request_size], 1, 0) != 1) {
			fprintf(stderr, "Server recv failed\n");
			exit(0);
		    }
		    request_size++;
		    if (request_size >= 10) {
			fprintf(stderr, "Server request too big\n");
			exit(0);
		    }
		} while (req[request_size - 1] != '\0');
#ifdef DEBUG
		fprintf(stderr, "server len=%d s=%s\n", request_size, req);
#endif
		if (!strcmp(req, "NO")) {
			response = "YES";
			response_len = 4;
		} else if (!strcmp(req, "YES")) {
			response = "NO";
			response_len = 3;
		} else {
#ifdef DEBUG
			fprintf(stderr, "Server is done\n");
#endif
			exit(0);
		}
		send(sub_socket, response, response_len, 0);
	}
}
