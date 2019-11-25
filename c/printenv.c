#include <stdio.h>

int
main(int argc, char **argv, char **env)
{
    int i;

    for (i = 0; env[i] != NULL; i++) {
        fputs(env[i], stdout);
        fputc('\n', stdout);
    }

    return 0;
}
