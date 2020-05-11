CC = gcc
CFLAGS = -Wall 
EXE = test test2 test3 test4

all: test test2 test3 test4 

run: test test2 test3 test4
	./test
	./test2
	./test3
	./test4

run_check: test test2 test3 test4
	valgrind ./test
	valgrind ./test2
	valgrind ./test3
	valgrind ./test4

test: cube_spline.o test.o
	$(CC) $(CFLAGS) -o test cube_spline.o test.o ../cube_spline.h
test.o: test.c
	$(CC) $(CFLAGS) -c test.c

test2: cube_spline.o test2.o
	$(CC) $(CFLAGS) -o test2 cube_spline.o test2.o ../cube_spline.h
test2.o: test2.c
	$(CC) $(CFLAGS) -c test2.c

test3: cube_spline.o test3.o
	$(CC) $(CFLAGS) -o test3 cube_spline.o test3.o ../cube_spline.h
test3.o: test3.c
	$(CC) $(CFLAGS) -c test3.c

test4: cube_spline.o test4.o
	$(CC) $(CFLAGS) -o test4 cube_spline.o test4.o ../cube_spline.h
test4.o: test3.c
	$(CC) $(CFLAGS) -c test4.c

cube_spline.o: ../cube_spline.c
	$(CC) $(CFLAGS) -c ../cube_spline.c


clean: 
	rm -f *.o $(EXE)
