#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#define SEMKEY 24602
#define SHMKEY 24601
#define SEGSIZE 100

int main() {

  int semd;
  int r, v;
  int file;
  char * data;

  semd = semget(SEMKEY, 1, 0);
  struct sembuf sb;
  sb.sem_num = 0;
  //sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  printf("trying to get in\n");
  semop(semd, &sb, 1);
  //printf("got the semaphore!\n");
  //sleep(10);

  int shmd;
  shmd = shmget(SHMKEY, SEGSIZE, 0644);
  data = shmat(shmd, 0, 0);

  if (!(*data))
    printf("Last addition:\n");
  else
    printf("Last addition: %s\n",data);

  printf("Your addition: ");
  fgets(data, SEGSIZE, stdin);
  *strchr(data, '\n') = '\0';

  file = open("story", O_WRONLY | O_APPEND);
  write(file, data, SEGSIZE);
  close(file);

  sb.sem_op = 1;
  semop(semd, &sb, 1);

  return 0;
}