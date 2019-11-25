/**
 * Simple example using stat for UNIXen only
 */


#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include <sys/stat.h>

int
main(int argc, char *argv[])
{
    /** need a local stat buffer */
    struct stat sb;
    int i;

    /** check that the arguments are correct */
    if (argc < 2) {
        fprintf(stderr, "Need at least one file name\n");
        return -1;
    }

    for (i = 1; i < argc; i++) {
        if (stat(argv[i], &sb) < 0) {
            fprintf(stderr, "Cannot stat '%s' : %s\n", argv[i], strerror(errno));
            return -1;
        }

        printf("Stat information about '%s':\n", argv[i]);
        printf("  Device ID  :   %ld\n", (long) sb.st_dev);
        printf("  i-Node     :   %ld\n", (long) sb.st_ino);
        printf("  # of links :   %ld\n", (long) sb.st_nlink);
        printf("  Size       :   %ld\n", (long) sb.st_size);
        printf("  Blocks     :   %ld\n", (long) sb.st_blocks);
        printf("  Block Size :   %ld\n", (long) sb.st_blksize);
        printf("  Type/Mode  :  0%lo\n", (long) sb.st_mode); /* in octal */
        printf("  UID        :   %lx\n", (long) sb.st_uid);
        printf("  GID        :   %lx\n", (long) sb.st_gid);
        printf("  Access Time: %s", ctime(&sb.st_atime));
        printf("  Mod    Time: %s", ctime(&sb.st_mtime));
        printf("  Create Time: %s", ctime(&sb.st_ctime));
    }

    return 0;
}
