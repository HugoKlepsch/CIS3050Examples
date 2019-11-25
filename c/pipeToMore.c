#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


/**
 * This program shows the steps involved in creating an output pipe
 * which is the standard in of a child program.
 *
 * A bunch of (interesting) data is then piped into the 
 * child, which in this case is the "more" program.
 */

#if defined( __APPLE__ )
# define	BINMORE	"/usr/bin/more"
#else
# define	BINMORE	"/bin/more"
#endif

/** a bunch of lines of data which can be piped into a child program */
static char *someData[] = {
	"In Xanadu did Kubla Khan",
	"A stately pleasure-dome decree:",
	"Where Alph, the sacred river, ran",
	"Through caverns measureless to man",
	"Down to a sunless sea.",
	"",
	"So twice five miles of fertile ground",
	"With walls and towers were girdled round:",
	"And there were gardens bright with sinuous rills,",
	"Where blossomed many an incense-bearing tree;",
	"And here were forests ancient as the hills,",
	"Enfolding sunny spots of greenery.",
	"",
	"But oh! that deep romantic chasm which slanted",
	"Down the green hill athwart a cedarn cover!",
	"A savage place! as holy and enchanted",
	"As e'er beneath a waning moon was haunted",
	"By woman wailing for her demon-lover!",
	"And from this chasm, with ceaseless turmoil seething,",
	"As if this earth in fast thick pants were breathing,",
	"A mighty fountain momently was forced:",
	"Amid whose swift half-intermitted burst",
	"Huge fragments vaulted like rebounding hail,",
	"Or chaffy grain beneath the thresher's flail:",
	"And 'mid these dancing rocks at once and ever",
	"It flung up momently the sacred river.",
	"Five miles meandering with a mazy motion",
	"Through wood and dale the sacred river ran,",
	"Then reached the caverns measureless to man,",
	"And sank in tumult to a lifeless ocean:",
	"And 'mid this tumult Kubla heard from far",
	"Ancestral voices prophesying war!",
	"",
	"The shadow of the dome of pleasure",
	"Floated midway on the waves;",
	"Where was heard the mingled measure",
	"From the fountain and the caves.",
	"It was a miracle of rare device,",
	"A sunny pleasure-dome with caves of ice!",
	"",
	"A damsel with a dulcimer",
	"In a vision once I saw:",
	"It was an Abyssinian maid,",
	"And on her dulcimer she played,",
	"Singing of Mount Abora.",
	"Could I revive within me",
	"Her symphony and song,",
	"To such a deep delight 'twould win me",
	"That with music loud and long",
	"I would build that dome in air,",
	"That sunny dome! those caves of ice!",
	"And all who heard should see them there,",
	"And all should cry, Beware! Beware!",
	"His flashing eyes, his floating hair!",
	"Weave a circle round him thrice,",
	"And close your eyes with holy dread,",
	"For he on honey-dew hath fed",
	"And drunk the milk of Paradise.",
	NULL
    };


/**
 * Simple mainline which does all the work
 */
int
main()
{
	int pipefds[2];
	int pid;
	int i;
	char nl = '\n';

	/*
	 *  create the pipe
	 *
	 *  must be done BEFORE the fork
	 */
	if (pipe(pipefds) < 0 ) {
		perror("pipe");
		exit(1);
	}


	/**
	 * Create a child process -- this will turn into "more"
	 */
	if ((pid = fork()) < 0)
	{
		perror("fork");
		exit(1);
	}

	/*
	 * child stuff
	 */
	if (pid == 0) {
		/*
		 * make read side of pipe stdin
		 * -- this is the magic to get the
		 * new file descriptor to appear
		 * where you want.  This works because
		 * you will get the lowest numbered
		 * available file descriptor when you
		 * open a new one.
		 */
		close(0);
		dup(pipefds[0]);
		close(pipefds[0]);

		/*
		 * close the write side of the pipe
		 * (because the child (more) is not
		 * going to be writing back to the
		 * parent
		 */
		close(pipefds[1]);


		/*
		 * execute the command -- note that
		 * we don't check the return status
		 */
		execl(BINMORE, "more", NULL);

		/** if we get here, exec() failed, so bail out */
		perror("exec");
		exit(1);
	}

	/*
	 *  parent code -- the child exitted in the
	 *  above if() block
	 */
	
	/*
	 *  close read side of pipe -- same logic as above
	 */
	close(pipefds[0]);


	/*
	 *  send a message, close the pipe
	 */
	i = 0;
	while (someData[i] != NULL) {
	    /** write a line from the data array to this file descriptor */
	    if (write(pipefds[1], someData[i], strlen(someData[i])) < 0)
			perror("write");

	    /** write a newline after the data line */
	    if (write(pipefds[1], &nl, 1) != 1)
			perror("write");

	    /** go to the next data line */
	    i++;
	}


	/** close the pipe -- this will flush the last of the data through */
	close(pipefds[1]);

	/*
	 *  wait for termination of the child "more" program
	 */
	wait(NULL);

	exit(0);
}

