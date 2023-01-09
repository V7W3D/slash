CC = gcc
CFLAGS = -Wall
CLIBRARY = -lreadline

slash : main.o
	$(CC) $(CFLAGS) -o slash main.o cd.o pwd.o mystring.o split_string.o utilslash.o star.o double_star.o $(CLIBRARY) 

main.o : main.c cd.o pwd.o cd.o star.o utilslash.o double_star.o
	$(CC) $(CFLAGS) -c main.c -o main.o $(CLIBRARY) 

pwd.o:pwd.h cd.h mystring.o split_string.o
	$(CC) $(CFLAGS) -c pwd.c -o pwd.o $(CLIBRARY) 

cd.o : cd.c cd.h mystring.o split_string.o
	$(CC) $(CFLAGS) -c cd.c -o cd.o $(CLIBRARY) 

mystring.o : mystring.c mystring.h
	$(CC) $(CFLAGS) -c mystring.c -o mystring.o  $(CLIBRARY) 

split_string.o : split_string.c split_string.h
	$(CC) $(CFLAGS) -c split_string.c -o split_string.o $(CLIBRARY) 

star.o : star.h utilslash.o
	$(CC) $(CFLAGS) -c star.c -o star.o $(CLIBRARY) 

utilslash.o : utilslash.h
	$(CC) $(CFLAGS) -c utilslash.c -o utilslash.o $(CLIBRARY) 

double_star.o : double_star.h 
	$(CC) $(CFLAGS) -c double_star.c -o double_star.o $(CLIBRARY)

clean:
	rm -f *.o slash
