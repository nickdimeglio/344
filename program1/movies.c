#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* struct for movie information */
struct movie
{
    char* title;
    char* year;
    char* languages;
    char* ratingValue;
    struct movie* next;
};

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
    currMovie->year = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->year, token);

    // The next token is the language(s)
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->languages, token);

    // The next token is the rating value
    // (this token will have a newline character to trim)
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->ratingValue = calloc(strlen(token), sizeof(char));
    strncpy(currMovie->ratingValue, token, strlen(token) - 1);

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;
}

/* TODO: Show movies released in a specified year
*/

/* TODO: Show highest rated movie for each year
*/

/* TODO: Show the title and year of release of all movies
 * in a specified language
*/

/*
* Return a linked list of movies by parsing
* data from each line of the specified file.
*/
struct movie* processFile(char* filePath)
{
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

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
    free(currLine);
    fclose(movieFile);
    printf("\nProcessed file %s and parsed data for %zu movies", filePath, count);
    return head;
}

/*
* Print data for the given movie
*/
void printMovie(struct movie* aMovie){
    printf("%s, %s, %s, %s\n", aMovie->title, aMovie->year, aMovie->languages, aMovie->ratingValue);
}

/*
* Print the linked list of movie
*/
void printMovieList(struct movie* list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}

/*
*   Process the file provided as an argument to the program to
*   create a linked list of movie structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o movies main.c
*/

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies.csv\n");
        return EXIT_FAILURE;
    }
    struct movie* list = processFile(argv[1]);
    printf("\n\n");
    printMovieList(list);
    printf("\n\n");
    return EXIT_SUCCESS;
}

