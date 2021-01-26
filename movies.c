#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "movies.h"

// Below is code for functions related to movies. Extremely important functions will have large code blocks.

/*
 * Function: processFile
 * Description: Processes the movie file received from argv[1] at the execution of the program into a linked list and returns it
 * Parameter: filePath
 * Output: Pointer to the first node in a linked list.
 * Additional Information: The code in this function is derived from the sample program for student data found on the
 *						   assignment 1 information page under Resources in the OSU CS344 class.
*/

struct movie* processFile(char* filePath) {
	int numMovies = 0;
	FILE* movieFile = fopen(filePath, "r");
	char* currentLine = NULL;
	size_t len = 0;
	ssize_t nread;
	char* token;

	struct movie* head = NULL;
	struct movie* tail = NULL;
	
	//Reads the file line by line
	//This while loop will construct the linked list.
	nread = getline(&currentLine, &len, movieFile); //skips first line because it is just for formatting in the .csv
	while ((nread = getline(&currentLine, &len, movieFile) != -1)) {
		//Function call to create a new student node whenever we reach a new line in the .csv
		struct movie* newMovie = createMovie(currentLine);
		if (head == NULL) { //conditional that executes for line one in the file
			head = newMovie;
			tail = newMovie;
			//This conditional will set the head and the tail to be the same node
			//This makes sense because this is the only node in our linked list at the time.
		}
		else { //For all other iterations of the while loop until the end of the file
			tail->next = newMovie;
			tail = newMovie;
			//Else conditional ensures that the tail of the linked list is always the node created at the beginning of every while iteration (after iteration 1)
		}
		numMovies++; //increments the number of movies by 1.
	}
	printf("Successfully processed the file: %s, and parsed data for %d movies\n", filePath, numMovies); //Line for the assignment requirements
	printf("\n\n");
	free(currentLine); //we no longer need this. freeing currentLine helps us avoid memory leaks.
	fclose(movieFile); //we should always close files when we are done with them
	return head; //returns the linked list

}

/*
 * Function: createMovie
 * Description: This function allocates memory for an empty link which will be filled in the processFile() function.
 * Parameter: currentLine
 * Output: A new link for the linked list.
 * Additional Information: The code in this function is derived from the sample program for student data found on the
 *						   assignment 1 information page under Resources in the OSU CS344 class.
 *						   Terminology used can also be found under the String Exploration section in Module 2 of the CS344 Canvas Page.
*/

struct movie* createMovie(char* currentLine) {
	//Dynamically allocates space for a link
	struct movie* currentMovie = malloc(sizeof(struct movie));

	//For use with strtok_r, which breaks strings into sequences of non-empty tokens.
	char* saveptr = NULL;

	//Code block for the title token, the first element
	char* token = strtok_r(currentLine, ",", &saveptr);
	currentMovie->title = calloc(strlen(token) + 1, sizeof(char));
	strcpy(currentMovie->title, token);

	//Code block for the year token, the  second element
	token = strtok_r(NULL, ",", &saveptr);
	currentMovie->year = atoi(token);

	currentMovie->next = NULL; //Next node is NULL because it does not exist yet.
	printf("\n");
	return currentMovie;
}

/*
 * Function: bubbleSortByYear
 * Description: This function uses the bubblesort algorithm to return a linked list sorted by year. This is necessary for the sort by year option in the main menu.
 *				Bubblesort is a simple algorithm that repeatedly steps through the list and swaps adjacent components. The running time of this algorithm is
 *				O(n^2) and could be more efficient, but for the scope of the assignment, it is small enough that I don't think it really matters.
 * Parameter: movieList
 * Output: A linked list sorted by year
*/

struct movie* bubbleSortByYear(struct movie* movieList) {
	//establish our variables
	//printf("Commencing bubble sort\n");
	int swapCompleted = 0;
	int i;
	struct movie* head;
	struct movie* nodeLeft = NULL;

	do {
		head = movieList;
		swapCompleted = 1; //change this to true. It is an int but will operate as a boolean in this function
		while (head->next != nodeLeft) { //While loop operates until we reach this specific link
			if (head->year > head->next->year) { //We are swapping by year in ascending order
				swap(head, head->next); //call to swap function
				swapCompleted = 0; //we are not done swapping if this while loop is being called
			}
			head = head->next;
		}
		nodeLeft = head; //the node we are swapping up to in each iteration
	} while (swapCompleted == 0);
}

//A simple swap function that swaps node a and node b. (Swaps the left and right node)
struct movie* swap(struct movie* a, struct movie* b) {
	//These lines declare temp variables to store the values of the node, a.
	int yearHolder = a->year;
	char* titleHolder = a->title;
	
	a->year = b->year;
	a->title = b->title;
	
	//These lines put the data from a into b.
	b->year = yearHolder;
	b->title = titleHolder;
	
}
//frees a list when we are done with it
struct movie* freeList(struct movie* movieList) {
	struct movie* temp; //creates a temporary link so we can free this instead, and not lose access to our position in the linked list
	while (movieList != NULL) { //while the list is not empty
		//printf("%s was freed\n", movieList->title); 
		temp = movieList; //sets the temporary link to the current position
		movieList = movieList->next; //sets the current position to the next position
		//first we free the dynamic content used within each link, then we free the link itself
		free(temp->title);
		free(temp);
	}
}