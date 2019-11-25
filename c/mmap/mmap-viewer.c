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
#include <sys/mman.h>

/* this must be an even number for things to work correctly */
#define	BYTES_PER_LINE	16

#define	HALF_BYTES_PER_LINE	(BYTES_PER_LINE / 2)

#define	DEFAULT_MAPSIZE 512

/** forward reference */
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
	void *mmapRegion;
	struct stat sb;
	size_t maplen;
	off_t mapoffset = 0;
	int status;
	int mapFD;

	if (argc < 2) {
		fprintf(stderr,
				"Usage:\n%s <filename> [ <start offset> [ <end offset> ] ]\n",
				argv[0]);
		fprintf(stderr,
				"\nDumps the first 2k of the indicated file\n"
				"\nIf <end offset> is negative, the entire file is dumped\n");

		return -1;
	}

	mapFD = open(argv[1], O_RDONLY);
	if (mapFD < 0) {
		fprintf(stderr, "Cannot open file '%s' for reading : %s\n",
				argv[1], strerror(errno));
		return -1;
	}

	/* find out file size */
	if (fstat(mapFD, &sb) < 0) {
		fprintf(stderr, "Cannot stat '%s' : %s\n", argv[1], strerror(errno));
		return -1;
	}

	/* if map length given, truncate only if too big, and set to file size if <0 */
	if (argc > 2) {
		int lengthGiven = atoi(argv[2]);
		if (lengthGiven < 0)
			maplen = sb.st_size;
		else
			maplen = lengthGiven;
	} else {
		maplen = DEFAULT_MAPSIZE;
	}

	if (maplen > sb.st_size) {
		fprintf(stderr,
				"Truncating map length to file size of %ld bytes\n",
				(long) sb.st_size);
		maplen = sb.st_size;
	}

	/* if map offset given, use it */
	if (argc > 3) {
		mapoffset = atoi(argv[3]);
		if (mapoffset < 0) {
			fprintf(stderr,
					"Map offset cannot be negative, but '%s' given\n", argv[3]);
			return -1;
		}
	}

	/** set up the map */
	mmapRegion = mmap(
			NULL, /* requested address */
			maplen, 
			PROT_READ,
			MAP_SHARED, /* flags */
			mapFD,
			mapoffset); /* offset */

	/** make sure that the mapping succeeded */
	if (mmapRegion == MAP_FAILED) {
		fprintf(stderr, "Memory mapping of '%s' (%ld bytes) failed: %s\n",
				argv[1], (long) maplen, strerror(errno));
		return -1;
	}


	/** dump the given region */
	printf("Dumping %ld bytes mapped to address %p, starting at offset %ld\n",
			(long) maplen, mmapRegion, (long) mapoffset);

	/**
	 * passing the region + offset here to get the reported addresses to
	 * be relative to the starting location of the map region, but shifted
	 * so that if we have skipped the first few bytes via setting an offset,
	 * then everything still works fine
	 */
	status = dump_map(stdout, mmapRegion, (mmapRegion + mapoffset), maplen);

	/** unmap and close */
	(void) munmap(mmapRegion, maplen);
	close(mapFD);

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

