mydbtest : mydbtest.o database.o timer.o
	#gcc -ldb -ldb-6.1 -ldb-6 -o mydbtest mydbtest.o database.o timer.o 
	gcc -o mydbtest mydbtest.o database.o timer.o 

mydbtest.o : main.c
	#gcc -ldb -ldb-6.1 -ldb-6 -c main.c
	gcc -D_BSD_SOURCE -std=c99 -c main.c -o mydbtest.o

database.o : database.c database.h
	#gcc -ldb -ldb-6.1 -ldb-6 -c database.c
	gcc -D_BSD_SOURCE -std=c99 -c database.c -o database.o

timer.o : timer.c timer.h
	#gcc -ldb -ldb-6.1 -ldb-6 -c timer.c
	gcc -std=c99 -c timer.c -o timer.o


