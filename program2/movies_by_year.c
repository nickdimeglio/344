#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

/*
*  TODO: Create a new directory named dimeglin.movies.random, where random
*  is an integer in the range [0, 999999] with permissions rwxr-x---. 
*  Within the new directory, and using the provided csv, create one file for 
*  each year in which at least one movie was released. Within each file, write
*  the titles for all movies released in that year with one on each line.
*/
void processFile(FILE* movieFile) {
    
    char* currLine = NULL;
    size_t len = 0;
    ssize_t nread;

    /* 
     * Create a new directory titled dimeglin.movies.random
    */
    // srand(time(0));
    int randInt = rand() % 999999;      
    char dirName[23];
    sprintf(dirName, "dimeglin.movies.%d", randInt);
    mkdir(dirName, 0750);
//
//    /*
//     * Log the file's movies line by line
//    */
//    nread = getline(&currLine, &len, movieFile); // Skip the column headers
//    while ((nread = getline(&currLine, &len, movieFile)) != -1) {
//
//        // Retrieve the title and year from the current line
//        char* saveptr;
//
//        // The first token is the title
//        char* token = strtok_r(currLine, ",", &saveptr);
//        char* title = calloc(strlen(token) + 1, sizeof(char));
//        strcpy(title, token);
//
//        // The next token is the year
//        token = strtok_r(NULL, ",", &saveptr);
//        char* year = calloc(strlen(token) + 1, sizeof(char));
//        strcpy(year, token);
//
//        /*
//         * Write the title on its own line in the file year.txt
//        */
//
//    }
//    free(currLine);
//    fclose(movieFile);

}


/* 
 *  Returns a stream referring to either the smallest (if findSmallest = true) 
 *  or the largest (if findSmallest = False) csv file in the provided directory
 *  whose name starts with the prefix 'movies_'  
*/
FILE* getBySize(DIR* searchDir, bool findSmallest) {
    char* frontRunner = calloc(256, sizeof(char)); // Max filename size is 256
    long frontRunnerSize = 0L;
    struct dirent* curr = NULL;
    int fileCount = 0;

    // Loop through directory to find the largest movie csv 
    while ((curr = readdir(searchDir)) != NULL) {

        // Skip entry if it is a directory
        if (curr->d_type == DT_DIR) continue;

        // Skip entry if it doesn't start with "_movies" 
        if ((strncmp("movies_", curr->d_name, 7) != 0)) continue;
        
        // Skip entry if it doesn't end with ".csv" 
        char* extension = strrchr(curr->d_name, '.');
        if (!extension || strcmp(".csv", extension) != 0) continue;

        // Get the file's size in bytes
        FILE* csv = fopen(curr->d_name, "r");
        fseek(csv, 0L, SEEK_END);
        long size = ftell(csv);

        // Update the front runner if needed
        if ((fileCount == 0) || (findSmallest && size < frontRunnerSize) || (!findSmallest && size > frontRunnerSize)) {
            strcpy(frontRunner, curr->d_name);
            frontRunnerSize = size;
        } 
        fileCount++;
    } 
    FILE* movieFile = fopen(frontRunner, "r");
    if (!movieFile) {
        // No File Found
        printf("\nThere were no csv files starting with '_movies', sorry.");
    } else {
        // Display name of file to be processed
        printf("Now processing the chosen file named %s\n", frontRunner);
    }
    return movieFile; 
}

 /* 
 *  TODO: Find the specified file, error check
 *  Returns a read-only stream for the file name specified 
 */
 FILE* getByName() {
     char fileName[256] = {"\0"};
     // Prompt user to provide a file name
     printf("\nEnter the complete file name: ");
     scanf("%s", fileName);

     // Return file stream 
     FILE* movieFile = fopen(fileName, "r");
     if (!movieFile) {
        // No File Found 
        printf("The file %s was not found. Try again\n", fileName);
     } else {
        // Display name of file to be processed
        printf("Now processing the chosen file named %s\n", fileName);
     }
     return movieFile; 
 }
 
 
 
 /*
  *  Prompt the user to select between processing the largest file
  *  in the current directory, the smallest file in the current
  *  directory, or a specific file by name.
 */
 void selectFile() {
 
     // File selection menu
     for (;;) {
         printf("\nWhich file do you want to process?");
         printf("\nEnter 1 to pick the largest file");
         printf("\nEnter 2 to pick the smallest file");
         printf("\nEnter 3 to specify the name of a file");

         // Record choice
         int choice;
         printf("\n\nEnter a choice from 1 to 3: ");
         scanf("%d", &choice);
 
         // Open the current directory for searching, then branch to find the right file
        FILE* movieFile = NULL;
        DIR* searchDir = opendir("."); 

        switch (choice) {
            case 1: movieFile = getBySize(searchDir, false); break;  // Get the largest file
            case 2: movieFile = getBySize(searchDir, true); break;   // Get the smallest
            case 3: movieFile = getByName(); break;                  // Get file by name
            default:
                printf("You entered an incorrect choice. Please try again.\n");
                break;
        }

        closedir(searchDir);

        if (movieFile != NULL) {
            processFile(movieFile);
            return;
        } 
    }
}

/*
 *  Provide the user with a main menu, where they can
 *  either select a file to process or exit the program.
*/

int main(int argc, char *argv[])
{
    // Main menu
    for (;;) { 
        printf("\n1. Select a file to process");
        printf("\n2. Exit the program"); 

        // Record choice
        int choice; 
        printf("\n\nEnter a choice 1 or 2: ");
        scanf("%d", &choice);

        // Branch accordingly
        switch (choice) {
            case 1: 
                    selectFile(); 
                    break;
            case 2: 
                    return EXIT_SUCCESS;
            default:
                    // Incorrect selection
                    printf("You entered an incorrect choice. Please try again.\n");
                    break;

        }
    }
}
