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
#include "iftun.h"



void src_dst_copy(int src, int dst)
{
	 char buffer[TAILLE_BUFFER];
	 int nRead;
	 nRead = read(src, buffer, TAILLE_BUFFER);
	 if (nRead < 0) return;
   send (dst, buffer, TAILLE_BUFFER, 0);
}
int tun_alloc(char *dev)
{
  struct ifreq ifr;
  int fd, err;

  if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ){
    perror("alloc tun");
    exit(1);
  }

  memset(&ifr, 0, sizeof(ifr));

  /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
   *        IFF_TAP   - TAP device  
   *
   *        IFF_NO_PI - Do not provide packet information  
   */ 
  ifr.ifr_flags = IFF_TUN; 
  if( *dev )
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);

  if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
    close(fd);
    return err;
  }
  strcpy(dev, ifr.ifr_name);
  return fd;
}      

int main (int argc, char** argv){

  int tunfd;
  int dst=1;
  
  printf("Création de %s\n",argv[1]);
  tunfd = tun_alloc(argv[1]);
  //src_dst_copy(tunfd,dst);
  //printf("tunfd = %s\n",tunfd);
  printf("Faire la configuration de %s...\n",argv[1]);
  printf("Appuyez sur une touche pour continuer\n");
  getchar(); 
  system("bash /mnt/partage/configure-tun.sh");
  printf("Interface %s Configurée:\n",argv[1]);
  system("ip addr");
  printf("Appuyez sur une touche pour terminer\n");
  getchar();
  getchar();
  return 0;
}
