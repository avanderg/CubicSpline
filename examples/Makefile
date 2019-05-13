CC = gcc
CFLAGS = -Wall 

all: test
test: spline.o test.o
	$(CC) $(CFLAGS) -o test spline.o test.o ../spline.h
spline.o: ../spline.c
	$(CC) $(CFLAGS) -c ../spline.c
test.o: test.c
	$(CC) $(CFLAGS) -c test.c
clean: 
	rm *.o
