#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/stat.h>
int main(int argc, char const *argv[])
{
	char input[BUFSIZ];

	if (argc < 2)
	{
		perror("nessun file specificato, utilizzo: copy_input <nomefile>\n");
		exit(EXIT_FAILURE);
	}

	int fd = open(argv[1],
				  O_CREAT | O_WRONLY | O_TRUNC,
				  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
	{
		perror("problemi a creare il file");
		exit(EXIT_FAILURE);
	}

	bool fine = false;
	while (!fine)
	{
		printf("inserisci una stringa\n");
		scanf("%s", input);
		if (strcmp(input, "fine") != 0)
		{
			if (write(fd, strcat(input, "\n"), strlen(input) + 1) == -1)
			{
				perror("problemi a scrivere la stringa sul file");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			fine = true;
		}
	}
	if (close(fd) == -1)
	{
		perror("problemi a chiudere il file");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
