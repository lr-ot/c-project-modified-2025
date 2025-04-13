#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct film {
    int filmID;
    char filmName[200];
    char filmDirector[100];
    char filmReleaseDate[20];
    char filmRating[10];
    double costPerRental;
    int numOfMonthlyViews;
    int currentlyRentedID;
    char rentalExpiration[20];
    struct film *next;
};
 
struct film *AddEntry(struct film*);
void BrowseEntries(struct film*);
struct film *EditEntry(struct film*);
struct film *DeleteEntry(struct film*);
void SearchForTitle(struct film*);
void TallyMonthlyEarnings(struct film*);
struct film *SortByFilmName(struct film*);
void SaveToFile(struct film*);
struct film *LoadFromFile();
struct film *ModifyData(struct film*);
void DisplayData(struct film*);
struct film *MemoryCleanUp(struct film*);

int main () {
    struct film *start = NULL;
    int ch;
    int choice = 0;

    //prompt to load database or start new
    puts("Do you want to 1: load a database or 2: start new ");
    scanf("%d", &choice);
    while ( ( ch = getchar() ) != '\n' && ch != EOF );   

    //if user chooses to load database, call load function
    if (choice == 1) {
        start = LoadFromFile();
        if (start == NULL) {
            puts("No data loaded. Starting with an empty database.");
        }
    }

    while (choice != 10) {
        //prompt user for choice
        puts("---Film Tracker----------------------------------------");
        puts("| 1. Save data to file                                |");
        puts("| 2. Load data from file                              |");
        puts("-------------------------------------------------------");
        puts("| 3. Add new film entry   | 7. Browse film entries    |");
        puts("| 4. Edit film entry      | 8. Search film by title   |");
        puts("| 5. Delete film entry    | 9. Tally monthly earnings |");
        puts("| 6. Sort films by name   | 10. Exit                  |");
        puts("-------------------------------------------------------");
        puts("Enter your choice: ");
        scanf("%d", &choice);
        while ( ( ch = getchar() ) != '\n' && ch != EOF );   

        //case statement to call function for choice made
        switch (choice) {
            case 1: SaveToFile(start); break;
            case 2: MemoryCleanUp(start); start = LoadFromFile(); if (start == NULL) { puts("No data loaded. Starting with an empty database."); } break;
            case 3: start = AddEntry(start); break;
            case 4: start = EditEntry(start); break;
            case 5: start = DeleteEntry(start); break;
            case 6: start = SortByFilmName(start); break;
            case 7: BrowseEntries(start); break;
            case 8: SearchForTitle(start); break;
            case 9: TallyMonthlyEarnings(start); break;
            case 10: start = MemoryCleanUp(start); exit(0);
        }
    }

    //clean up memory
    start = MemoryCleanUp(start);

    puts("finished");  

    return 0;
}

struct film *AddEntry(struct film* start) {
    struct film *move = NULL, *new = NULL;
    int ch;
    int newID = 1;

    //allocate new node
    new = (struct film*)malloc(sizeof(struct film));
    if (new == NULL) {
        puts("memory error");
        start = MemoryCleanUp(start);
        exit(1);
    }

    if (start == NULL) {
        //assign address in new to start
        start = new;
    } else {
        //loop through the list until we reach the last node.
        //allocate new memory, tie into the list
        move = start;
        while (move->next != NULL) {
            move = move->next;
        }
        //move is now positioned at the last node of the existing list
        newID = move->filmID + 1;
        //tie it into the existing list
        move->next = new;
    }

    //set ID to new node
    new->filmID = newID;
    new->next = NULL;

    //modify values in new node
    ModifyData(new);
            
    return start;
}

void BrowseEntries(struct film* start){
    struct film *move = start;

    puts("---\nBrowse film entries");  

    //return if node is empty
    if (move == NULL) {
        printf("Data not found\n");
        return;
    }
    
    //loop through linked list until it finds the terminal node
    while (move->next != NULL) {
        //print data
        DisplayData(move);
        move = move->next;
    }

    //print data at last node
    DisplayData(move);
    move = move->next;

    puts("---\nPress ENTER to continue");  
    getchar();    
}

struct film *EditEntry(struct film* start) {
    struct film *move = start;
    int ch;
    int idToEdit = 0;

    puts("---\nEdit film entry");  

    //return if node is empty
    if (move == NULL) {
        printf("Data not found\n");
        return start;
    }

    //prompt for film ID to edit
    puts("Enter ID of entry to edit:"); 
    scanf("%d", &idToEdit);
    while ( ( ch = getchar() ) != '\n' && ch != EOF );
    
    //traverse list until given position
    while (move != NULL && move->filmID != idToEdit) {
        move = move->next;
    }

