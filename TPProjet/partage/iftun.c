#include "iftun.h"

void src_dst_copy(int src, int dst)
{
	char buffer[TAILLE_BUFFER];
	int data;
	while(1){
		data = read(src,buffer,sizeof(buffer));
		if(data == -1){
			return 1;
		}else{
			write(dst,buffer,data);
		}
	}
}