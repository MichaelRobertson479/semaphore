#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define SEMKEY 24602
#define SHMKEY 24601
#define SEGSIZE 100

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

int main (int argc, char *argv[]) {

    //shared memory
    int shmd;

    //semaphore
    int semd;
    int v, r;

    //file
    int file;

    if (argc == 1) {
        printf("missing flag\n");
        return 0;
    }

    else {
        if (strcmp(argv[1],"-c") == 0) {
            
            //create semaphore
            semd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);

            if (semd == -1) {
                printf("error %d: %s\n", errno, strerror(errno));
                semd = semget(SEMKEY, 1, 0);
                v = semctl(semd, 0, GETVAL, 0);
                printf("semctl returned: %d\n", v);
            }
  
            else {
                union semun us;
                us.val = 1;
                r = semctl(semd, 0, SETVAL, us);
                printf("semctl returned: %d\n", r);
            }

            //create shared memory
            shmd = shmget(SHMKEY, SEGSIZE, IPC_CREAT | 0644);

            //create file
            file = open("story", O_CREAT | O_EXCL | O_TRUNC | O_RDWR, 0644);
        }

        else if (strcmp(argv[1],"-v") == 0) {
            
            //display story
            file = open("story", O_RDONLY);
            char line [SEGSIZE];

            printf("The story so far:\n");

            while (read(file, line, SEGSIZE) > 0) {
                printf("%s\n",line);
            }

            close(file);
        }

        else if (strcmp(argv[1],"-r") == 0) {
            
            //display story
            file = open("story", O_RDONLY);
            char line [SEGSIZE];

            printf("The story so far:\n");

            while (read(file, line, SEGSIZE) > 0) {
                printf("%s\n",line);
            }

            close(file);

            //remove semaphore
            semctl(semd, IPC_RMID, 0);
            printf("\nsemaphore removed\n");

            //remove shared memory
            shmctl(shmd, IPC_RMID, 0);
            printf("shared memory removed\n");

            //remove file
            remove("story");
            printf("file removed\n");
        }

        else {
            printf("invalid flag\n");
            return 0;
        }
    }

    return 0;
}