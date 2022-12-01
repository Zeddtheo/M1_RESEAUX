#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>
#include "iftun.h"

void test_iftun(char *tun_name, int dst){
	int tunfd;
	unsigned char buffer[1500];
	int nread;
	char command[50];

	tunfd = tun_alloc(tun_name);


	system("chmod +x configure-tun.sh");

	strcpy(command, "./configure-tun.sh ");
	strcat(command, tun_name);
	system(command);
	if(tunfd < 0){
		perror("Allocating interface");
		exit(1);
	}
	while(1) {
        src_dst_copy(tunfd, dst);
	}
}
int main (int argc, char** argv){

	if (argc != 2) {
		printf("Usage Error \n");
		return 1;
	}

	test_iftun(argv[1], 1);

	return 0;
}