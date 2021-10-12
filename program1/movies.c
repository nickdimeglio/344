#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* struct for movie information */
struct movie
{
    char* title;
    int year;
    char* languages;
    double ratingValue;
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
    int year = atoi(token);
    currMovie->year = year;

    // The next token is the language(s)
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->languages, token);

    // The next token is the rating value
    // (this token will have a newline character to trim)
    token = strtok_r(NULL, "/n", &saveptr);
    double ratingValue = strtod(token, &saveptr);
    currMovie->ratingValue = ratingValue;

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;
}

/*
* Print data for the given movie
*/
void printMovie(struct movie* aMovie){
    printf("%s, %d, %s, %.1f\n", aMovie->title, aMovie->year, aMovie->languages, aMovie->ratingValue);
}


/* 
 * Show movies released in a specified year
*/
void printMoviesFromYear(struct movie* list, int year)
{
    bool moviePrinted = false; 
    while (list != NULL)
    {
        if (list->year == year) {
        printf("%s\n", list->title);
        moviePrinted = true;
        }
        list = list->next;
    }
    if (!moviePrinted) {
        printf("No data about movies released in the year %u", year);
    }

}


/* 
 * Show highest rated movie for each year
*/
void printTopMovies(struct movie* list) {
    struct movie* topMovies[122] = {NULL}; // 122 Possible years (1900-2021)

    // Build an array of top movies
    while (list != NULL) {
        int yearIndex = list->year - 1900;
        if (topMovies[yearIndex] == NULL || list->ratingValue > topMovies[yearIndex]->ratingValue) {
            topMovies[yearIndex] = list;
        } 
        list = list->next;
    }
   
    // Display the results
    for (size_t i = 0; i < 122; i++) {
        struct movie* m = topMovies[i];
        if (m != NULL) {
            printf("%d %.1f %s\n", m->year, m->ratingValue, m->title);
        }
    }

}

/* 
 * Show the title and year of release of all movies
 * in a specified language
*/
void printMoviesInLanguage(struct movie* list, char* lang) {
    
    bool moviePrinted = false;
    // Walk through movies
    while (list != NULL) {
    

        // For use with strtok_r
        char* saveptr;
        char* languages = calloc(strlen(list->languages) + 1, sizeof(char));
        strcpy(languages, list->languages);

        // Check each of the movie's languages for a match 
        char* token = strtok_r(languages, "[];", &saveptr);

        while (token != NULL) {
            // Print movie if match is found
            if (strcmp(lang, token) == 0) {
                printf("%d %s\n", list->year, list->title);
                moviePrinted = true;
            }
            token = strtok_r(NULL, "[];", &saveptr);
        }
        list = list->next;
    }
    if (!moviePrinted) {
        printf("No data about movies released in %s", lang);
    }
}

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
    printf("\n\nProcessed file %s and parsed data for %zu movies", filePath, count);
    return head;
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

    // Create movie linked list
    struct movie* list = processFile(argv[1]);

    for (;;) { 
        // Provide choices for data 
        printf("\n\n1. Show movies released in the specified year"); 
        printf("\n2. Show highest rated movie for each year");
        printf("\n3. Show the title and year of release of all movies in a specific language");
        printf("\n4. Exit from the program");

        // Request choice
        int choice; 
        printf("\n\nEnter a choice from 1 to 4: ");
        scanf("%d", &choice);

        // Show requested data
        int year;
        char lang[20];

        switch (choice) {
            case 1: 
                // Show movies released in the specified year
                printf("Enter the year for which you want to see movies: ");
                scanf("%d", &year);
                printMoviesFromYear(list, year);
                break;
            case 2: 
                // Show highest rated movie for each year
                printTopMovies(list);
                break;
            case 3: 
                // Show the title and year of release of all movies in a specific language
                printf("Enter the language for which you want to see movies: ");
                scanf("%s", lang);
                printMoviesInLanguage(list, lang);
                break;
            case 4: 
                // Exit from the program
                return EXIT_SUCCESS;
            default:
                // Invalid input
                printf("You entered an incorrect choice. Try again.");
                break;
        }
    }
}
