#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <unistd.h> 
#include <sys/types.h> //See reference 2 for sys/types.h, unistd.h and sys/stat.h
#include <sys/stat.h>
#include <fcntl.h>
#include "movies.h" 

//predefined values determined by assignment constraints 
#define PREFIX "movies_"
#define FILETYPE ".csv" 
#define MAX_RANDOM_VALUE 99999
#define MIN_RANDOM_VALUE 0
#define NEW_DIRECTORY_FORMAT "lamb.movies."

/*
 * Program written by Brandon Lam for Operating Systems 1 class. This program allows for someone to organize a chosen file, the largest file, or the smallest file in the current directory.
 * The files have predetermined formatting rules as outlined by the assignment description, and using these files, new directories will be created containing text files that list all of
 * the movies that were released in that year. Example: ./lamb.movies.1592/1995.txt	where ./ is the current directory and ./lamb.movies.1592/ is a newly created file and 1995.txt contains movies
 * from 1995. In order to actually process individual movie files, code was referenced from assignment 1 in which I already implemented a bubble sorting method to sort movies by year and parsed
 * individual movies from a file. This assignment builds off of assignment 1 by attempting to demonstrate understanding of file and directory manipulation.
 * 
 * References:
 * Much of the more complex code here is derived from the Exploration modules in the CS 344: Operating Systems 1 canvas page.
 *			--There is sample code from Repl.it that is referenced here that comes from the Exploration module. In particular: Directories and Files
 * Some of the other code comes from various places of the C standard library or the linux manual pages.
 * 1) https://man7.org/linux/man-pages/man2/open.2.html <-Reference for file manipulation
 * 2) https://man7.org/linux/man-pages/man2/stat.2.html <-Referenced for reading directory file statistics
 * 3) https://www.cplusplus.com/reference/cstdbool/ <-Referenced but not used.
 * 4) https://canvas.oregonstate.edu/courses/1825887/pages/exploration-files?module_item_id=20268623 <- For working with Directories
 * 5) https://canvas.oregonstate.edu/courses/1825887/pages/exploration-permissions?module_item_id=20268626 <- For Octal Notation
 * 6) https://canvas.oregonstate.edu/courses/1825887/pages/exploration-files?module_item_id=20268623 <- For working with files
 * 7) https://www.man7.org/linux/man-pages/man3/sprintf.3.html <- Usage of sprintf to write formatted output to string
 * 8) https://man7.org/linux/man-pages/man3/random.3.html <- for random numbers
*/

/* Function Declarations */
void showMenu(int);
int mainloop();
int findLargestFile();
int findSmallestFile();
int findCustomFile();
void createNewDirectory(char* filepath);
void createDirectoryTextFiles(char* filepath, struct movie* movieList);
int findCustomFile();

/* Implementation */

int main(int argc, char* argv[]) {
	int userActive = 1;
	int userOption = -1;
	srand(time(0));
	do {
		showMenu(1);
		printf("Please enter a value between 1 and 2.\n");
		scanf("%d", &userOption);
		switch (userOption) {
		case 1:
			mainloop(); //brings us to the submenu
			break;
		case 2:
			printf("Exiting program\n");
			userActive = 0;
			break;
		default: //the error handling case. If the input is not 1 or 2. We assume that users input integers
			printf("\n\n");
			printf("Not a valid selection. Try again.\n");
			break;
		}
	} while (userActive != 0);
	printf("\n");
	printf("Program ending.\n");
	return EXIT_SUCCESS;
}


/*
 * Function: showMenu
 * Description: This is the outermost layer of the main menu. It simply prints text based on the user's input.
 * Parameters: None
 * Output: None
 */
void showMenu(int menuType) {
	if (menuType == 1) {
		printf("\n\n");
		printf("Please enter an integer corresponding to the menu options.\n");
		printf("1. Select a file that you want to be processed.\n");
		printf("2. Exit the program\n");
	}
	else if (menuType == 2) {
		printf("\n\n");
		printf("Please enter an integer corresponding to the file you wish to process.\n");
		printf("1. Process the largest file in the directory.\n");
		printf("2. Process the smallest file in the directory.\n");
		printf("3. Process a file of your choosing.\n");
	}
}


