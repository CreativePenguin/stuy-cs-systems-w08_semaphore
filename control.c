#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>

#define CREATE_ARG "-c"
#define VIEW_ARG "-v"

void create() {

}

int main(int argc, char** argv) {
  //printf("argv: %s\n", argv[1]);
  if(strcmp(argv[1], CREATE_ARG) == 0) {
    printf("semaphore created\n");
    printf("shared memory created\n");
    printf("file created\n");
  }

  if(strcmp(argv[1], VIEW_ARG) == 0) {
    printf("The story so far:\n");
  }

  return 0;
}
