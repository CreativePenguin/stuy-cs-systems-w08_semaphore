#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>

#define WRITE 1
#define SHM_KEY 25694
#define SEM_KEY 63419
#define SEG_SIZE 255

union semun {
  int val;               /* Value for SETVAL */
  struct semid_ds *buf;  /* Buffer for IPC_STAT, IPC_SET */
  unsigned short *array; /* Array for GETALL, SETALL */
  struct seminfo *__buf; /* Buffer for IPC_INFO
                            (Linux-specific) */
};

int main() {
  int shmd = shmget(SHM_KEY, 1, 0);
  int semd = semget(SEM_KEY, 1, 0);
  char *data = shmat(shmd, 0, 0);
  struct sembuf sb;
  // int fds[2];
  // pipe(fds);
  // char term_in[255];
  if(semd == -1) {
    printf("Run ./control -c first\n");
    printf("There is no shared memory available\n");
  }
  printf("Last addition:");
  printf("%s\n", data);
  fgets(data, SEG_SIZE, stdin);
  // term_in[strlen(term_in) - 1] = '\0';
  // write(fds[WRITE], term_in, 255);
  // printf("fds[WRITE] = %d\n", fds[WRITE]);
  printf("data = %s\n", data);
}