    //edit node if position found
    if (move != NULL) {
        DisplayData(move);
        ModifyData(move);
    } else {
        //otherwise print error message
        printf("Data not found\n");
    }

    return start;
}

struct film *DeleteEntry(struct film* start) { 
    struct film *move = start, *prev = NULL;
    int ch;
    int idToDelete = 0;

    puts("---\nDelete film entry");  

    //return if node is empty
    if (move == NULL) {
        printf("Data not found\n");
        return start;
    }

    //prompt for film ID to delete
    puts("Enter ID of entry to delete:"); 
    scanf("%d", &idToDelete);
    while ( ( ch = getchar() ) != '\n' && ch != EOF );

    //if start node needs to be deleted
    if (start->filmID == idToDelete) {
        start = move->next;
        free(move);
        printf("Entry %d deleted.\n", idToDelete);
        return start;
    }

    //traverse list until given position
    while (move != NULL && move->filmID != idToDelete) {
        prev = move;
        move = move->next;
    }

    //delete node if position found
    if (move != NULL) {
        prev->next = move->next;
        free(move);
        printf("Entry %d deleted.\n", idToDelete);
    } else {
        //otherwise print error message
        printf("Data not found\n");
    }

    return start;
}

void SearchForTitle(struct film* start) { 
    struct film *move = start;
    char filmSearchQuery[200];
    int ch;
    int i = 0;

    puts("---\nSearch film by title");  

    //return if node is empty
    if (move == NULL) {
        printf("Data not found\n");
        return;
    }

    //prompt for film title to search
    puts("Enter title of film to search:"); 
    fgets(filmSearchQuery, 200, stdin);
    filmSearchQuery[strcspn(filmSearchQuery, "\n")] = '\0';

    //loop through linked list until it finds the terminal node
    while (move != NULL) {
        //if string of search query is contained within film name
        if ( strncmp(move->filmName, filmSearchQuery, strlen(filmSearchQuery)) == 0) {
            //display data at node
            DisplayData(move);
            //add to number of matches found
            i++;
        }
        move = move->next;
    }

    //print number of matches found
    printf("---\n%d match(es) found\n", i);
    puts("---\nPress ENTER to continue");  
    getchar();   
}

void TallyMonthlyEarnings(struct film* start){
    struct film *move = start;
    double monthlyTotal = 0.00;

    puts("---\nTally monthly earnings");  

    //return if node is empty
    if (move == NULL) {
        printf("Data not found\n");
        return;
    }

    //loop through linked list until it finds the terminal node
    while (move->next != NULL) {
        //add amount at node to monthly total
        monthlyTotal += (move->costPerRental * move->numOfMonthlyViews);
        move = move->next;
    }

    //add amount at last node to monthly total
    monthlyTotal += (move->costPerRental * move->numOfMonthlyViews);
    move = move->next;

    //print monthly earnings
    printf("Monthly earnings: $%.2lf\n", monthlyTotal);
}

struct film *SortByFilmName(struct film* start) {
    puts("---\n Sort films by name");  
    if (start == NULL || start->next == NULL) {
        // List is empty or has only one node, no need to sort
        return start;
    }

    struct film *sorted = NULL; // New sorted list
    struct film *current = start;

    while (current != NULL) {
        struct film *next = current->next; // Save the next node

        // Insert the current node into the sorted list
        if (sorted == NULL || strcmp(current->filmName, sorted->filmName) < 0) {
            // Insert at the beginning of the sorted list
            current->next = sorted;
            sorted = current;
        } else {
            // Find the correct position in the sorted list
            struct film *temp = sorted;
            while (temp->next != NULL && strcmp(temp->next->filmName, current->filmName) < 0) {
                temp = temp->next;
            }
            // Insert the current node in the sorted list
            current->next = temp->next;
            temp->next = current;
        }

        current = next; // Move to the next node
    }

    puts("List was sorted alphabetically by film name");
    return sorted;
}

void SaveToFile(struct film* start) {
    puts("---\nSave data to file");  
    char filename[100];

    //prompt user for filename
    puts("Enter the filename to save the data: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    //open file for writing
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        puts("Error opening file for writing");
        return;
    }

    struct film* move = start;

    //loop through linked list until it finds the terminal node
    while (move != NULL) {
        //write data to file
        fprintf(file, "%d\n%s\n%s\n%s\n%s\n%.2lf\n%d\n%d\n%s\n",
                move->filmID,
                move->filmName,
                move->filmDirector,
                move->filmReleaseDate,
                move->filmRating,
                move->costPerRental,
                move->numOfMonthlyViews,
                move->currentlyRentedID,
                move->rentalExpiration);
        move = move->next;
    }

    fclose(file);

    printf("Data saved successfully to '%s'.\n", filename);
}

