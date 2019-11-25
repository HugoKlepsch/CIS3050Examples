#include <stdio.h>
#include <errno.h>
#include <string.h> /* for strerror() */
#include <stdlib.h> /* for strtol() */
#include <fcntl.h> /* for open() */
#include <ctype.h> /* for isblank() */
#include <unistd.h> /* for isatty() */
#include <sys/stat.h> /* for struct stat */
#include <sys/shm.h>


#define	DEFAULT_SHARESIZE 512


/** forward references */
static int commandLineMapEditor(unsigned char *shmRegion);


/**
 * Update data in region.
 *
 * Not much to it -- just copy the data to the memory location
 */
void
updateMemory(unsigned char *shmRegion, off_t offset, char *text, size_t textlen)
{
	int i;

	for (i = 0; i < textlen; i++)
		shmRegion[offset + i] = text[i];
}


int
main(int argc, char **argv)
{
	unsigned char *shmRegion;
	int shmID;
	key_t shmKey;
	int shmFlags;
	size_t shareSize = DEFAULT_SHARESIZE;
	struct shmid_ds shmCtrlBuf;
	int status;


	if (argc > 1) {
		shareSize = atoi(argv[1]);
		if (shareSize <= 0) {
			fprintf(stderr, "Cannot use a share of less than 1 byte\n");
			return -1;
		}
	}

	if (argc > 2) {
		shmKey = atoi(argv[2]);
		printf("Key set to %ld\n", (long) shmKey);
	} else {
		/** create a default key based on ftok */
		shmKey = ftok(getenv("HOME"), getuid());
		printf("Calculating default key using home directory and user ID:\n"
				"  ftok('%s', %d)\n",
					getenv("HOME"), getuid());
		printf("Using default key of %ld\n", (long) shmKey);
	}



	/* create if need be, and allow read and write by owner */
	shmFlags = IPC_CREAT | 0400 | 0200;

	/** set up the share, creating it if need be */
	printf("Calling for shared memory with possible creation:\n"
			"	shmget(%ld, %ld, 0%lo)\n",
					(long) shmKey, (long) shareSize, (long) shmFlags);
	shmID = shmget(shmKey, shareSize, shmFlags);
	if (shmID < 0) {
		fprintf(stderr, "Error: shmget(%ld, %ld, 0%lo) failed: %s\n",
				(long) shmKey, shareSize, (long) shmFlags, strerror(errno));
		return -1;
	}

	printf("Got shared memory block of %ld bytes, ID %d using key %ld\n",
			(long) shareSize, shmID, (long) shmKey);


	/** now attach the shared memory to this process, where the kernel sees fit */
	printf("Calling for attachment of shared memory region ID %ld\n"
			"	shmat(%ld, NULL, 0)\n", (long) shmID, (long) shmID);
	shmRegion = (unsigned char *) shmat(shmID, NULL, 0);
	if (shmRegion == NULL) {
		fprintf(stderr, "Shared memory mapping of ID '%d' failed: %s\n",
				shmID, strerror(errno));
		return -1;
	}

	printf("Editing %ld bytes of shared memory mapped to address %p\n",
			(long) shareSize, shmRegion);



	/** now do the edits */
	if ((status = commandLineMapEditor(shmRegion)) < 0) {
		fprintf(stderr, "Error editing shared memory region!\n");
	}

	/** detatch the shared segment*/
	shmdt(shmRegion);

	/** ask for the shared segment to be destroyed */
	memset(&shmCtrlBuf, 0, sizeof(shmCtrlBuf));
	if (shmctl(shmID, IPC_STAT, &shmCtrlBuf) < 0) {
		fprintf(stderr, "Error: cannot stat shared memory ID %d: %s\n",
				shmID, strerror(errno));
		return -1;
	}

	printf("Shared memory UID is %d, our EUID is %d\n",
			shmCtrlBuf.shm_perm.uid, geteuid());

	if (shmctl(shmID, IPC_RMID, &shmCtrlBuf) < 0) {
		fprintf(stderr, "Error: Failed removing shared memory with ID %d: %s\n",
				shmID, strerror(errno));
		return -1;
	}

	/* exit indicating our success */
	exit(status >= 0 ? 0 : status);
}


/**
 * Loop on input, placing strings into the shared memory region
 */
static int
commandLineMapEditor(unsigned char *shmRegion)
{
	char commandLine[BUFSIZ];
	long offset;
	int textlen, nEdits = 0;
	char *text;

	/** print first prompt */
	if (isatty(0)) {
		printf("Command structure:\n<N> <TEXT>\n"
				"Text value '<TEXT>' will be placed at offset <N>.\n"
				"(Offsets are in hexadecimal.)\n");
		printf("CMD> ");
		fflush(stdout);
	}

	/**
	 * loop reading commands
	 * we use fgets() to ensure that we are reading whole lines
	 */
	while (fgets(commandLine, BUFSIZ, stdin) != NULL) {

		/** if told to quit, get out of here */
		if (commandLine[0] == 'q' || commandLine[0] == 'Q')
			break;


		/* the number leading the string is the offset, and the rest is text */
		offset = strtol(commandLine, &text, 16);
		if (text == commandLine) {
			fprintf(stderr, "Could not find an offset number leading input: %s",
					commandLine);
		} else {

			/* eat any whitespace at the beginning of the text */
			while (isblank(*text))
				text++;

			/** make sure that there is something left */
			textlen = strlen(text);
			if (textlen == 0) {
				fprintf(stderr, "No text after offset given in input: %s",
					commandLine);
			} else {
				printf("Updating %d bytes at offset %ld\n", textlen, offset);

				updateMemory(shmRegion, offset, text, textlen);
				nEdits++;
			}
		}

		/* prompt again */
		if (isatty(0)) {
			printf("CMD> ");
			 fflush(stdout);
		}
	}

	return nEdits;
}

