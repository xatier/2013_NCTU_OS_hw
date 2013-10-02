A Quick Start Guide
====

### Step 1. Get example code
You can clone the example code from github

    git clone https://github.com/RuKuWu/2013_NCTU_OS_hw.git

Or you can just download the zip file from github and unzip it

P.S. - if you use the workstations of nctu-cs, git is already installed.
### Step 2. Build the code
Go to the dir which example code located in

    cd 2013_NCTU_OS_hw1-master

Then build all src code by

    make

Or you can build each part by

    make [ fork | fifo | shm ]

---
Instruction
---

## A. Process Information

In Linux, most of things are implemented as special files, so as runtime process. `/proc` is a virtual directory.
You can get the information of running processes by looking up files and directories in this folder.

Now, we will show how to get the information from `/proc`.

### Step 1.

>   Open any application you want to observe. 
We use Firefox as example, which is a default browser in most of linux-based operating systems.

### Step 2.

>   Type `ps aux` (it is distinct of `ps -aux`) or `ps fax` in terminal to list all running processes.  
Then, type `ps aux | grep firefox` to show process information of firefox.(Remember the pid of firefox)

>   `ps` and `grep` are build-in program in linux. First of one can list all processes in operating system,
the other one can filter the input and just print out what you want.  

>   If you want to know more details of these two program, type `man ps` and `man grep` in terminal.

### Step 3.

>   Type `pstree [PID]` and you can see the processes tree of firefox.  
Type `cd /proc/[PID]; ls` and you can see all things about the process of firefox.

## B. Linux Multi-Process Programming

In this part, we will tall you how to write a multi-process program in linux environment.
If you are already familiar with this, you can skip this part.

### Before coding: some of our suggestion
1.  Programming language  
>   Basic C/C++ knowledge is required.

2.  Editor  
>   Vim is recommonded. However, you can use any editor you like.  
>   If you are finding a suitable editor, you can try 
[gedit](https://projects.gnome.org/gedit/), [bluefish](http://bluefish.openoffice.nl/index.html), [code::blocks](http://www.codeblocks.org/) and [sublime](http://www.sublimetext.com/).

3.  Makefile  
>   Makefile can help you build the src code and save time.  

### Fork

`fork()` duplicates the calling process.
The origin process is called as 'parent', duplicate is called as 'child'.  
`fork()` returns pid of child process in parent and return 0 in child. If `fork()` is failed, -1 is returned.

If you want to know more details of fork, type `man fork` in terminal.

#### * Example 1-1 fork

>   This example show the basic use of fork.
>   In the example, fork is called and create the child process.  
>   Usually, we use if statement to decide actions of each process.

```C 
  if (cpid < 0) { /* failed */
  } else if (cpid == 0) { /* child process */
  } else { /* parent process */ }
```

>   You can see two of them use same variable "str" as print string buffer; however, they print out different content.  
>   That is because when fork is called, child process is assigned its own memory space. Thus, they would not change any variable which belongs to each other.

#### * Example 1-2 fork with waitpid 

>   We extend the example 1-1 to show how waitpid works. 
`waitpid()` is a function to let parent  process wait for any child process to end.

>   In our example, child uses a file to leave a message to parent. 
If parent reads before child writes, no content will be read.  
>   Thus, Parent should be paused, waiting for end of file writing of child and then parent could read the file. 
In the program, parent calls the `waitpid()` to wait for child to end.

>   `waitpid(-1, 0, 0)` means waiting for all children done, same as `wait(0)`.  
>   If you want more details of `waitpid()`, type `man waitpid` in terminal.

### Named pipe (a.k.a. FIFO)

Pipeline is a useful mechanism in Unix/Linux based system. 
It creates a buffer in memory to let a program push messages to this buffer and then any other program knowing the pipe descriptor can pop out messages from it.

In [Part A - Step 2](https://github.com/RuKuWu/2013_NCTU_OS_hw#step-2), we use a command `ps aux | grep firefox`. It's a use of pipeline. 
`command A | command B` means to pipe the output of A to B as input via a anonymous pipeline. 
Thus, in command `ps aux | grep firefox`, the output of `ps aux` is redirected to `grep firefox` as input.

First, we will introducte the anonymous pipeline in example 2-1 to let you know how pipeline works.  
Then, in exmample 2-2 and 2-3, we will talk about named pipe(FIFO) and show you what's a difference between anonymous and named pipe.

#### * Example 2-1 pipe

>   In this example, we call `pipe()` to create a pipeline for communicating between two processes. 
When `pipe()` is called,  it will give out two file descriptors which are not point to a file but a memory space.
The first descriptor fd[0] is for popping things from memory, the second descriptor fd[1] is for pushing.

```C 
  pipe(fd);
  cpid = fork();
  if ( cpid < 0 ){ /*failed*/
  } else if ( cpid == 0 ) { /* child process, sent message to parent */
    close(fd[0]); /* close a unused read pipe fd */
    write(fd[1], buf, sizeof(char) * (strlen(buf)+1) );
    close(fd[1]);
  } else { /* parent process, recv message from child */
    close(fd[1]); /* close a unused write pipe fd */
    read(fd[0], buf, sizeof(char) * BUFFER_SIZE);
    close(fd[0]);
  }
```

>   **Please note that** `pipe()` should be called before `fork()`. 
That is because two processes should know the file descriptor of pipeline.
If `pipe()` is put after `fork()`, only the process calling `pipe()` changes the file descriptors.
In contrast, if putting `pipe()` before `fork()`, the value of file descriptors is copied to child process and child process can use the descriptors. 

>   If you want to know more details of `pipe()`, type `man pipe` in terminal.

#### * Example 2-2 FIFO

>   Named pipeline (a.k.a. FIFO) is same as anonymous pipeline, but creating a special file. 
This file let communication between two unrelated processes,  which have no parent-child relation, becomes possible. 
But in this example, we show the basic use first.
