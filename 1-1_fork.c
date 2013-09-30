#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
/*
 *	This is the basic fork function.
 *	if you want to see the detail of fork(),
 *	Type "man 2 fork" in your terminal.
 */
int main(int argc, char* argv[]) {
	pid_t cpid;
	char str[100];

	cpid = fork();

	if (cpid < 0) { /* failed */
		// if fork fail, it will return negative value.
		perror("Fork() failed");
		exit(1);
	} else if (cpid == 0) { /* child process */
		// the child process will get 0 when fork return.
		sprintf(str, "This is child process with pid: %u, child pid: %u\n", getpid(), cpid);
	} else { /* parent process */
		// the parent process will get the child process pid when fork return.
		sprintf(str, "This is parent process with pid: %u, child pid: %u\n", getpid(), cpid);

		wait(5);
	}

	printf("%s", str);

	return 0;
}
