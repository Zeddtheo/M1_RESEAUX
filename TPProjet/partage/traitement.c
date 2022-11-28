#include "traitement.h"

char* tailleEntete_in (char *buffer, int size) {
  char *newAndImprovedBuffer = malloc(sizeof(char) * (size + 2));
  short sizeP = (short) size;
  newAndImprovedBuffer[0] = sizeP;
  memcpy(newAndImprovedBuffer + 2, buffer, size);
  return newAndImprovedBuffer;
}


int tailleEntete_out (char *buffer) {
  if (sizeof(buffer) < 2)
    return -1;

  char size[2];

  return (int)strtol(size, NULL, 10);
}