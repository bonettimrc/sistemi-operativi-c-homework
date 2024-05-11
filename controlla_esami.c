#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	if(argc<2){
		perror("usage: controlla_esami <file_dati>");
		exit(1);
	}

	char matricola[BUFSIZ];
	while(1){
		printf("Inserisci nome matricola:");
		scanf("%s", matricola);
		if(strcmp(matricola, "fine")==0){
			exit(1);
		}
		int piped[2];
		if(pipe(piped)<0){
			perror("pipe");
		}
		pid_t pid1 = fork();
		switch(pid1){
			case -1:
				perror("first fork");
				break;
			case 0:
				// child P1
				close(1);
				dup(piped[1]);
				close(piped[0]);
				close(piped[1]);
				execlp("grep", "grep", matricola, argv[1], (char *)NULL);
				break;
			default:
				// father
				pid_t pid2 = fork();
				switch(pid2){
					case -1:
						perror("second fork");
						break;
					case 0:
						// child P2
						close(0);
						dup(piped[0]);
						close(piped[0]);
						close(piped[1]);
						printf("Il numero di studenti corrispondenti: \n");
						execlp("wc", "wc", "--lines", (char *)NULL);
						break;
					default:
						//father
						close(piped[0]);
						close(piped[1]);
						int status;
						wait(&status);
						wait(&status);
				}
		}
	}
	return 0;
}