struct film *LoadFromFile() {
    char filename[100];
    char buffer[200];
    struct film *start = NULL, *move = NULL, *new = NULL;

    puts("---\nLoad data from file");  

    // Prompt the user for the filename
    puts("Enter the filename to load the data from:");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        puts("Error opening file for reading.");
        return NULL;
    }

    // Read data from the file and populate the linked list
    while (fgets(buffer, sizeof(buffer), file)) {
        new = (struct film *)malloc(sizeof(struct film));
        if (new == NULL) {
            puts("memory error");
            fclose(file);
            return start;
        }

        // Read and populate the fields of the film
        new->filmID = atoi(buffer);
        fgets(new->filmName, sizeof(new->filmName), file);
        new->filmName[strcspn(new->filmName, "\n")] = '\0';
        fgets(new->filmDirector, sizeof(new->filmDirector), file);
        new->filmDirector[strcspn(new->filmDirector, "\n")] = '\0';
        fgets(new->filmReleaseDate, sizeof(new->filmReleaseDate), file);
        new->filmReleaseDate[strcspn(new->filmReleaseDate, "\n")] = '\0';
        fgets(new->filmRating, sizeof(new->filmRating), file);
        new->filmRating[strcspn(new->filmRating, "\n")] = '\0';
        fgets(buffer, sizeof(buffer), file);
        new->costPerRental = atof(buffer);
        fgets(buffer, sizeof(buffer), file);
        new->numOfMonthlyViews = atoi(buffer);
        fgets(buffer, sizeof(buffer), file);
        new->currentlyRentedID = atoi(buffer);
        fgets(new->rentalExpiration, sizeof(new->rentalExpiration), file);
        new->rentalExpiration[strcspn(new->rentalExpiration, "\n")] = '\0';

        new->next = NULL;

        // Add the new node to the linked list
        if (start == NULL) {
            start = new;
        } else {
            move = start;
            while (move->next != NULL) {
                move = move->next;
            }
            move->next = new;
        }
    }

    fclose(file);
    puts("Data loaded successfully.");
    return start;
}

struct film *ModifyData(struct film* new){
    int ch;

    //display film ID
    printf("\nFilm ID: %d\n", new->filmID);

    //get film name
    puts("Enter film name:"); 
    fgets(new->filmName, 200, stdin);
    new->filmName[strcspn(new->filmName, "\n")] = '\0';

    //get film director
    puts("Enter film director:");
    fgets(new->filmDirector, 100, stdin);
    new->filmDirector[strcspn(new->filmDirector, "\n")] = '\0';
    
    //get film rating
    puts("Enter film rating:"); 
    fgets(new->filmRating, 20, stdin);
    new->filmRating[strcspn(new->filmRating, "\n")] = '\0';
    
    //get film release date
    puts("Enter film release date:");
    fgets(new->filmReleaseDate, 20, stdin);
    new->filmReleaseDate[strcspn(new->filmReleaseDate, "\n")] = '\0';

    //get cost per rental
    new->costPerRental = 5.99;

    //get number of monthly views
    puts("Enter number of monthly views:"); 
    scanf("%d", &new->numOfMonthlyViews);
    while ( ( ch = getchar() ) != '\n' && ch != EOF );

    //get id of customer currently renting
    puts("Enter ID of customer currently renting:"); 
    scanf("%d", &new->currentlyRentedID);
    while ( ( ch = getchar() ) != '\n' && ch != EOF );

    //get rental expiration date
    puts("Enter rental expiration date:");
    fgets(new->rentalExpiration, 20, stdin);
    new->rentalExpiration[strcspn(new->rentalExpiration, "\n")] = '\0';

    return new;
}

void DisplayData(struct film* move) {
    //print film data
    printf("---\n");
    printf("Film ID: %d\n", move->filmID);
    printf("Film Name: %s\n", move->filmName);
    printf("Film Director: %s\n", move->filmDirector);
    printf("Film Rating: %s\n", move->filmRating);
    printf("Film Release Date: %s\n", move->filmReleaseDate);
    printf("Cost Per Rental: $%.2lf\n", move->costPerRental);
    printf("Monthly Views: %d\n", move->numOfMonthlyViews);
    printf("Currently Rented By Customer ID: %d\n", move->currentlyRentedID);
    printf("Rental Expiration Date: %s\n", move->rentalExpiration);
}

struct film *MemoryCleanUp(struct film* start) {
    if (start == NULL) {
        printf("No memory to free\n");
        return NULL;
    }
    //loop through the LL until it finds the terminal node
    struct film* move = start;

    while (move->next != NULL) {
        struct film* last = move;
        move = move->next;
        free(last);
    }

    //free last node
    free(move);

    return NULL;
}