/*
 * Function: mainloop
 * Description: This is the second layer of the main program loop. Here the user can specify what file they want to process from one of three choices.
 * Parameters: None
 * Output: None
 */
int mainloop() {
	int validChoice = 0;
	int validCustomFile = 0;
	int userInput = 0;
	do { //this do while loop runs for the file processing menu
		showMenu(2);
		printf("Please enter a value between 1 and 3.\n");
		scanf("%d", &userInput);
		switch (userInput) { //the three choices the user has for file processing are listed below. Any other intger input should go to default case
		case 1:
			findLargestFile();
			validChoice = 1;
			break;
		case 2:
			findSmallestFile();
			validChoice = 1;
			break;
		case 3:
			validChoice = findCustomFile();
			break;
		default:
			printf("\n\n");
			printf("Not a valid menu selection. Try again.\n\n\n");
			validChoice = 0;
			break;
		}
	} while (validChoice == 0);
}

/*
 * Function: findCustomFile
 * Description: This function gets input from the user and determines whether or not the file name inputted is in the current directory. This input has no strict constraints like
 *				finding the largest file and finding the smallest file. However, the error handling is minimal and the program may break if you search for non-data files.
 *				For example if you were to search main.c it would find it. A simple fix would be to add more error handling but I ran out of time to do so.
 * Parameters: None
 * Output: None
 */

int findCustomFile() {
	char* fileName = calloc(100, sizeof(char)); //allocate memory for the file name
	int directoryFound = 0;
	char filepath[1001] = "./";
	printf("Please enter the name of the file you wish to find: ");
	scanf("%s", fileName);
	DIR* currentDirectory = opendir("./"); //DIR is a directory stream data type which lets us access a directory, specified in the latter half of the line.
	struct dirent* directoryRead; //dirent is a struct defined by DIR which contains member variables useful for reading the directory
	//the program will likely crash if you try to search for certain files, such as "movies" because there is a movies file while the program is running
	while (directoryRead = readdir(currentDirectory)) { //while there are still files to be read
		if (strcmp(fileName, directoryRead->d_name) == 0) { //d_name is a member of the dirent struct which we use to store files found in the directory.
			directoryFound = 1; //using strcmp, we can find exact matches for a custom file desired by the user
		}
	}
	if (directoryFound == 1) {
		printf("Now processing the smallest file in the directory with the name: %s\n", fileName);
	}
	else if (directoryFound == 0) {
		printf("\n");
		printf("Could not find directory with the name: %s. Try again.\n", fileName);
		return 0;
	}
	free(directoryRead);
	closedir(currentDirectory); //we should always close stuff when we are done with it
	strcat(filepath, fileName); //adds the file name to the file path.
	createNewDirectory(filepath); //We pass in the current directory for creating a new directory
	free(fileName);
	return 1;
}

/*
 * Function: findLargestFile
 * Description: This function finds the largest file starting with movies_ and ending with .csv in the current directory per assignment constraints.
 *				If it does not find any files with those parameters then it should return that no files of valid type were found.
 *				At the end of the function we make a call to createNewDirectory because we want to create a new directory containing .txt files for the
 *				largest file.
 * Parameters: None
 * Output: None
 */


int findLargestFile() {
	char* fileName = calloc(100, sizeof(char)); //allocate memory for the file name
	DIR* currentDirectory = opendir("./"); //opens the directory we are currently in and stores it in currentDirectory. DIR is a type representing the directory stream
	struct dirent* directoryRead; //dirent allows us to read from the current directory
	struct stat directoryStats;
	char maxEntry[256];
	int i = 0;
	int currentMax = 0;
	int validFileType;
	int validPrefix;
	char filepath[1001] = "./";

	while (directoryRead = readdir(currentDirectory)) {
		validPrefix = strncmp(PREFIX, directoryRead->d_name, strlen(PREFIX)); //strncmp is a strict comparison based on the length of "movies_" to find
		//d_name is an array of characters within the dirent struct, so we use arithmetic to change the index that the pointer points to
		validFileType = strncmp(FILETYPE, directoryRead->d_name + strlen(directoryRead->d_name) - strlen(FILETYPE), strlen(FILETYPE));
		if (validFileType == 0 && validPrefix == 0) { //if the file starts with movies_ and ends with .csv
			strcpy(maxEntry, "./");
			stat(strcat(maxEntry, directoryRead->d_name), &directoryStats); //these two lines store the current entry and its relevant information
			if (directoryStats.st_size > currentMax) { //this if statement updates the current largest file and its name as necessary
				currentMax = directoryStats.st_size; //st_size is the size of the file and a member variable of the stat struct.
				strcpy(fileName, directoryRead->d_name);
			}
			memset(maxEntry, '\0', 256); //reset the array
		}
	}
	if (currentMax == 0) {
		printf("No files of valid type found\n");
		return 0;
	}
	else {
		printf("Now processing the largest file in the directory with the name: %s\n", fileName);
		strcat(filepath, fileName); //adds the file name to the file path.
		createNewDirectory(filepath); //same as in above function
	}
	free(directoryRead);
	closedir(currentDirectory);
	free(fileName);
}

