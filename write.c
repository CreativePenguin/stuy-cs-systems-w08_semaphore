#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>

#define WRITE 1
#define SEM_KEY 25694
#define SEG_SIZE 255

union semun {
  int val;               /* Value for SETVAL */
  struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
  unsigned short *array; /* Array for GETALL, SETALL */
  struct seminfo *__buf; /* Buffer for IPC_INFO
                            (Linux-specific) */
};

int main() {
  printf("Last addition:");
  int shmd;
  char *data;
  int fds[2];
  pipe(fds);
  char term_in[256];

  shmd = shmget(SEM_KEY, SEG_SIZE, IPC_CREAT | IPC_EXCL | 0640);
  data = shmat(shmd, 0, 0);
  printf("fgets failed. Rats\n");
  fgets(data, 256, stdin);
  printf("%s\n", data);
  term_in[strlen(term_in) - 1] = '\0';
  write(fds[WRITE], term_in, 255);
  printf("fds[WRITE] = %d\n", fds[WRITE]);
  printf("data = %s\n", data);
}
