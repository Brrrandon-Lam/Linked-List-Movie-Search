CC=gcc --std=gnu99
movies_by_year: main.o movies.o
	$(CC) main.o movies.o -o movies_by_year
main.o: main.c
	$(CC) -c main.c
movies.o: movies.c movies.h
	$(CC) -c movies.c
clean:
	rm -f *.o movies_by_year