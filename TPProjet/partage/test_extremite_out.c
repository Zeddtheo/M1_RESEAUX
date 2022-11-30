

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iftun.h"
#include "extremite.h"

int main (int argc, char **argv){

	if (argc != 2) {
		printf("Usage Error \n");
		exit(1);
	}

	char *idTun = argv[1];

	char cmd[100];
	int fdTun;


	/** Récupération du tunel **/
	fdTun = tun_alloc(idTun);

	strcpy(cmd, "bash ./configure-tun.sh ");
	strcat(cmd, idTun);

	/** Execution des configurations **/
	system("chmod +x configure-tun.sh");
	system(cmd);

	/** Lancement du serveur d'écoute sur le port 123 **/
	ext_out("123", fdTun);

	return 0;
}