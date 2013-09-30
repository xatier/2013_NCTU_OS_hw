CC = gcc
fork_src = 1-1_fork.c 1-2_fork_waitpid.c
fifo_src = 2-1_pipe.c 2-2_fifo.c 2-3_fifo_nonrelated_process.c
shm_src = 3-1_shared_memory.c 3-2_shared_memory_with_key.c 3-3_shared_memory_del.c

.PHONY: all
all: fork fifo shm

fork: $(fork_src:.c=)
	$(foreach fork_src, $(src), $(CC) $(src) -o $(src:.c=);)	
fifo: $(fifo_src:.c=)
	$(foreach fifo_src, $(src), $(CC) $(src) -o $(src:.c=);)	
shm: $(shm_src:.c=)
	$(foreach shm_src, $(src), $(CC) $(src) -o $(src:.c=);)	

.PHONY: clean cleanfork cleanfifo cleanshm
clean: cleanfork cleanfifo cleanshm

cleanfork:
	rm $(fork_src:.c=)
cleanfifo:
	rm $(fifo_src:.c=)
cleanshm:
	rm $(shm_src:.c=)
