#include <stdio.h>
#include <errno.h>
#include <string.h> /* for strerror() */
#include <stdlib.h> /* for strtol() */
#include <fcntl.h> /* for open() */
#include <ctype.h> /* for isblank() */
#include <unistd.h> /* for isatty() */
#include <sys/stat.h> /* for struct stat */
#include <sys/mman.h>


#define	MIN_MAPSIZE 512
#define	MAX_MAPSIZE 2048

/** forward references */
static int commandLineMapEditor(unsigned char *map);


/**
 * Update data in map.
 *
 * Not much to it -- just copy the data to the memory location
 */
void
updateMemory(unsigned char *mmap, off_t offset, char *text, size_t textlen)
{
	int i;

	for (i = 0; i < textlen; i++)
		mmap[offset + i] = text[i];
}

int
main(int argc, char **argv)
{
	unsigned char *mmapRegion;
	struct stat sb;
	size_t maplen;
	int status;
	int mapFD;

	if (argc < 2) {
		fprintf(stderr, "Need name of file to map\n");
		return -1;
	}

	mapFD = open(argv[1], O_RDWR);
	if (mapFD < 0) {
		fprintf(stderr, "Cannot open file '%s' : %s\n", argv[1], strerror(errno));
		return -1;
	}

	/* find out file size */
	if (fstat(mapFD, &sb) < 0) {
		fprintf(stderr, "Cannot stat '%s' : %s\n", argv[1], strerror(errno));
		return -1;
	}

	/** cap the map size in a reasonable range */
	maplen = sb.st_size > MAX_MAPSIZE ? MAX_MAPSIZE : sb.st_size;
    if (maplen < MIN_MAPSIZE) {
        fprintf(stderr, "Creating map of %d bytes on file of only %ld bytes\n",
                MIN_MAPSIZE, (long) sb.st_size);
        maplen = MIN_MAPSIZE;
    }


	/** set up the map */
	mmapRegion = (unsigned char *) mmap(
			NULL, /* requested address */
			maplen, 
			PROT_READ | PROT_WRITE,
			MAP_SHARED, /* flags */
			mapFD,
			0); /* offset */

	/** make sure that the mapping succeeded */
	if (mmapRegion == MAP_FAILED) {
		fprintf(stderr, "Memory mapping of '%s' failed: %s\n",
				argv[1], strerror(errno));
		return -1;
	}


	/** now do the edits */
	if ((status = commandLineMapEditor(mmapRegion)) < 0) {
		fprintf(stderr, "Error editing map!\n");
	}

	/** unmap and close */
	(void) munmap(mmapRegion, maplen);
	close(mapFD);

	/* exit indicating our success */
	exit(status >= 0 ? 0 : status);
}


/**
 * Loop on input, placing strings into the map
 */
static int
commandLineMapEditor(unsigned char *map)
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

				updateMemory(map, offset, text, textlen);
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

