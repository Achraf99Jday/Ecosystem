CFLAGS := -g -Wall 
PROGRAMS:= tests_ecosys ecosys

all:  tests_ecosys ecosys

ecosys.o: ecosys.c ecosys.h

main_ecosys.o: main_ecosys.c ecosys.h

main_tests.o: main_tests.c ecosys.h

tests_ecosys: ecosys.o main_tests.o
	$(CC) -o tests_ecosys $(CFLAGS) $^

ecosys: ecosys.o main_ecosys.o
	$(CC) -o ecosys $(CFLAGS) $^


clean:
	rm -f *.o *~ $(PROGRAMS)
