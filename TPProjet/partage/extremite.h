#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "iftun.h"
#define MAXLIGNE 80
#define PORT 123
#define CIAO "Au revoir ...\n"

int ext_out(int fd, char *port);
int ext_in(int fd,char *hote, char *port);
void echo(int f, int fd);
void ip_add_route(char *ip);
int ext_bi(int tunfd,char *outIp, char *port);
void asyncInOut(char *ipOut, char* portOut, char *portIn, int fdTun);