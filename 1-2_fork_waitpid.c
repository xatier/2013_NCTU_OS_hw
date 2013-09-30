#include <stdio.h>
#include <unistd.h>
/*
 *	A simple example for waiting child process
 *	if you want to see the detail of waitpid()
 *	type "man waitpid" in your terminal
 */

const char path[30] = "fork_example2.txt";

int main(int argc, char* argv[]) {
	pid_t cpid; // child pid

	cpid = fork();

	if ( cpid < 0 ) {
		perror("Fork() failed\n");
		return -1;
	} else if ( cpid == 0 ) {
		FILE* fd = fopen(path, "w");
		if ( fd < 0) {
			perror("Child fopen failed\n");
			return -1;
		}
		fprintf(fd, "-> A message from child: My pid is %u\n", getpid());
		fclose(fd);
		printf("Child: I left a message.\n");
	} else {
		printf("Parent: Wait for the message child leave\n");
#ifndef _DOWRONG
		waitpid(-1, 0, 0); // same as wait(0);
#endif
		printf("Parent: I got the message.\n");
		FILE* fd = fopen(path, "r");
		if ( fd < 0 ) {
			perror("Parent fopen failed\n");
			return -1;
		}
		char str[50] = "";
		while( fgets(str, 50, fd) )
			printf("%s", str);
		fclose(fd);
		remove(path);
	}


	return 0;
}
