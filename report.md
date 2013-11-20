# Operating system homework 2 report

## (20pts) A.3: Report

### In your report, you should explain your design and how you solve the synchronization problem in the shared memory version.


    I use a variable called `turn` to decide the status of the game. There're 4 states in the game: parent's turn, child's turn, parent wins, child wins. The two IPC mechanisms use FIFO/SHM to pass this variable to the other process.



## (10pts) B.1: What is the purpose of the program? What are the meanings of the output messages?

    The purpose of the program is filling a block of memory with an integer with different ways.
    The output messages are the duration time between the for ( k = 0; k < ROUND; k++) { blah... } loop.



## (20pts) B.2: Tweak BUF_SIZE (in `shared_mem.cpp`) to `1024*1024*60` and redo the experiments. Describe your findings and explain the cause.

    SysV's dual process method crashed. We're out of share memory (too large query size).

## (10pts) B.3: What are the technical differences between using -DSINGLE and -DCOPY?

    Duplicate the `BUF_X` array or not. If we try to write the same space, the compiler may optimize the code and reduce half of assignment.

## (10pts) B.4: What are the technical differences between using -DSINGLE and -DSHM?

    do fork or not, use single process for dual process.

## (10pts) B.5: What are the technical differences between using -DSHM and -DMMAP?

    Use system V's shmget/shmat/shmdt APIs or POSIX shm_open/shm_unlink/mmap APIs. The detail implementation of these two APIs are different, so the limitation of shm size are different.


## Appendix.

### Run script: `run.sh`
    #!/bin/sh
    
    echo "single"
    g++ -O3 -DSINGLE ./shared_mem.cpp -lrt
    ./a.out
    
    echo "copy"
    g++ -O3 -DCOPY ./shared_mem.cpp -lrt
    ./a.out
    
    echo "shm"
    g++ -O3 -DSHM ./shared_mem.cpp -rlt
    ./a.out
    
    echo "mmap"
    g++ -O3 -DMMAP ./shared_mem.cpp -lrt
    ./a.out



- Dear TA: please don't use `–` instead of `-` in command line :)


### Exp 1. `BUF_SIZE=1024*1024*6`


    $ ./run.sh
    single
    
    Single process mode
    
    Duration : 13.2681 seconds
    copy
    
    Single process mode with data copy
    
    Duration : 34.5009 seconds
    shm
    
    Dual process mode
    
    SHM_SIZE = 25165824
    
    Using System V shared memory segment
    BUF_X at 0x2b66982e8000
    BUF_Y at 0x2b6699ae8000
    Duration : 12.4914 seconds
    mmap
    
    Dual process mode
    
    SHM_SIZE = 25165824
    Using mmap
    BUF_X at 0x2b1b5f543000
    BUF_Y at 0x2b1b60d43000
    Duration : 12.491 seconds
    

### Exp 2. `BUF_SIZE=1024*1024*60`
    
    $ ./run.sh
    single
    
    Single process mode
    
    Duration : 131.665 seconds
    copy
    
    Single process mode with data copy
    
    Duration : 338.06 seconds
    shm
    
    Dual process mode
    
    SHM_SIZE = 251658240
    Using System V shared memory segment
    a.out: ./shared_mem.cpp:144: void TwoProcess(): Assertion `sid !=-1' failed.
    ./run.sh: line 13: 22771 已經終止            (core dumped) ./a.out


    mmap
    
    Dual process mode
    
    SHM_SIZE = 251658240
    Using mmap
    BUF_X at 0x2b4f9f319000
    BUF_Y at 0x2b4fae319000
    Duration : 139.584 seconds
