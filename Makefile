CC = gcc
CFLAGS = -Wall
CLIBRARY = -lreadline

slash : main.o
	$(CC) $(CFLAGS) -o slash main.o cd.o pwd.o mystring.o split_string.o $(CLIBRARY)

main.o : main.c cd.o pwd.o
	$(CC) $(CFLAGS) -c main.c -o main.o $(CLIBRARY)

pwd.o:pwd.h cd.h mystring.o split_string.o
	$(CC) $(CFLAGS) -c pwd.c -o pwd.o $(CLIBRARY)

cd.o : cd.c cd.h mystring.o split_string.o
	$(CC) $(CFLAGS) -c cd.c -o cd.o $(CLIBRARY)

mystring.o : mystring.c mystring.h
	$(CC) $(CFLAGS) -c mystring.c -o mystring.o  $(CLIBRARY)

split_string.o : split_string.c split_string.h
	$(CC) $(CFLAGS) -c split_string.c -o split_string.o $(CLIBRARY) 

clean:
	rm -f *.o slash