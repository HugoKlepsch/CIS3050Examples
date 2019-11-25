#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifndef NEEDS_GETOPT
#include <unistd.h>

extern char *optarg;
extern int optind;
extern int optopt;
extern int opterr;
extern int optreset;

#else

/*
 * Adapted from (broken) code provided by Microsoft at:
 * https://github.com/iotivity/iotivity/tree/master/resource/c_common/windows
 */

char* optarg = NULL;
int optind = 1;
int optopt = 0;

int getopt(int argc, char *const argv[], const char *optstring)
{
    if (optarg == NULL) {
        fprintf(stderr, "Using home-brewed getopt()\n");
    }

    /**
     * if the option index is out of bounds, or arg is not a flag, stop
     */
    if ((optind >= argc)
            || (strchr("-/", argv[optind][0]) == NULL)
            || (argv[optind][0] == 0))
    {
        return -1;
    }

    /* match the flag character againts the options */
    int optopt = argv[optind][1];
    const char *matchingOption = strchr(optstring, optopt);

    /** if no match, we return '?' */
    if (matchingOption == NULL)
    {
        optind++;
        return '?';
    }

    /** if the option string indicates an argument, process it */
    if (matchingOption[1] == ':')
    {
        /**
         * check whether argument abuts flag in argv, otherwise get next argv
         */
        if (argv[optind][2] != '\0')
        {
            optarg = &argv[optind][2];
        } else {
            optind++;
            if (optind >= argc)
            {
                return '?';
            }
            optarg = argv[optind];
        }
    }
    /** advance to the next index */
    optind++;
    return optopt;
}
#endif

void printHelp()
{
    printf("This command prints its arguments out in a new base\n");
}

int main(int argc, char **argv)
{
    FILE *outputStream = stdout;
    int base = 10, ch;
    int verbosity = 0;
    int i;

    while ((ch = getopt(argc, argv, "hvo:b:")) != -1) {
        switch (ch) {
        case 'b':
            base = atoi(optarg);
            fprintf(outputStream, "Base set to %d\n", base);
            break;

        case 'o':
            outputStream = fopen(optarg, "w");
            if (outputStream == NULL) {
                fprintf(stderr, "Cannot open '%s' for output : %s\n",
                        optarg, strerror(errno));
                return -1;
            }
            break;
            
        case 'v':
            verbosity++;
            break;
        case 'h':
        case '?':
        default:
            printHelp();
            return 1;
        }
    }
    argc -= optind;
    argv += optind;

    /** note that argc is now zero! */
    for (i = 0; i < argc; i++) {
        if (base == 10)
            fprintf(outputStream, "%d\n", atoi(argv[i]));
        else if (base == 16)
            fprintf(outputStream, "%x\n", atoi(argv[i]));
        else if (base == 8)
            fprintf(outputStream, "%o\n", atoi(argv[i]));
        else {
            fprintf(stderr, "Unknown base %d\n", base);
            return -1;
        }
    }
    return 0;
}

