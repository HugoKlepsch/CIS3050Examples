#include <stdio.h>
#include <signal.h>
#include <string.h> /* for strncmp */

void trapOnIntr()
{
    printf("Got a ^C\n");
}

int
main(int argc, char **argv)
{
    char buffer[BUFSIZ];

    signal(SIGINT, trapOnIntr);

    printf("I will repeat what you type, exit on 'quit' and itnore ^C\n");
    while (fgets(buffer, BUFSIZ, stdin) != NULL) {
        printf("Saw: %s", buffer);
        if (strncmp(buffer, "quit", 4) == 0) {
            return (0);
        }
    }

    return (0);
}
