Program by Brandon Lam (lamb) written for CS 344 at Oregon State University

To run the program:
1) Download all of the files (.csv files are optional)
	-Necessary files are main.c, movies.c, movies.h
	-If you downloaded the Makefile, skip to step 3
	-If you did not download the Makefile, follow step 2 and skip step 3
2) In terminal type:
	gcc --std=gnu99 -o movies_by_year main.c movies.c movies.h
3) Type make
	-make clean if necessary
4) Type ./movies_by_year in your terminal and the program will run.

Important note: Make sure that you at least have one .csv file or other file in the correct format in the same directory as
		the other files, or else it will not run properly.
		If the data files do not follow the correct format, they will also not be processed properly.
		If you do not have valid data files to work with, I recommend using the ones provided.