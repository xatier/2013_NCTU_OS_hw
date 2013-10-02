#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * A example to show pipe() usage.
 *
 * A child process will use pipe to give a message to parent process.
 * 
 * if you want to see the detail of pipe()
 * Type "man 2 pipe" in your terminal.
 */

#define BUFFER_SIZE 64

int main (int argc, char* argv[]) {
	int fd[2];
	pid_t cpid;
	char buf[BUFFER_SIZE] = "";
	memset(buf, 0, sizeof(char) * BUFFER_SIZE);

	if ( pipe(fd) < 0 ){
		/* 
		 * pipe() will create a pipe and fill fd array.
		 * fd[0] is for reading, the other is for writing.
		 */
		perror("Pipe failed.");
		return -1;
	}

	cpid = fork();
	
	if ( cpid < 0 ){
		perror("Fork failed.");
		return -1;
	} else if ( cpid == 0 ) { /* child process, sent message to parent */
		close(fd[0]); /* close a unused read pipe fd */
		sprintf(buf, "CAN YOU HEAR ME?? I'm Child %u", getpid());
		write(fd[1], buf, sizeof(char) * (strlen(buf)+1) );
		close(fd[1]);
		return 0;
	} else { /* parent process, recv message from child */
		close(fd[1]); /* close a unused write pipe fd */
		read(fd[0], buf, sizeof(char) * BUFFER_SIZE);
		printf("Parent got a message.\n");
		printf("-> %s\n", buf);
		close(fd[0]);
		return 0;
	}

	return 0;
}
