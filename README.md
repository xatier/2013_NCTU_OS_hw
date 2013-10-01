A Quick Start Guide
====
For those who already know about multi-process programming

### Step 1. Get example code
>   You can clone the example code from github
  
    git clone https://github.com/RuKuWu/2013_NCTU_OS_hw1.git

>   Or you can just download the zip file from github and unzip it

P.S. - if you use the workstations of nctu-cs, git is already installed.
### Step 2. Build the code
>   Go to the dir which example code located in

    cd 2013_NCTU_OS_hw1-master

>   Then build all src code by

    make

>   Or you can build each part by

    make [ fork | fifo | shm ]

---
Instruction
---

## A. Process Information

In Linux, most of things are implemented as file descriptor, so as runtime process. `/proc` is a virtual directory.
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
if you want to know the detail of these two program, type `man ps` and `man grep` in terminal.

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
>   Makefile can help you  
