#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>


/* struct for movie information */
struct movie
{
    char* title;
    int year;
    struct movie* next;
};

/* 
 * Show movies released in a specified year
*/
void printMoviesFromYear(struct movie* list, int year) {
    bool moviePrinted = false; 
    while (list != NULL)		// Walk through linked list
    {
        if (list->year == year) {
        printf("%s\n", list->title);
        moviePrinted = true;		// Match found, don't print "no data" message
        }
        list = list->next;
    }
    if (!moviePrinted) {
	// No match found, let user know
        printf("No data about movies released in the year %u", year); 
    }
}

/* Parse the current line which is comma delimited and create a
*  movie struct with the data in this line
*/
struct movie* createMovie(char* currLine)
{
    struct movie* currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char* saveptr;

    // The first token is the title
    char* token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the year
    token = strtok_r(NULL, ",", &saveptr);
    int year = atoi(token);
    currMovie->year = year;

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;
}

/*
*  TODO: Update this function to process data as specified 
*  by Assignment 2 requirements 
*/
struct movie* processFile(FILE* movieFile) {
    
    char* currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    size_t count = 0;

    // The head of the linked list
    struct movie* head = NULL;
    // The tail of the linked list
    struct movie* tail = NULL;

    // Skip the column headers
    nread = getline(&currLine, &len, movieFile);

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get a new movie node corresponding to the current line
        struct movie* newNode = createMovie(currLine);

        // Increment the count of movies in the file
        count += 1;

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
    }
    int desc = fileno(movieFile);
    printf("Process file with descriptor: %d\n", desc);
    free(currLine);
    fclose(movieFile);
    return head;
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
     char fileName[256];
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
