#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
*   TODO: Find the largest file
*   Finds the largest file with the extension csv in the current
*   directory whose name starts with the prefix 'movies_' and
*   then returns a stream with that file open to read.
*/
FILE* getLargest() {
    char fileName[] = "movies.csv";
    FILE* movieFile = fopen(fileName, "r");
    return movieFile; 
}

/* 
 *  TODO: Find the smallest file
*   Finds the smallest file with the extension csv in the current
*   directory whose name starts with the prefix 'movies_' and
*   then returns a stream with that file open to read.
*/
FILE* getSmallest() {
    char fileName[] = "movies.csv";
    FILE* movieFile = fopen(fileName, "r");
    return movieFile; 
}

/* 
 *  TODO: Find the specified file, error check
*   Returns a read-only stream for the file name specified 
*/
FILE* getByName() {
    char fileName[] = "movies.csv";
    FILE* movieFile = fopen(fileName, "r");
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

        // Branch accordingly
        FILE* movieFile = NULL;
        switch (choice) {
            case 1:
                movieFile = getLargest(); 
                break;
            case 2:
                movieFile = getSmallest();
                break;
            case 3:
                movieFile = getByName();
                break;
            default:
                printf("You entered an incorrect choice. Please try again.\n");
                break;
        }
        if (movieFile != NULL) {
            processFile(movieFile);
            break;
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
