#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        perror("file non specificato, utilizzo: anteprima <num>cerca <nomefile> <stringa1> ... <stringaN>\n");
        exit(EXIT_FAILURE);
    }
    if (access(argv[1], F_OK) == -1)
    {
        perror("problemi con il file\n");
    }
    else
    {
        for (int i = 2; i < argc; i++)
        {
            if (fork() == 0)
            {
                printf("%s\t", argv[i]);
                fflush(stdout);
                execlp("grep", "grep", "--count", argv[i], argv[1], (char *)NULL);
            }
            else
            {
                int status;
                wait(&status);
            }
        }
    }
    return 0;
}
