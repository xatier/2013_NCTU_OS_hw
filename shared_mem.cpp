//============================================================================
// Name        : shared.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

using namespace std;


#define SINGLE_PROCESS    1
#define SINGLE_PROCESS_WITH_COPY 2
#define DUAL_PROCESS_MMAP  3
#define DUAL_PROCESS_SHM   4

#define BUF_SIZE 1024*1024*6
#define ROUND 1000

#ifdef SINGLE
	#define MODE  SINGLE_PROCESS 
#elif defined(COPY)
	#define MODE SINGLE_PROCESS_WITH_COPY
#elif defined(MMAP)
	#define MODE	DUAL_PROCESS_MMAP
#elif defined(SHM)
	#define MODE  DUAL_PROCESS_SHM
#else
	#error "you forgot -D"
#endif


volatile int *BUF_X;
volatile int *BUF_Y;

char MEM_MAPPED_FILE[] = "/XYZ";

timeval st, et;

void Ping(volatile int* buf, int v);
void ShowDuration();
void SingleProcess();
void TwoProcess();

int main()
{
#if MODE==SINGLE_PROCESS 
	printf("\nSingle process mode\n\n");
	SingleProcess();
#elif MODE==SINGLE_PROCESS_WITH_COPY
	printf("\nSingle process mode with data copy\n\n");
	SingleProcess();
#elif MODE==DUAL_PROCESS_MMAP || MODE==DUAL_PROCESS_SHM
	printf("\nDual process mode\n\n");
	TwoProcess();
#else
	#error "unknown mode"
#endif
	return 0;
}

void Ping(volatile int* BUF, int v)
{
	int k;

	while(BUF[BUF_SIZE-1]== v);

	for (k =0; k < BUF_SIZE; k++ ) {
		asm volatile("" : : : "memory");
		BUF[k] = v;
	}
}


void SingleProcess()
{
	int k;
	BUF_X = new int[BUF_SIZE];
#if MODE==SINGLE_PROCESS
	BUF_Y = BUF_X;
#else
	// SINGLE_PROCESS_WITH COPY
	BUF_Y = new int[BUF_SIZE];
#endif
	BUF_X[BUF_SIZE-1] = 0;

	gettimeofday(&st,0);

	for ( k = 0; k < ROUND; k++) {
		Ping(BUF_X, -1);
#if MODE==SINGLE_PROCESS_WITH_COPY
		memcpy((void*)BUF_Y, (void*)BUF_X, BUF_SIZE*sizeof(int));
#endif		
		Ping(BUF_Y, 0);

#if MODE==SINGLE_PROCESS_WITH_COPY
		memcpy((void*)BUF_X, (void*)BUF_Y, BUF_SIZE*sizeof(int));
#endif				
	}

	gettimeofday(&et,0);
	ShowDuration();

	delete[] BUF_X;

#if MODE==SINGLE_PROCESS_WITH_COPY
	delete[] BUF_Y;
#endif			

}


void TwoProcess()
{
	pid_t pidY;
	int k;

	unsigned int SHM_SIZE = BUF_SIZE*sizeof(int);

	printf("SHM_SIZE = %d\n", SHM_SIZE);

#if MODE==DUAL_PROCESS_MMAP
	printf("Using mmap\n");
	int sid = shm_open(MEM_MAPPED_FILE, O_CREAT | O_RDWR,  S_IRWXU);
#else
	printf("Using System V shared memory segment\n");
	int sid = shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | S_IRWXU);
#endif
	//perror(0);
	assert(sid !=-1);

#if MODE==DUAL_PROCESS_MMAP
	ftruncate(sid,  SHM_SIZE );
	BUF_X = (volatile int*) mmap(0, SHM_SIZE, PROT_WRITE|PROT_READ, MAP_SHARED, sid, 0 );
#else
	BUF_X = (int*) shmat(sid, 0, 0);
#endif

	assert(BUF_X);
	cout<<"BUF_X at "<<(void*)BUF_X<<endl;
	BUF_X[BUF_SIZE-1] = 0;

	pidY = fork();

	assert(pidY !=-1);

	if ( pidY == 0) { // Process Y
#if MODE==DUAL_PROCESS_MMAP
		sid = shm_open(MEM_MAPPED_FILE,  O_RDWR,  0);
		assert(sid !=-1);
		BUF_Y = (volatile int*) mmap(0, BUF_SIZE*sizeof(int), PROT_WRITE|PROT_READ, MAP_SHARED, sid, 0 );
#else
		BUF_Y = (int*) shmat(sid, 0, 0);
#endif
		assert(BUF_Y);
		cout<<"BUF_Y at "<<(void*)BUF_Y<<endl;

		for ( k = 0 ; k < ROUND; k++) {
			Ping(BUF_Y, 0);
		}

#if MODE==DUAL_PROCESS_MMAP
		munmap((void*)BUF_Y, BUF_SIZE*sizeof(int));
		sid = shm_unlink(MEM_MAPPED_FILE);
		assert(sid==0);
#else
		shmdt((void*)BUF_Y);
#endif
	}
	else {	// Process X

		gettimeofday(&st,0);

		for ( k = 0 ; k < ROUND; k++) {
			Ping(BUF_X, -1);
		}

		while(BUF_X[BUF_SIZE-1]==-1);

		gettimeofday(&et,0);

		ShowDuration();

#if MODE==DUAL_PROCESS_MMAP
		munmap((void*)BUF_X, BUF_SIZE*sizeof(int));
#else
		shmdt((void*)BUF_X);
		int s = shmctl(sid, IPC_RMID, 0);
		assert(s !=-1);
#endif
	}

}



void ShowDuration()
{
	double d = et.tv_usec;

	d = d - st.tv_usec;

	d = et.tv_sec - st.tv_sec + d/1000000;

	cout<<"Duration : "<<d<<" seconds"<<endl;
}


