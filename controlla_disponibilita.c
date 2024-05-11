#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[]){
	if(argc<2){
		perror("usage: controlla_disponibilita <magazzino-1> <magazzino-2> ... <magazzino-N>");
		exit(EXIT_FAILURE);
	}
	for(int i=1; i<argc-1; i++){
		if(access(argv[i], R_OK)<0){
			perror("file non leggibile");
			exit(EXIT_FAILURE);
		}
	}
	int **piped_in=(int**)malloc((argc-1)*sizeof(int*));
	int **piped_out=(int**)malloc((argc-1)*sizeof(int*));
	for(int i=0; i<argc-1; i++){
		piped_in[i]=(int*)malloc(2*sizeof(int));
		piped_out[i]=(int*)malloc(2*sizeof(int));
	}

	for(int i=0; i<argc-1; i++){
		if (pipe(piped_in[i]) == -1 || pipe(piped_out[i]) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}
	}

	for(int i=0; i<argc-1; i++){
		pid_t pid = fork();
		switch (pid) {
			case -1:
				perror("fork");
				exit(EXIT_FAILURE);
			case 0:
				for(int j=0; j<argc-1; j++){
					if(j!=i){
						close(piped_in[j][0]);
						close(piped_in[j][1]);
						close(piped_out[j][0]);
						close(piped_out[j][1]);
					}
					else{
						close(piped_in[j][0]);
						close(piped_out[j][1]);
					}
				}
				char risorsa[BUFSIZ];
				while(read(piped_out[i][0], risorsa, BUFSIZ)>0){
					pid_t pid2 = fork();
					switch (pid2) {
						case -1:
							perror("fork nipote");
							exit(EXIT_FAILURE);
						case 0:
							close(1);
							dup(piped_in[i][1]);
							close(piped_out[i][0]);
							close(piped_in[i][1]);
							execlp("grep", "grep", "-c", risorsa, argv[i+1], (char *)NULL);
							perror("execlp");
							exit(EXIT_FAILURE);
					}
					int status;
					wait(&status);
				}
				close(piped_in[i][1]);
				close(piped_out[i][0]);
				exit(EXIT_SUCCESS);
		}
	}
	for(int i=0; i<argc-1; i++){
		close(piped_out[i][0]);
		close(piped_in[i][1]);
	}
	char risorsa[BUFSIZ];
	printf("Inserisci risorsa da controllare: ");
	scanf("%s", risorsa);
	while(strcmp(risorsa, "fine")!=0){
		for(int i=0; i<argc-1; i++){
			if(write(piped_out[i][1], risorsa, BUFSIZ)<0){
				perror("write");
				exit(EXIT_FAILURE);
			}
		}
		for(int i=0; i<argc-1; i++){
			char numero[BUFSIZ];
			read(piped_in[i][0], numero, BUFSIZ);
			printf("nipote numero %d risponde %s", i+1, numero);
		}
		printf("Inserisci risorsa da controllare: ");
		scanf("%s", risorsa);
	}
	for(int i=0; i<argc-1; i++){
		close(piped_out[i][1]);
		close(piped_in[i][0]);
	}
	for(int i=0; i<argc-1; i++){
		int status;
		wait(&status);
	}
	for(int i=0; i<argc-1; i++){
		free(piped_in[i]);
		free(piped_out[i]);
	}
	free(piped_in);
	free(piped_out);
}
