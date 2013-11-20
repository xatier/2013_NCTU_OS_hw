#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_SIZE 40

int map[50][50] = {0};

void valid (int turn, int cx, int cy, int *nx, int *ny) {
    int dir[4][2] = {{1, 0}, {-1, 0}, {0, -1}, {0, 1}};

    int x, y;
    *nx = *ny = -1;

    for (int d = 0; d < 4; d++) {
        x = cx + dir[d][0];
        y = cy + dir[d][1];
        if (0 <= x && x < 50 &&
            0 <= y && y < 50 &&
            map[x][y] == 0) {
            *nx = x;
            *ny = y;
            map[x][y] = turn+1;
            break;
        }
    }
}

int run (int *turn, int lx, int ly, int *x, int *y) {
    valid(*turn, lx, ly, x, y);

    if (*x == -1 && *y == -1) {
        *turn = *turn ? 3 : 2;
    }
    else {
        *turn = 1 - *turn;
        printf("%d %d\n", *x, *y);
    }
}


int main (int argc, char* argv[]) {
    char str[BUFFER_SIZE] = "";
    char *buf;
    int fd;
    int game_over = 0;
    int turn = 0;
    char output[20];


    int x, y;
    int lx, ly;

    srand(time(NULL));
    x = rand() % 50;
    y = rand() % 50;
    map[x][y] = 2;

    pid_t cpid;

    int shm_id = shmget(IPC_PRIVATE, BUFFER_SIZE, 0666);
    if (shm_id < 0)
    {
        perror("shmget");
        exit(1);
    }


    cpid = fork();
    if ( cpid < 0 ) {
        perror("Fork failed.");
        return -1;
    } else if ( cpid == 0 ) {          // child first
        sprintf(output, "%d_SHM.txt", getpid());
        freopen(output, "w", stdout);
        puts("1");
    }
    else {
        buf = (char *)shmat(shm_id, 0, 0);
        if (buf < (char*)0) {
            perror("shmat");
            exit(1);
        }
        sprintf(buf, "%d %d %d", turn, x, y);

        shmdt(buf);

        sprintf(output, "%d_SHM.txt", getpid());
        freopen(output, "w", stdout);
        puts("0");
    }

    // game loop
    while (turn < 2) {


        buf = (char *)shmat(shm_id, 0, 0);
        if (buf < (char*)0) {
            perror("shmat");
            exit(1);
        }
        sscanf(buf, "%d%d%d", &turn, &lx, &ly);

        shmdt(buf);

        x = lx;
        y = ly;


        if (cpid == 0) {
            if (turn == 0) {      // child's turn
                map[lx][ly] = 2;
                run(&turn, lx, ly, &x, &y);

                buf = (char *)shmat(shm_id, 0, 0);
                if (buf < (char*)0) {
                    perror("shmat");
                    exit(1);
                }
                sprintf(buf, "%d %d %d", turn, x, y);

                shmdt(buf);

                if (turn >= 2) {
                    break;
                }
            }
        }
        else {
            if (turn == 1) {      // parent's turn
                map[lx][ly] = 1;
                run(&turn, lx, ly, &x, &y);

                buf = (char *)shmat(shm_id, 0, 0);
                if (buf < (char*)0) {
                    perror("shmat");
                    exit(1);
                }
                sprintf(buf, "%d %d %d", turn, x, y);

                shmdt(buf);

                if (turn >= 2) {
                    break;
                }
            }
        }
    }

    if (cpid == 0) {
        printf("%c\n", turn == 2 ? '0' : '1');
    }
    else {
        printf("%c\n", turn == 3 ? '0' : '1');
    }

    if (turn >= 2 && cpid != 0) {
        if (shmctl(shm_id, IPC_RMID, 0) !=0)
        {
            perror("shmctl IPC_RMID");
        }
    }

    return 0;
}
