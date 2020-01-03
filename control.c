#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define CREATE_ARG "-c"
#define VIEW_ARG "-v"
#define REMOVE_ARG "-r"
#define KEY 694
#define SEG_SIZE 100

void create() {
  
}

int main(int argc, char** argv) {
  int shmd;
  char *data;
  //printf("argv: %s\n", argv[1]);
  if(strcmp(argv[1], CREATE_ARG) == 0) {
    printf("semaphore created\n");
    printf("shared memory created\n");
    printf("file created\n");
    shmd = shmget(KEY, SEG_SIZE, IPC_CREAT | 0640);
    data = shmat(shmd, 0, 0);
  }

  if(strcmp(argv[1], VIEW_ARG) == 0) {
    printf("The story so far:\n");
    strcpy(data, "hello there\n");
    data[strchr(data, "\n")] = 0;
    printf("%s\n", data);
  }

  if(strcmp(argv[1], REMOVE_ARG) == 0) {
    printf("trying to get in\n");
  }

  return 0;
}
