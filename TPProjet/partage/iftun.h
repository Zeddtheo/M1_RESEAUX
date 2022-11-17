#ifndef IFTUN_H			
#define IFTUN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#define TAILLE_BUFFER 1024

void src_dst_copy(int src, int dst);

#endif