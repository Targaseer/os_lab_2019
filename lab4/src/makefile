CC=gcc
CFLAGS=-I.

all : parallel_min_max parallel_sum process_memory

parallel_sum : utils.o parallel_sum.o
	$(CC) -o parallel_sum parallel_sum.o utils.o -L/projects/os_lab_2019/lab4/sum -lsum -lpthread

parallel_sum.o :
	$(CC) -o parallel_sum.o -c parallel_sum.c $(CFLAGS)

process_memory :
	$(CC) -o process_memory process_memory.c $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o parallel_min_max parallel_sum.o parallel_sum process_memory
