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
#include "extremite.h"

#define TAILLE_BUFFER 1024

char globalBuffer[TAILLE_BUFFER];

// int tun_alloc(char *dev)
// {
//   struct ifreq ifr;
//   int fd, err;

//   if( (fd = open("/dev/net/tun", O_RDWR)) < 0 ){
//     perror("alloc tun");
//     exit(-1);	
//   }

//   memset(&ifr, 0, sizeof(ifr));

//   /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
//    *        IFF_TAP   - TAP device  
//    *o
//    *        IFF_NO_PI - Do not provide packet information  
//    */ 
//   ifr.ifr_flags = IFF_TUN; 
//   if( *dev )
//     strncpy(ifr.ifr_name, dev, IFNAMSIZ);

//   if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
//     close(fd);
//     return err;
//   }
//   strcpy(dev, ifr.ifr_name);
//   return fd;
// }      

  int main (int argc, char** argv)
  {
    char nameTun[TAILLE_BUFFER];
    strcpy(nameTun, argv[1]);
    int tunfd;
    if(argc < 2) {
  	  fprintf(stderr,"Usage: %s interface\n",argv[0]);
    }
    printf("Création de %s\n",argv[1]);
    tunfd = tun_alloc(nameTun);
    printf ("tunfd: %d\n", tunfd);
    system("bash /mnt/partage/configure-tun.sh");
  	src_dst_copy(tunfd, 1);
    printf("Faire la configuration de %s...\n",argv[1]);
    printf("Appuyez sur une touche pour continuer\n");
    getchar();
    printf("Interface %s Configurée:\n",argv[1]);
    system("ip addr");
    printf("Appuyez sur une touche pour terminer\n");
    getchar();

    return 0;
  }
// int main (int argc, char** argv){

//   int tunfd;
//   printf("Création de %s\n",argv[1]);
//   tunfd = tun_alloc(argv[1]);
//   printf("Faire la configuration de %s...\n",argv[1]);
//   system("bash /mnt/partage/configure-tun.sh");
//   printf("Appuyez sur une touche pour continuer\n");
//   getchar();
//   printf("Interface %s Configurée:\n",argv[1]);
//   system("ip addr");
//   printf("Appuyez sur une touche pour terminer\n");
//   getchar();

//   return 0;
// }