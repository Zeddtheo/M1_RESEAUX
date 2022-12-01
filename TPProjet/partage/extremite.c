#include "extremite.h"

int ext_out(int fd, char *port){
  int s,n; /* descripteurs de socket */
  int len,on; /* utilitaires divers */
  struct addrinfo * resol; /* résolution */
  struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
                           PF_INET6,SOCK_STREAM,0, /* IP mode connecté */
                           0,NULL,NULL,NULL};
  struct sockaddr_in client; /* adresse de socket du client */
  int err = getaddrinfo(NULL,port,&indic,&resol); /* code d'erreur */
  printf("im here");
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
    char hotec[NI_MAXHOST]; 
    char portc[NI_MAXSERV];
		err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
		if (err < 0 ){
			fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
		}else{
			fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
		}
    echo(n,hotec,portc,fd);
    // ssize_t lu; /* nb d'octets reçus */
    // char msg[MAXLIGNE+1]; /* tampons pour les communications */
		// char tampon[MAXLIGNE+1];
    // do { /* Faire echo et logguer */
  	// 	lu = recv(n,tampon,MAXLIGNE,0);
  	// 	if (lu > 0 )
  	// 	{
  	// 		//tampon[lu] = '\0';
    //         /* log */

  	// 		snprintf(msg,MAXLIGNE,"> %s",tampon);
  	// 		write(fd, tampon, lu);
  	// 	}
	  //  }while (1);
  }
  close(n);
  return EXIT_SUCCESS;
}
int ext_in(int fd, char *hote, char *port)
{
  struct addrinfo *resol; /* struct pour la résolution de nom */
  int s; /* descripteur de socket */
  char *tmpdst;
  tmpdst = malloc(INET6_ADDRSTRLEN);
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
  fprintf(stderr,"le n° de la socket est : %i\n",s);
  fprintf(stderr,"Essai de connexion à %s (%s) sur le port %s\n\n",fd,port,hote);
  if (connect(s,resol->ai_addr,sizeof(struct sockaddr_in6))<0) {
		perror("connection");
		exit(4);
	}
  freeaddrinfo(resol); /* /!\ Libération mémoire */
  /* Session */

	char buffer[1024];
	int nread;

	while (1) {
		nread = read(fd, buffer, 1024);
		if (nread <= 0) {
			printf("nread = %d\n", nread);
			break;
		}

		printf("%d caracteres lus par le  client : \n", nread);
		for (int i = 0; i < nread; i++) {
			printf("%c", buffer[i]);
		}
		printf("\n");

		send(s, buffer, nread, 0);
	}

	close(s);
	fprintf(stderr, "Fin de la session.\n");
  // while (1) {
  //   src_dst_copy(fd,s);
  // }
  // /* Destruction de la socket */
  // close(s);

  // fprintf(stderr,"Fin de la session.\n");
  return EXIT_SUCCESS;
}
void echo(int fd, char *hote, char *port, int tunnel)
{
  int nread;
	char buffer[1024];
	int pid = getpid();

	while (1) {
		nread = recv(fd, buffer, 1024, 0);
		if (nread <= 0) {
			printf("nread = %d\n", nread);
			break;
		}
		printf("%d caracteres lus par le serveur :\n", nread);
		for (int i = 0; i < nread; i++) {
			printf("%c", buffer[i]);
		}
		printf("\n");
		write(tunnel, buffer, nread);
	}

	close(fd);
	fprintf(stderr, "[%s:%s](%i): Termine.\n", hote, port, pid);
}
//Connection bidirectionnel
void ext_bi(char *ipOut, char* portOut, char *portIn, int fd) {
    int f = fork();
    if(f < 0){
      perror("Fork\n");
      exit(1);
	  }
    else if(f == 0){
      ext_in(fd ,ipOut,portOut);
      sleep(5);
    }
    else {
      ext_out(fd, portIn);
    }
}
