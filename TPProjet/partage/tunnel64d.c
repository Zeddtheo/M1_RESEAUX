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

#define MAX_BUFFER 100
char max_Buffer[MAX_BUFFER];

struct Config{
  char *tun;
  char *inip;
  char *inport;
  char *options;
  char *outip;
  char *outport;
};
struct Config loadConfig;

void show_config(){
  printf("Config: \n");
  if(loadConfig.tun!=NULL)     printf("tun = %s\n",loadConfig.tun);
  if(loadConfig.inip!=NULL)    printf("inip = %s\n",loadConfig.inip);
  if(loadConfig.inport!=NULL)  printf("inport = %s\n",loadConfig.inport);
  if(loadConfig.options!=NULL) printf("options = %s\n",loadConfig.options);
  if(loadConfig.outip!=NULL)   printf("outip = %s\n",loadConfig.outip);
  if(loadConfig.outport!=NULL) printf("outport = %s\n",loadConfig.outport);
}

char *str_sub(const char *s, int start){
  char *new_s = NULL;
  if(s != NULL){
    new_s = malloc(sizeof(*new_s)*(MAX_BUFFER-start+2));
    if(new_s!=NULL){
      int i;
      for(i = start;s[i]!='\n'&&s[i]!='\0';i++){
        new_s[i-start] = s[i];
      }
      new_s[i-start-1] = '\0';
    }else{
      fprintf(stderr,"Memoire insuffisante\n");
      exit(EXIT_FAILURE);
    }
  }
  return new_s;
}
void read_var_value(char** var, char** value){
  int cpt = -1;
  *var = strtok(max_Buffer, "=");
  cpt = strlen((*var));
  *value = str_sub(max_Buffer,cpt+1);
}
void read_config(char *fileName){
  FILE *file = fopen(fileName,"r");
  if(file == NULL){
    fprintf(stderr,"ERROR!%s\n");
    exit(1);
  }
  char* var;
  char* value;

  while(fgets(max_Buffer,MAX_BUFFER,file)!=NULL){
    if(max_Buffer[0]!='#'){
      read_var_value(&var, &value);
      if(strcmp(var,"tun")==0){loadConfig.tun = value;}
      if(strcmp(var,"inip")==0){loadConfig.inip = value;}
      if(strcmp(var,"inport")==0){loadConfig.inport = value;}
      if(strcmp(var,"options")==0){loadConfig.options = value;}
      if(strcmp(var,"outip")==0){loadConfig.outip = value;}
      if(strcmp(var,"outport")==0){loadConfig.outport = value;}
    }
  }
  fclose(file);
}

int main (int argc, char* argv[]){
  if(argc!=2){
    fprintf(stderr,"Usage: %s configFile.txt\n",argv[0]);
    exit(1);
  }
  int tunfd;
  char dev[2048];
  read_config(argv[1]);
  show_config();
  strcpy(dev,loadConfig.tun);

  tunfd = tun_alloc(dev);
  if(tunfd<0){
    perror("tun alloc");
    exit(1);
  }
  system("bash /mnt/partage/configure-tun.sh");   
  ip_add_route(loadConfig.outip);
  ext_bi(tunfd,loadConfig.inip,loadConfig.inport);
  return 0;
  
  // printf("Création de %s\n",argv[1]);
  // tunfd = tun_alloc(argv[1]);
  // src_dst_copy(tunfd,dst);
  // printf("tunfd = %s\n",tunfd);
  // printf("Faire la configuration de %s...\n",argv[1]);
  // printf("Appuyez sur une touche pour continuer\n");
  // getchar(); 
  // printf("Interface %s Configurée:\n",argv[1]);

  // printf("Appuyez sur une touche pour terminer\n");
  // getchar();
  // return 0;
}
