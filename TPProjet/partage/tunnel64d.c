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
#include "extremite.h"

#define MAX_BUFFER 1024
char max_Buffer[MAX_BUFFER];

typedef struct Conf{
  char *nameTun;
  char *inIp;
  char *inPort;
  char *options;
  char *outIp;
  char *outPort;
}Conf;
//struct Config loadConfig;
char* getValue(char* line){
  int i;
  for(i = 0;line[i]!='=';i++);
  i++;
  char* value = malloc(sizeof(char)*(strlen(line)-1));
  strncpy(value, line+i,strlen(line-1));
  value[strlen(value)-1] = '\0';
  return value;
}
Conf* readConfFile(char* filename){
  Conf* confFile = (Conf*)malloc(sizeof(Conf));
  FILE* file = fopen(filename, "r");

  if(file == NULL){
    printf("Erreur ouverture fichier configration\n");
    return NULL;
  }

char* line = NULL;
  size_t len;
  ssize_t read;
  int i = 0;
  while((read = getline(&line, &len, file)) != -1){

    /* On saute les commentaires */
    if(line[0] == '#'){
      continue;
    }
    char* value = getValue(line);
    switch (i) {
      case 0:
        confFile->nameTun = (char*)malloc(sizeof(char)*strlen(value));
        strcpy(confFile->nameTun, value);
        break;
      case 1:
        confFile->inIp = (char*)malloc(sizeof(char)*strlen(value));
        strcpy(confFile->inIp, value);
        break;
      case 2:
        confFile->inPort = (char*)malloc(sizeof(char)*strlen(value));
        strcpy(confFile->inPort, value);
        break;
      case 3:
        confFile->options = (char*)malloc(sizeof(char)*strlen(value));
        strcpy(confFile->options, value);
        break;
      case 4:
        confFile->outIp = (char*)malloc(sizeof(char)*strlen(value));
        strcpy(confFile->outIp, value);
        break;
      case 5:
        confFile->outPort = (char*)malloc(sizeof(char)*strlen(value));
        strcpy(confFile->outPort, value);
        break;
    }
    i++;
  }
  return confFile;
}

void printConf(Conf *conf){
  printf("name : %s\n", conf->nameTun);
  printf("OUT IP : %s\n", conf->outIp);
  printf("OUT PORT : %s\n", conf->outPort);
  printf("OPTIONS : %s\n", conf->options);
  printf("IN IP : %s\n", conf->inIp);
  printf("IN PORT : %s\n", conf->inPort);
}

int main(int argc, char **argv){
  if (argc != 2) {
		printf("Usage Error \n");
		exit(1);
	};
  Conf *conf = readConfFile(argv[1]);

  printConf(conf);
  int fdTun = tun_alloc(conf->nameTun);
  if(fdTun == -1) {
      fprintf(stderr, "ERREUR ALLOCATION TUNNEL\n");
      return 1;
  }
  char cmd2[256];
  char cmd[256];
  sprintf(cmd, "./configure-tun.sh %s %s ", conf->nameTun, conf->inIp);
  printf("Configuration en cours\n");
  system("chmod +x configure-tun.sh");
  system(cmd);
  sprintf(cmd2, "./configure-route.sh %s %s %s", conf->options, conf->inIp, conf->nameTun);
  printf("Configuration route\n");
  system("chmod +x configure-route.sh");
  system(cmd2);

  asyncInOut(conf->outIp, conf->outPort, conf->inPort, fdTun);
}


// void show_config(){
//   printf("Config: \n");
//   if(loadConfig.tun!=NULL)     printf("tun = %s\n",loadConfig.tun);
//   if(loadConfig.inip!=NULL)    printf("inip = %s\n",loadConfig.inip);
//   if(loadConfig.inport!=NULL)  printf("inport = %s\n",loadConfig.inport);
//   if(loadConfig.options!=NULL) printf("options = %s\n",loadConfig.options);
//   if(loadConfig.outip!=NULL)   printf("outip = %s\n",loadConfig.outip);
//   if(loadConfig.outport!=NULL) printf("outport = %s\n",loadConfig.outport);
// }

// char *str_sub(const char *s, int start){
//   char *new_s = NULL;
//   if(s != NULL){
//     new_s = malloc(sizeof(*new_s)*(MAX_BUFFER-start+2));
//     if(new_s!=NULL){
//       int i;
//       for(i = start;s[i]!='\n'&&s[i]!='\0';i++){
//         new_s[i-start] = s[i];
//       }
//       new_s[i-start-1] = '\0';
//     }else{
//       fprintf(stderr,"Memoire insuffisante\n");
//       exit(EXIT_FAILURE);
//     }
//   }
//   return new_s;
// }
// void read_var_value(char** var, char** value){
//   int cpt = -1;
//   *var = strtok(max_Buffer, "=");
//   cpt = strlen((*var));
//   *value = str_sub(max_Buffer,cpt+1);
// }
// void read_config(char *fileName){
//   FILE *file = fopen(fileName,"r");
//   if(file == NULL){
//     fprintf(stderr,"ERROR!%s\n");
//     exit(1);
//   }
//   char* var;
//   char* value;

//   while(fgets(max_Buffer,MAX_BUFFER,file)!=NULL){
//     if(max_Buffer[0]!='#'){
//       read_var_value(&var, &value);
//       if(strcmp(var,"tun")==0){loadConfig.tun = value;}
//       if(strcmp(var,"inip")==0){loadConfig.inip = value;}
//       if(strcmp(var,"inport")==0){loadConfig.inport = value;}
//       if(strcmp(var,"options")==0){loadConfig.options = value;}
//       if(strcmp(var,"outip")==0){loadConfig.outip = value;}
//       if(strcmp(var,"outport")==0){loadConfig.outport = value;}
//     }
//   }
//   fclose(file);
// }

// int main (int argc, char* argv[]){
//   if(argc!=2){
//     fprintf(stderr,"Usage: %s configFile.txt\n",argv[0]);
//     exit(1);
//   }
//   int tunfd;
//   char dev[2048];
//   read_config(argv[1]);
//   show_config();
//   strcpy(dev,loadConfig.tun);

//   tunfd = tun_alloc(dev);
//   if(tunfd<0){
//     perror("tun alloc");
//     exit(1);
//   }
//   system("bash /mnt/partage/configure-tun.sh");   
//   ip_add_route(loadConfig.outip);
//   ext_bi(tunfd,loadConfig.inip,loadConfig.inport);
//   return 0;
  
//   // printf("Création de %s\n",argv[1]);
//   // tunfd = tun_alloc(argv[1]);
//   // src_dst_copy(tunfd,dst);
//   // printf("tunfd = %s\n",tunfd);
//   // printf("Faire la configuration de %s...\n",argv[1]);
//   // printf("Appuyez sur une touche pour continuer\n");
//   // getchar(); 
//   // printf("Interface %s Configurée:\n",argv[1]);

//   // printf("Appuyez sur une touche pour terminer\n");
//   // getchar();
//   // return 0;
//}
