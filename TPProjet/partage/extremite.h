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

int ext_out(char* port, int fd);
int ext_in(char * hote, char* port, int fd);
void echo(int f, int fd);
void ip_add_route(char *ip);