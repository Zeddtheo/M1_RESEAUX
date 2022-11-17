#include "iftun.h"

void src_dst_copy(int src, int dst)
{
	char buffer[TAILLE_BUFFER];
	int nRead;
	nRead = read(src, buffer, TAILLE_BUFFER);
	if (nRead < 0) return;
	send (dst, buffer, TAILLE_BUFFER, 0);
}