/*
 * Function: findSmallestFile
 * Description: This function does the same thing as findLargestFile(). Much of the code here is redundant and I was too shortsighted to see that until after I wrote it.
 *				If we wanted to reduce the number of lines of code in the main.c file then what we could do is also pass in the user input from the mainloop() function
 *				and run a switch or if-else block to check whether we want to find the largest or smallest file. The only difference between the two is a couple of variable
 *				names and declarations, and what comparison we are making in the inner most if statement. 
 * Parameters: None
 * Output: None
 */


int findSmallestFile() {
	char* fileName = calloc(100, sizeof(char)); //allocate memory for the file name
	DIR* currentDirectory = opendir("./"); //opens the directory we are currently in and stores it in currentDirectory. DIR is a type representing the directory stream
	struct dirent* directoryRead; //dirent allows us to read from the current directory
	struct stat directoryStats;
	char minEntry[256];
	int i = 0;
	int currentMin = 999;
	int validFileType;
	int validPrefix;
	char filepath[1001] = "./";

	while (directoryRead = readdir(currentDirectory)) {
		validPrefix = strncmp(PREFIX, directoryRead->d_name, strlen(PREFIX)); //strncmp is a strict comparison based on the length of "movies_" to find
		//d_name is an array of characters that is null terminated and stores the name of the file in the directory. If we want to only use strncmp on the last 4 characters .csv
		//then by comparing FILETYPE (.csv) and the last characters of the d_name array before the null terminating symbol against the length of the FILETYPE so its
		validFileType = strncmp(FILETYPE, directoryRead->d_name + strlen(directoryRead->d_name) - strlen(FILETYPE), strlen(FILETYPE));
		if (validFileType == 0 && validPrefix == 0) { //if the file starts with movies_ and ends with .csv
			strcpy(minEntry, "./"); 
			stat(strcat(minEntry, directoryRead->d_name), &directoryStats); //these two lines store the current entry
			if (directoryStats.st_size < currentMin) {
				currentMin = directoryStats.st_size;
				strcpy(fileName, directoryRead->d_name);
			}
			memset(minEntry, '\0', 256); //resets the array so we dont run into issues
		}
	}
	//printf("The smallest file is: %s\n", minEntry);
	if (currentMin == 999) { //simple error handling incase we dont find files
		printf("No files of valid type found\n");
		return 0;
	}
	else {
		printf("Now processing the smallest file in the directory with the name: %s\n", fileName);
		strcat(filepath, fileName); //adds the file name to the file path.
		createNewDirectory(filepath);
	}
	free(directoryRead); 
	closedir(currentDirectory); //we should always close stuff when we are done with it
	free(fileName);
}

/*
 * Function: createNewDirectory
 * Description: This function takes in the path to the file we want to create a directory for and gives it a random value in addition to the proper permissions in octal notation.
 *				The permissions are defined in the assignment constraints and this function creates a new directory in the current directory. The new directory will be empty until
 *				.txt files are created via the createDirectoryTextFiles call towards the end of the function.
 * Parameters: filepath
 * Output: A new directory in the current directory
 */

