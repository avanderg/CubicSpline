CC = gcc
CFLAGS = -Wall 

all: test test2

test: spline.o test.o
	$(CC) $(CFLAGS) -o test spline.o test.o ../spline.h
test.o: test.c
	$(CC) $(CFLAGS) -c test.c

test2: spline.o test2.o
	$(CC) $(CFLAGS) -o test2 spline.o test2.o ../spline.h
test2o: test2.c
	$(CC) $(CFLAGS) -c test2.c

spline.o: ../spline.c
	$(CC) $(CFLAGS) -c ../spline.c

clean: 
	rm *.o
