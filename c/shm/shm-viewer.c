/**
 * Prints out a mapped region in a nice format
 */
 
#include <stdio.h>
#include <errno.h>
#include <string.h> /* for strerror() */
#include <stdlib.h> /* for strtol() */
#include <fcntl.h> /* for open() */
#include <ctype.h> /* for isblank() */
#include <unistd.h> /* for isatty() */
#include <sys/stat.h> /* for struct stat */
#include <sys/shm.h>

/* this must be an even number for things to work correctly */
#define	BYTES_PER_LINE	16

#define	HALF_BYTES_PER_LINE	(BYTES_PER_LINE / 2)

#define	DEFAULT_MAPSIZE 512

/** forward references */
static int
dump_map(FILE *fp,
		unsigned char *startAddress, unsigned char *baseAddress,
		size_t nBytesToPrint);

/**
 * mainline -- dump out the indicated region of the indicated file
 */
int
main(int argc, char **argv)
{
	void *shmRegion;
	key_t shmKey;
	int shmID;
	size_t shareSize;
	int shmFlags;
	off_t mapoffset = 0;
	int status;

	if (argc < 3) {
		fprintf(stderr, "Usage:\n%s <length> <key> [ <map offset> ]\n", argv[0]);
		fprintf(stderr, "\n"
				"Required arguments are the shared memory length and the key "
				"used to get the shared memory in the companion writer.\n");
		fprintf(stderr, "\n"
				"The option <map offset> argument skips printing the first"
				"bytes of the printed map\n");

		return -1;
	}
	
	shareSize = (size_t) atoi(argv[1]);
	shmKey = (key_t) atoi(argv[2]);
	if (argc > 3) {
		mapoffset = atoi(argv[3]);
	}

	printf("Attempting to use shared memory region with key %ld of size %ld\n",
			(long) shmKey, (long) shareSize);



	/* don't create (the writer does this) but allow reading */
	shmFlags = 0400;

	/** set up the share, creating it if need be */
	shmID = shmget(shmKey, shareSize, shmFlags);
	if (shmID < 0) {
		fprintf(stderr, "Error: shmget(%ld, %ld, 0%lo) failed: %s\n",
				(long) shmKey, (long) shareSize, (long) shmFlags, strerror(errno));
		return -1;
	}

	printf("Got shared memory block of %ld bytes, ID: %d\n",
			(long) shareSize, shmID);


	/** now attach the shared memory to this process, where the kernel sees fit */
	shmRegion = (unsigned char *) shmat(shmID, NULL, 0);
	if (shmRegion == NULL) {
		fprintf(stderr, "Shared memory mapping of ID '%d' failed: %s\n",
				shmID, strerror(errno));
		return -1;
	}


	/** dump the given region */
	printf("Dumping %ld bytes mapped to address %p, starting at offset %ld\n",
			(long) shareSize, shmRegion, (long) mapoffset);

	/**
	 * passing the region + offset here to get the reported addresses to
	 * be relative to the starting location of the map region, but shifted
	 * so that if we have skipped the first few bytes via setting an offset,
	 * then everything still works fine
	 */
	status = dump_map(stdout, shmRegion, (shmRegion + mapoffset), shareSize);

	/** detatch the shared segment*/
	shmdt(shmRegion);

	/* exit indicating our success */
	exit(status >= 0 ? 0 : status);
}



/**
 * If you pass the same pointer for baseAddress and startAddress, it will
 * startAddress its numbering from zero.  If you pass "((void *) 0)"
 * frequently known as "NULL", then it will tell you the absolute
 * memory addresses of the bytes it prints.
 *
 * The value "startAddress" should be the real data you want to print;
 * nBytesToPrint is the number of bytes starting at that location.
 *
 * Note that if you are getting a core dump in here, it most likely
 * means that the pointer value you passed for startAddress is not a
 * valid pointer value.
 */
static int
dump_map(FILE *fp,
		unsigned char *startAddress,
		unsigned char *baseAddress,
		size_t nBytesToPrint)
{
	unsigned char buffer[BYTES_PER_LINE];
	size_t bytesThisLine, bytesRemain;
	size_t i, j;

	i = 0;
	while (i < nBytesToPrint)
	{
		bytesRemain = nBytesToPrint - i;
		/** figure out how many bytes to print */
		bytesThisLine = (bytesRemain < BYTES_PER_LINE)
					? bytesRemain : BYTES_PER_LINE;

		/** copy the data into our working buffer */
		memcpy(buffer, &startAddress[i], bytesThisLine);

		fprintf(fp, "0x%08lx", ((unsigned long) (startAddress - baseAddress)) + i);

		/** print the hex values */
		for (j = 0; j < bytesThisLine; j++)
		{
			if (j == HALF_BYTES_PER_LINE)
				fprintf(fp, " ");
			fprintf(fp, " %02x", startAddress[i+j]);
		}

		/** pad if we are short */
		for ( ; j < BYTES_PER_LINE; j++)
		{
			if (j == HALF_BYTES_PER_LINE)
				fprintf(fp, " ");
			fprintf(fp, "   ");
		}

		/** print as chars */
		fprintf(fp, " ");
		for (j = 0; j < bytesThisLine; j++)
			fprintf(fp, "%c", isprint(startAddress[i+j]) ? startAddress[i+j] : '.');

		fprintf(fp, "\n");

		/** update i by the amount we have printed */
		i += bytesThisLine;
	}

	if (ferror(fp)) return -1;

	return 1;
}

