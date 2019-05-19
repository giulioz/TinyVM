#include <stdio.h>

int CVMstart();

void printVal(int value) {
  printf("PRINT: %d\n", value);
}

int main(int argc, char **argv) {
  CVMstart();
  return 0;
}