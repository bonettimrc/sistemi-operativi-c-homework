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
		perror("numero di righe da visualizzare non specificato, utilizzo: anteprima <num>\n");
		exit(EXIT_FAILURE);
	}

	char input[BUFSIZ];
	bool fine = false;
	while (!fine)
	{
		printf("inserisci nome file\n");
		scanf("%s", input);
		if (strcmp(input, "fine") == 0)
		{
			fine = true;
		}
		else
		{
			if (access(input, F_OK) == -1)
			{
				perror("problemi con il file\n");
			}
			else
			{
				if (fork() == 0)
				{
					execlp("head", "head", "-n", argv[1], input, (char *)NULL);
				}
				else
				{
					int status;
					wait(&status);
				}
			}
		}
	}
	return 0;
}
