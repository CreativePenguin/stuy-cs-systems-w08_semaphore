#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/resource.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
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

int main(int argc, char** argv) {
  int shmd;
  char *data;
  int semd;
  int v;
  int r;
  int newfile;
  struct sembuf sb;
  //printf("argv: %s\n", argv[1]);
  if(argc == 1) {
    printf("error, argument required");
    exit(1);
  }

  if(strcmp(argv[1], CREATE_ARG) == 0) {
    shmd = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | IPC_EXCL | 0640);
    data = shmat(shmd, 0, 0);
    printf("shared memory created\n");
    if(semd == -1) {
      semd = semget(SEM_KEY, 1, 0);
      v = semctl(semd, 0, GETVAL, 0);
    } else {
      union semun us;
      us.val = 1;
      r = semctl(semd, 0, SETVAL, us);
    }
    printf("semaphore created\n");
    newfile = open("ban.out", O_CREAT | O_EXCL | O_TRUNC | O_RDONLY, 0640);
    printf("file created\n");
  }

  if(strcmp(argv[1], VIEW_ARG) == 0) {
    printf("Trying to get in...\n");
    sb.sem_num = 0;
    sb.sem_op = -1;
    semop(semd, &sb, 1);
    if(semd == -1) {
      printf("could not get in\n");
      exit(0);
    }
    printf("The story so far:\n");
    //strcpy(data, "hello there\n");
    //printf("%s\n", strchr(data, '\n'));
    //data[strchr(data, "\n")] = 0;
    char *buf;
    //while(read(newfile, buf, SEG_SIZE)) printf("%s", buf);
    read(newfile, buf, SEG_SIZE);
    printf("%s\n", buf);
    sb.sem_num = 0;
    sb.sem_op = 1;
    semop(semd, &sb, 1);
  }

  if(strcmp(argv[1], REMOVE_ARG) == 0) {
    printf("trying to get in\n");
    sb.sem_num = 0;
    sb.sem_op = -1;
    semop(semd, &sb, 1);
    if(semd == -1) {
      printf("could not get in\n");
      exit(1);
    }
    shmdt(data);
    shmctl(SHM_KEY, IPC_RMID, 0);
    printf("shared memory removed\n");
    semctl(SEM_KEY, IPC_RMID, 0);
    printf("semaphore memory removed\n");
    semop(semd, &sb, 1);
    close(newfile);
  }

  return 0;
}
