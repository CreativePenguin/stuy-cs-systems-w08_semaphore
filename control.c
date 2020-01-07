#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#define CREATE_ARG "-c"
#define VIEW_ARG "-v"
#define REMOVE_ARG "-r"
#define SHM_KEY 25694
#define SEM_KEY 63419
#define SEG_SIZE 255

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
                              (Linux-specific) */
};

int shmget_error(int shmd) {
  if(shmd == -1) {
    printf("error %d: %s\n", errno, strerror(errno));
  }
}

int main(int argc, char** argv) {
  int shmd;
  char *data;
  int semd;
  int v;
  int r;
  struct sembuf sb;
  //printf("argv: %s\n", argv[1]);
  if(argc == 1) {
    printf("error, argument required");
    exit(1);
  }

  if(strcmp(argv[1], CREATE_ARG) == 0) {
    printf("semaphore created\n");
    printf("shared memory created\n");
    printf("file created\n");
    shmd = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | IPC_EXCL | 0640);
    data = shmat(shmd, 0, 0);
    if(semd == -1) {
      semd = semget(SEM_KEY, 1, 0);
      v = semctl(semd, 0, GETVAL, 0);
    } else {
      union semun us;
      us.val = 1;
      r = semctl(semd, 0, SETVAL, us);
    }
  }

  if(strcmp(argv[1], VIEW_ARG) == 0) {
    sb.sem_num = 0;
    sb.sem_op = -1;
    semop(semd, &sb, 1);
    printf("The story so far:\n");
    strcpy(data, "hello there\n");
    printf("%s\n", strchr(data, '\n'));
    //data[strchr(data, "\n")] = 0;
    printf("%s\n", data);
  }

  if(strcmp(argv[1], REMOVE_ARG) == 0) {
    printf("trying to get in\n");
    shmdt(data);
    semop(semd, &sb, 1);
    printf("shared memory removed\n");
  }

  return 0;
}
