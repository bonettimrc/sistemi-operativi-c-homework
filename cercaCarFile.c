#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
volatile sig_atomic_t counter = 0;
volatile sig_atomic_t letter;
void handler1()
{
    printf("La lettera %c compare %d volte nel tempo che ho avuto\n", letter, counter);
    exit(EXIT_SUCCESS);
}
void handler2()
{
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        perror("file non specificato, utilizzo: cercaCarFile C1 [C2 ... Cn] NomeFile NumSec\n");
        exit(EXIT_FAILURE);
    }
    char NomeFile[BUFSIZ];
    strcpy(NomeFile, argv[argc - 2]);
    if (access(NomeFile, F_OK) == -1)
    {
        perror("problemi con il file");
        exit(EXIT_FAILURE);
    }
    int NumSec = atoi(argv[argc - 1]);
    int Nf = argc - 3;
    int pids[Nf];
    for (int i = 0; i < Nf; i++)
    {
        int pid = fork();
        if (pid == 0)
        {
            signal(SIGUSR1, handler1);
            signal(SIGUSR2, handler2);
            int fd = open(NomeFile, O_RDONLY);
            char c;
            letter = argv[i + 1][0];
            while (read(fd, &c, sizeof(c)) > 0)
            {
                if (c == argv[i + 1][0])
                {
                    counter++;
                }
            }
            printf("Lettera %s compare %d volte, Ho vinto!\n", argv[i + 1], counter);
            kill(0, SIGUSR2);
            exit(EXIT_SUCCESS);
        }
        else
        {
            pids[i] = pid;
        }
    }
    sleep(NumSec);
    for (int i = 0; i < Nf; i++)
    {
        kill(pids[i], SIGUSR1);
    }
    for (int i = 0; i < Nf; i++)
    {
        int status;
        wait(&status);
    }

    return 0;
}
