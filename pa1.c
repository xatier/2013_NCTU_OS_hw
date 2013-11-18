#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 40

int main (int argc, char* argv[]) {
	const char path[20] = "named_pipe";
	char str[BUFFER_SIZE] = "";
	int fd;
    int count = 0;
    int game_over = 0;

	pid_t cpid;

	if ( mkfifo(path, 0666) < 0 ) {
		perror("mkfifo failed.");
		return -1;
	}

	printf("mkfifo successed.\n");

	cpid = fork();
	if ( cpid < 0 ) {
		perror("Fork failed.");
		return -1;
	} else if ( cpid != 0 ) {
		fd = open(path, O_WRONLY);
        sprintf(str, "%d", count);
		write(fd, str, sizeof(char)*(strlen(str)+1));
        close(fd);
	}

    // game loop
    while (!game_over) {

        fd = open(path, O_RDONLY);
		read(fd, str, sizeof(char)*BUFFER_SIZE);
        puts(str);
        sscanf(str, "%d", &count);
        close(fd);

        // printf("[%d: count%d]\n", cpid, count);

        if (cpid == 0) {
            if (count % 2 == 1) {      // child's turn
                count++;
                printf("%d ->c %d\n", count-1, count);
            }
        }
        else {
            if (count % 2 == 0) {   // parent's turn
                count++;
                printf("%d ->p %d\n", count-1, count);
            }
        }


		fd = open(path, O_WRONLY);
        sprintf(str, "%d", count);
        puts(str);
		write(fd, str, sizeof(char)*(strlen(str)+1));
        close(fd);

        if (count >= 8) {
            game_over = 1;
            break;
        }
    }

    if (cpid == 0) {
        puts("child's out");
    }
    else {
        puts("parent's out");
    }

    if (game_over) {
        puts("removed");
	    remove(path);
    }

	return 0;
}
