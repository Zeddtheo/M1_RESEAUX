#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include "iftun.h"
void catchPackets(char *tun_name, int dst){
    int tunfd;
    unsigned char buffer[1500];
    int nread;
    char command[50];

    tunfd = tun_alloc(tun_name);
    //system("chomd+x configure-tun.sh");
    //strcpy(command, "./configure-tun.sh");
    system("bash /mnt/partage/configure-tun.sh");
    strcat(command,tun_name);
    system(command);

    if(tunfd<0){
        perror("Allocating interface");
        exit(1);
    }
    while(1){
        nread = read(tunfd,buffer,sizeof(buffer));
        if(nread<0){
            perror("Reading from interface");
            close(tunfd);
            exit(1);
        }
        write(dst,buffer,nread);
        printf("\n");
    }
}
int main(int argc, char** argv){
    if(argc != 2){
        printf("Error \n");
        return 1;
    }

    catchPackets(argv[1],1);
    return 0;
}