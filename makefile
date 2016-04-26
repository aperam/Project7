CC= g++
CCFLAGS= -g -w

exec: main.o makefile
	$(CC) $(CCFLAGS) -o Aprog main.o

main.o: main.C
	$(CC) $(CCFLAGS) -c main.C

clean:
	rm -rf *.o
