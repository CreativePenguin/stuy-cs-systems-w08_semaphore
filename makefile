ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

# all: control.o write.o
all: control.o
	$(CC) -o control control.o
	# $(CC) write write.o

control.o: control.c
	$(CC) -c control.c

write.o: write.c
	$(CC) -c write.c

clean:
	rm *.o *.out control write