void createNewDirectory(char* filepath) {
	struct movie* movieList = processFile(filepath); //processFile gets passed the path to the file we want to process
	bubbleSortByYear(movieList); //bubble sort method, see movies.c
	char* directoryName = calloc(18, sizeof(char)); //allocates 18 spaces because the maximum number of characters would be something like lamb.movies.99999 which is 17 + 1
	//the maximum value is 99999, but rand will choose between 0 and 99998 so we must add +1 to get between 0 and 99999. An alternative would've been to define MAX_RAND as 100000
	int randomValue = (rand() % (MAX_RANDOM_VALUE + 1));
	char fileID[18];
	char mainPath[1001] = "./"; //necessary for remembering our current directory as well

	strcat(directoryName, NEW_DIRECTORY_FORMAT);
	sprintf(fileID, "%d", randomValue); //sprintf() formats the randomValue to string pointed to by fileID
	strcat(directoryName, fileID); //adds the random value to the directory name to get the final directory.
	//The assignment states rwxr-x---, meaning owner has all permissions, groups can read and execute, and others have no permissions.
	if (mkdir(directoryName, 0750) == 0) { //the function mkdir() returns 0 if successful. The 0750 represents the permissions in octal notation.
		printf("The directory was successfully created with the name: %s\n", directoryName);
	}
	else {
		printf("Failed to create a directory for the file.\n");
	}
	strcat(mainPath, directoryName);
	createDirectoryTextFiles(mainPath, movieList); 
	free(directoryName);
}

/*
 * Function: createDirectoryTextFiles
 * Description: This function has a double while loop that iterates through our linked list. The function itself creates .txt files for a bubble sorted linked list based on the year
 *				each movie in the file was released. The outer loop creates a .txt file and writes the first movie corresponding to that year that it finds in that .txt file. If there
 *				are multiple movies released that year then we enter the second while loop and iterate through accordingly. This is the deepest level function call of our program and
 *				after the end of this function it wraps all the way back up to the main loop.
 * Parameters: filePath, movieList
 * Output: .txt files in the current directory
*/

void createDirectoryTextFiles(char* filepath, struct movie* movieList) {
	char movieYear[5];
	char* subFilePath = calloc(strlen(filepath) + 20, sizeof(char));
	char movieTitle[256];
	int fileIn;
	//The outer loop creates a text file for a specific year and stores the first movie in there. It then runs the inner loop and then resets all necessary variables.
	while (movieList != NULL) {
		//this code block creates the new text file and resets what needs to be reset at the beginning of the loop
		memset(movieTitle, '\0', sizeof(movieTitle)); //reset
		memset(subFilePath, '\0', sizeof(subFilePath));
		memset(movieYear, '\0', sizeof(movieYear));
		//This block of code before hte next comment establishes a file path that we want created, where we copy any necessary information and format it
		//accordingly so that we can establish a generic file path within the directory. Example: ./lamb.movies.1503/2010.txt which can then be opened for writing
		strcpy(subFilePath, filepath);
		strcat(subFilePath, "/");
		sprintf(movieYear, "%d", movieList->year);
		strcat(subFilePath, movieYear);
		strcat(subFilePath, ".txt");
		//printf("The path to the file is: %s\n", subFilePath);
		//open file for input
		fileIn = open(subFilePath, O_WRONLY | O_CREAT | O_APPEND, 0640); //the permissions to these text files are rw-r----- or 640 in octal notation
		strcpy(movieTitle, movieList->title);
		strcat(movieTitle, "\n"); //Append a newline to the end of title.
		write(fileIn, movieTitle, strlen(movieList->title) + 1); //writes the movie title to the current .txt
		//this code will execute so long as the next movie in the linked list shares the next year.
		while ((movieList->next != NULL) && (movieList->year == movieList->next->year)) {
			memset(movieTitle, '\0', sizeof(movieTitle)); //reset
			strcpy(movieTitle, movieList->next->title);
			strcat(movieTitle, "\n"); //We put the title from the linked list into the array, then establish the newline at the end and write it into the file
			write(fileIn, movieTitle, strlen(movieList->next->title) + 1); //writes the movie title to the current .txt
			movieList = movieList->next;
		}

		movieList = movieList->next;
		memset(movieTitle, '\0', sizeof(movieTitle));
		close(fileIn);
	}
	//free the stuff we allocate in this function
	freeList(movieList);
	free(subFilePath); 
}