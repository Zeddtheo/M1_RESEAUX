#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define MAXLIGNE 80

int ext_out(char *port,int tun_fd);
int ext_in(char * hote, char* port, int tunfd);