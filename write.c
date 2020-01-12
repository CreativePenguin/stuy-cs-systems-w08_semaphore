#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <fcntl.h>

#define WRITE 1
#define SHM_KEY 25694
#define SEM_KEY 24601
#define SEG_SIZE 255

//union semun {
//	int val;    /* Value for SETVAL */
//	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
//	unsigned short *array;  /* Array for GETALL, SETALL */
//	struct seminfo *__buf;	 /* Buffer for IPC_INFO
//                                (Linux-specific) */
//};

int main() {
	int semd = semget(SHM_KEY, 1, 0);
  printf("semd = %d, ", semd);
	//int file = open("ban.out", O_APPEND, 0640);
  FILE *file1 = fopen("ban.out", "a");
	struct sembuf sb;
	// int fds[2];
	// pipe(fds);
	// char term_in[255];
	sb.sem_num = 1;
	sb.sem_op = -1;
  if(semop(semd, &sb, 1) == -1) printf("semop == -1\n");
  //if(semd == -1) {
	//	printf("Run ./control -c first\n");
	//	printf("There is no shared memory available\n");
  //  exit(1);
	//}
	int shmd = shmget(SHM_KEY, 1, 0);
  printf("shmd = %d\n", shmd);
	char *data = shmat(shmd, 0, 0);
	printf("Last addition:");
	printf("%s\n", data);
	printf("Your addition:");
	fgets(data, SEG_SIZE, stdin);
	fputs(data, file1);
  //write(file, data, SEG_SIZE);
	//close(file);
  fclose(file1);
  shmdt(data);
  return 0;
	// term_in[strlen(term_in) - 1] = '\0';
	// write(fds[WRITE], term_in, 255);
	// printf("fds[WRITE] = %d\n", fds[WRITE]);
	//printf("data = %s\n", data);
}
