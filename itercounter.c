#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <signal.h>
int contatore = 0;

void handler()
{
    printf("%d\n", contatore);
    exit(EXIT_SUCCESS);
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        perror("file non specificato, utilizzo: itercounter <Nf> <Nsec>\n");
        exit(EXIT_FAILURE);
    }
    int Nf = atoi(argv[1]);
    int Nsec = atoi(argv[2]);
    int pids[Nf];
    for (int i = 0; i < Nf; i++)
    {
        int pid = fork();
        if (pid == 0)
        {
            signal(SIGUSR1, handler);
            while (1)
            {
                sleep(1);
                contatore++;
            }
            exit(0);
        }
        else
        {
            pids[i] = pid;
        }
    }
    sleep(Nsec);
    for (int i = 0; i < Nf; i++)
    {
        kill(pids[i], SIGUSR1);
    }
    return 0;
}
