#include "extremite.h"


/* echo des messages reçus (le tout via le descripteur f) */
void ip_add_route(char *ip){
  char tmp[100] = "ip route add";
  strcat(tmp,ip);
  printf("add route: %s\n", tmp);
  system(tmp);
}
void echo(int f, int fd)
{
  ssize_t lu; /* nb d'octets reçus */
  char tampon[MAXLIGNE+1]; 
  printf("Echo Ready\n");
  while ( 1 ){ /* Faire echo et logguer */
    lu = read(f,tampon,MAXLIGNE);
    if (lu > 0 ){
        tampon[lu] = '\0';
        printf("Lu out: %s\n",tampon);
        write(fd,tampon,lu);
      } 
    else {
        break;
      }
  }
       
  close(f);
}
int ext_out(int fd, char *port){
  int s,n; /* descripteurs de socket */
  int len,on; /* utilitaires divers */
  struct addrinfo * resol; /* résolution */
  struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
                           PF_INET6,SOCK_STREAM,0, /* IP mode connecté */
                           0,NULL,NULL,NULL};
  struct sockaddr_in client; /* adresse de socket du client */
  int err = getaddrinfo(NULL,port,&indic,&resol); /* code d'erreur */
   
  if (err<0){
    fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
    exit(2);
  }

  /* Création de la socket, de type TCP / IP */
  if ((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0) {
    perror("allocation de socket");
    exit(3);
  }
  fprintf(stderr,"le n° de la socket est : %i\n",s);

  /* On rend le port réutilisable rapidement /!\ */
  on = 1;
  if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0) {
    perror("option socket");
    exit(4);
  }
  fprintf(stderr,"Option(s) OK!\n");

  /* Association de la socket s à l'adresse obtenue par résolution */
  if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
    perror("bind");
    exit(5);
  }
  freeaddrinfo(resol); /* /!\ Libération mémoire */
  fprintf(stderr,"bind!\n");

  /* la socket est prête à recevoir */
  if (listen(s,SOMAXCONN)<0) {
    perror("listen");
    exit(6);
  }
  fprintf(stderr,"listen!\n");

  while(1) {
    /* attendre et gérer indéfiniment les connexions entrantes */
    len=sizeof(struct sockaddr_in);
    if( (n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len)) < 0 ) {
      perror("accept");
      exit(7);
    }
    if(fork() == 0){
      echo(n,fd);
      break;
    }
  }
  return EXIT_SUCCESS;
}
int ext_in(int fd, char *hote, char *port)
{
  struct addrinfo *resol; /* struct pour la résolution de nom */
  int s; /* descripteur de socket */
  struct addrinfo hints = {AI_PASSIVE,
                          PF_INET6,SOCK_STREAM,0,
                          0,NULL,NULL,NULL
  };

  if(getaddrinfo(hote,port,&hints,&resol)<0){
    perror("resolution addresse");
    exit(2);
  }
 
  if ((s=socket(resol->ai_family,resol->ai_socktype, resol->ai_protocol))<0) {
    perror("allocation de socket");
    exit(3);
  }

  while (connect(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
    perror("connection");
    sleep(1);
  }
  freeaddrinfo(resol); /* /!\ Libération mémoire */

  src_dst_copy(fd,s);
  /* Destruction de la socket */
  close(s);

  fprintf(stderr,"Fin de la session.\n");
  return EXIT_SUCCESS;
}
int ext_bi(int tunfd, char *outIp, char *port){
  int pid = fork();
  if(pid == -1){
    perror("fork");
    exit(EXIT_FAILURE);
  }
  if(pid == 0){
    return ext_in(tunfd,outIp,port);
  }else{
    return ext_out(tunfd,port);
  }
}


void asyncInOut(char *ipOut, char* portOut, char *portIn, int fdTun) {
    int f = fork();

    if(f < 0){
	  perror("Fork\n");
	  exit(1);
	}
	else if(f == 0){
	  sleep(5);
    ext_in(fdTun, portOut, ipOut);
  }
	else {
    ext_out(fdTun, portIn);
  }
}
