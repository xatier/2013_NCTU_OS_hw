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
	pid_t cpid;
	
	if ( mkfifo(path, 0666) < 0 ) { /* create a named pipe file in system */
		perror("mkfifo failed.");
		return -1;
	}

	printf("mkfifo successed.\n");

	cpid = fork();
	if ( cpid < 0 ) {
		perror("Fork failed.");
		return -1;
	} else if ( cpid == 0 ) { /* child process, send a message */
		fd = open(path, O_WRONLY);
		sprintf(str, "CAN YOU HEAR ME?? I'm child %u.", getpid());
		write(fd, str, sizeof(char)*(strlen(str)+1));
		close(fd);
	} else {
		fd = open(path, O_RDONLY);
		read(fd, str, sizeof(char)*BUFFER_SIZE);
		printf("Parent got a message.\n");
		printf("->%s\n", str);
		close(fd);
		remove(path);
	}

	return 0;
}
