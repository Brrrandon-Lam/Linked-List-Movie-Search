#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Struct definition for movie is below with 5 variables, including
 *a pointer to the next movie for linked list functionality*/
struct movie {
	char* title;
	int year;
	//float rating; //rating and language are not part of assignment 2, but they are still part of the movie information.
	//char language[5][21]; //For the sake of assignment 2, they will not be used
	struct movie* next;
};

/*Functions declarations pertaining to movies.c*/
struct movie* processFile(char* filePath);
struct movie* createMovie(char* currentLine);
struct movie* bubbleSortByYear(struct movie* movieList);
struct movie* swap(struct movie*, struct movie* b);
struct movie* freeList(struct movie*);
