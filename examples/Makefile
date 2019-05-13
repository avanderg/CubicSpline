CC = gcc
CFLAGS = -Wall 

all: test test2

test: cube_spline.o test.o
	$(CC) $(CFLAGS) -o test cube_spline.o test.o ../cube_spline.h
test.o: test.c
	$(CC) $(CFLAGS) -c test.c

test2: cube_spline.o test2.o
	$(CC) $(CFLAGS) -o test2 cube_spline.o test2.o ../cube_spline.h
test2o: test2.c
	$(CC) $(CFLAGS) -c test2.c

cube_spline.o: ../cube_spline.c
	$(CC) $(CFLAGS) -c ../cube_spline.c

clean: 
	rm *.o
