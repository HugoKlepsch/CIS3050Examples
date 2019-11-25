/*
 * A Very Simple Shell
 *
 * This program parses input using strtok, and tries to run
 * the results
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define	MAXLINE	2048
#define	MAXARGV	128


int
main()
{
	/* we will let the users have MAXLINE (2048 chars) of typing */
	char line[MAXLINE];

		/* we will assume that there will be fewer than 126 args (excluding
		 * the program name and the NULL we need at the end */
	char *argv[MAXARGV];	

		/* we will use these while tokenizing */
	char *curtok, *tokargptr;
	int exitting = 0;
	int status = 0;
	int argc;
	int pid, exitingPid;
	int i;


	while ( ! exitting )
	{
		/* print a prompt (if standard input (fd 0) is a tty) */
		if (isatty(0))
		{
			printf("vssh $ ");
			fflush(stdout);
		}

		/* get a line - exiting on EOF */
		if (fgets(line, MAXLINE, stdin) == NULL)
			break;

		/* break line into pieces - ignore if more than 128 tokens */
		tokargptr = line;
		argc = 0;
		while ((argc < (MAXARGV-1))
				&& ((curtok = strtok(tokargptr, " \t\n")) != NULL))
		{
			tokargptr = NULL;
			argv[argc++] = curtok;
		}
		/** put a NULL at the end of the list */
		argv[argc] = NULL;



		/** if we got a command, run it */
		if (argc > 0)
		{
			/** if we are exitting, then short-circuit the loop */
			if (strcmp(argv[0], "exit") == 0)
			{
				exitting = 1;
				continue;
			}


			/* print the line out */
			printf("cmd: %s", argv[0]);
			for (i = 1; i < argc; i++)
			{
				printf(" '%s'", argv[i]);
			}
			printf("\n");


			/* now run the command */
			if ((pid = fork()) == 0)
			{
				/* in the child, so run exec */
				execvp(argv[0], argv);

				perror("failed running exec()");
				exit (-1);
			}

			if (pid < 1)
			{
				perror("failed running fork()");
				exit (-1);
			}


			/** wait for our child command */
			while ((exitingPid = wait(&status)) != pid) {
				printf("Looking for pid %d, saw exit of %d\n",
						pid, exitingPid);
			}

			if (WIFEXITED(status))
			{
				if (WEXITSTATUS(status) != 0)
					printf("Child (%d) exitted with %d\n",
							pid, WEXITSTATUS(status));
			}
			else
				printf("Child (%d) crashed!\n", pid);

		}
	}

	/** exit with the status of the last command */
	exit(WEXITSTATUS(status));
}

