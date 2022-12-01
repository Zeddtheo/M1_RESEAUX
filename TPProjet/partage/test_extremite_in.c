#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iftun.h"
#include "extremite.h"
int main (int argc, char **argv){

	if (argc != 3) {
		printf("Usage Error \n");
		exit(1);
	}

	char *idTun = argv[1];

	char cmd[100];
	int fd;

	/** Récupération du tunel **/
	fd = tun_alloc(idTun);

	strcpy(cmd, "bash ./configure-tun.sh ");
	strcat(cmd, idTun);

	/** Execution des configurations **/
	system("chmod +x configure-tun.sh");
	system(cmd);

	//Lancer l'écouteur du tunel et renvoi des packets au serveur 
	ext_in(fd,argv[2],"123");

	return 0;
}