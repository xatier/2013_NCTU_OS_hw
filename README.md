A Quick Start Guide
====
For those who already know about multi-process programming

## Step 1. Get example code
>   You can clone the example code from github
  
    git clone https://github.com/RuKuWu/2013_NCTU_OS_hw1.git

>   Or you can just download the zip file from github and unzip it

P.S. - if you use the workstations of nctu-cs, git is already installed.
## Step 2. Build the code
>   Go to the dir which example code located in

    cd 2013_NCTU_OS_hw1-master

>   Then build all src code by

    make

>   Or you can build each part by

    make [ fork | fifo | shm ]

---
Instruction
---

## A. Process Info

In Linux, most of things are implemented as file descriptor, so as runtime process. `/proc` is a virtual directory.
You can get the information of running processes by looking up files and directories in this folder.

Now, we will show how to get the information from `/proc`.

## Step 1. Run some processes

Open any application you want to observe. 
We use Firefox as example, which is a default application in most of linux-based operating systems.
