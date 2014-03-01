#Enables C++11
PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64


all: simulate.c assemble.c
	gcc -lm simulate.c -o simulate
	gcc -lm assemble.c -o assemble

test: test.doge
	./assemble test.doge test.mc
	./simulate test.mc

# make clean - remove .o files.  You can modify it to remove whatever
clean:
	rm -f *.o assemble simulate
