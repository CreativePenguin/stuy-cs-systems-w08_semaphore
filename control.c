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
#define SEM_KEY 24601
#define SEG_SIZE 255

//union semun {
//	int val;    /* Value for SETVAL */
//	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
//	unsigned short *array;  /* Array for GETALL, SETALL */
//	struct seminfo *__buf;	 /* Buffer for IPC_INFO
//	                            (Linux-specific) */
//};

int error_check(int val) {
  if(val < 0) printf("error %d: %s\n", errno, strerror(errno));
  return val;
}

int main(int argc, char** argv) {
  int shmd;
  char *data;
  int semd;
  int file;
  //FILE *file;
  //printf("argv: %s\n", argv[1]);
  if(argc == 1) {
    printf("error, argument required");
    exit(1);
  }

  if(strcmp(argv[1], CREATE_ARG) == 0) {
    shmd = shmget(SHM_KEY, 1, IPC_CREAT | 0640);
    error_check(shmd);
    data = shmat(shmd, 0, 0);
    error_check((int) data);
    printf("shared memory created: shmd = %d, data = %s\n", shmd, data);
    //printf("shared memory created\n");
    if(semd < 0) {
      semd = semget(SHM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
      printf("Yeet\n");
    } else {
      union semun us;
      us.val = 1;
      semctl(semd, 0, SETVAL, us);
    }
    error_check(semd);
    printf("semaphore created, semd = %d\n", semd);
    error_check(open("ban.out", O_CREAT | O_TRUNC, 0640));
    //fopen("ban.out", "w+");
    //file = fopen("ban.out", "w+");
    printf("file created\n");
    //error_check(shmdt(data));
    //error_check(shmctl(SHM_KEY, IPC_RMID, 0));
  }

  if(strcmp(argv[1], VIEW_ARG) == 0) {
    printf("Trying to get in...\n");
    // semd = semget(SEM_KEY, 1, IPC_STAT);
    // semd = semget(SEM_KEY, 1, 0);
    //union semun us;
    //us.val = 1;
    //semctl(semd, 0, SETVAL, us);
    //error_check(semd);
    //struct sembuf sb;
    //sb.sem_num = 1;
    //sb.sem_op = -1;
    //error_check(semop(semd, &sb, 1));
    //printf("semd = %d, semop = %d\n", semd, semop(semd, &sb, 1));
    //if (semd == -1) {
    //  printf("could not get in\n");
    //  exit(0);
    //}

    printf("The story so far:\n");
    //strcpy(data, "hello there\n");
    //printf("%s\n", strchr(data, '\n'));
    //data[strchr(data, "\n")] = 0;
    //char *buf;
    char buf[SEG_SIZE];

    //while(read(newfile, buf, SEG_SIZE) != -1) printf("%s\n", buf);
    //fgets(buf, SEG_SIZE, file);
    shmd = shmget(SHM_KEY, 0, 0);
    error_check(shmd);
    data = shmat(shmd, 0, 0);
    error_check((int) data);
    file = open("ban.out", O_RDONLY);
    error_check(read(file, buf, SEG_SIZE));
    printf("%s\n", buf);
    printf("Your addition:");
    printf("%s\n", data);
    //while((buf = fgetc(file)) != EOF) {
    //  printf("%c", buf);
    //}
    //printf("%s\n", buf);

    //sb.sem_num = 1;
    //sb.sem_op = 1;
    //semop(semd, &sb, 1);
    close(file);
    shmdt(data);
  }

  if(strcmp(argv[1], REMOVE_ARG) == 0) {
    printf("trying to get in\n");
    semd = semget(SEM_KEY, 1, 0);
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    if(semop(semd, &sb, 1) == -1) printf("semop = -1\n");
    printf("semd = %d\n", semd);
    if(semd == -1) {
      printf("could not get in\n");
      exit(1);
    }
    shmd = shmget(SHM_KEY, 1, 0);
    error_check(shmd);
    data = shmat(shmd, 0, 0);
    error_check((int) data);
    error_check(shmdt(data));
    error_check(shmctl(SHM_KEY, IPC_RMID, 0));
    printf("shared memory removed\n");
    error_check(semctl(SEM_KEY, 0, IPC_RMID));
    //shmctl(int __shmid, int __cmd, struct shmid_ds *__buf);
    //semctl(int __semid, int __semnum, int __cmd, ...);
    printf("semaphore memory removed\n");
    //semop(semd, &sb, 1);
    open("ban.out", O_TRUNC);
    close(file);
  }

  return 0;
}
