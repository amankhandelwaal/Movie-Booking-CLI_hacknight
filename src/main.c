#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    char name[50];
    char email[50];
    char mobile[10];
    char row;
    int col;
    char movie_selected[50];
} Details;

typedef struct
{
    char *movie_name;
    char seats[10][15][4];
} Theatre;

Theatre *list = NULL;
Details *dynamic_array = NULL;
int count = 0;

Theatre one, two, three, four, five;

void PrintMenu()
{
    printf("\033[1;36m");
    printf("   ╔════════════════════════════════════════════════════════════╗\n");
    printf("   ║                                                            ║\n");
    printf("   ║                         BOOK YOUR SHOW                     ║\n");
    printf("   ║                                                            ║\n");
    printf("   ║  Please select an option from below:                       ║\n");
    printf("   ║                                                            ║\n");
    printf("   ║    (1) Enter Details                                       ║\n");
    printf("   ║    (2) Show Details                                        ║\n");
    printf("   ║    (3) Book Movies                                         ║\n");
    printf("   ║    (4) Generate Bill                                       ║\n");
    printf("   ║    (5) Exit                                                ║\n");
    printf("   ║                                                            ║\n");
    printf("   ╚════════════════════════════════════════════════════════════╝\n");
    printf("   > ");
    printf("\033[0m");
}

void InputDetails()
{
    Details *temp = realloc(dynamic_array, (count + 1) * sizeof(Details));
    if (temp == NULL)
    {
        printf("\n Memory allocation failed !");
        return;
    }
    dynamic_array = temp;

    printf("   >>> Enter your name: ");
    scanf(" %[^\n]", dynamic_array[count].name);
    getchar();
    printf("   >>> Enter your email address: ");
    scanf("%s", dynamic_array[count].email);
    getchar();
    printf("   >>> Enter mobile number: ");
    scanf("%s", dynamic_array[count].mobile);
    getchar();

    FILE *file = fopen("data.csv", "a");
    if (!file)
    {
        printf("Could not open file data.csv for writing.\n");
        return;
    }

    // Write user details to the CSV with placeholders for movie selection and seat information
    fprintf(file, "%s,%s,%s,%s,%c,%d\n", dynamic_array[count].name, dynamic_array[count].email, dynamic_array[count].mobile, "None", '-', 0);
    fclose(file);

    count++;
}

void ShowDetails()
{
    printf("   >>> Enter first or last name: ");
    char search[50];
    scanf("%s", search);
    int found = 0;
    for (int i = 0; i < count; i++)
    {
        if (strstr(dynamic_array[i].name, search) != NULL)
        {
            printf("   Name: %s\n   Mobile: %s\n   Email: %s\n   Movie: %s\n   Seat: %c-%d\n",
                   dynamic_array[i].name, dynamic_array[i].mobile, dynamic_array[i].email,
                   dynamic_array[i].movie_selected, dynamic_array[i].row, dynamic_array[i].col);
            found = 1;
        }
    }
    if (!found)
    {
        printf("   No details found\n");
    }
}

void ReadCSVAndUpdateSeats(const char *filename, Theatre *theatre, char *moviename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Could not open file %s for reading.\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        char movie[50], row;
        int col;
        sscanf(line, "%*[^,],%*[^,],%*[^,],%[^,],%c,%d", movie, &row, &col);
        if (strcmp(movie, moviename) == 0 && row != '-' && col != 0)
        {
            int row_index = row - 'A';
            if (row_index >= 0 && row_index < 10 && col >= 1 && col <= 15)
            {
                strcpy(theatre->seats[row_index][col - 1], "[X]");
            }
        }
    }
    fclose(file);
}

void DisplaySeats(Theatre *theatre, char *movie_name)
{
    printf("   You have chosen the movie %s\n", movie_name);
    printf("   Select a Seat!\n");

    printf("\t\tRecliner $500\n");
    for (int i = 9; i > 8; i--)
    {
        printf("\033[38;5;250m\t\t%c ", 'A' + i);
        printf("\033[0m");
        for (int j = 0; j < 15; j++)
        {
            if (strcmp(theatre->seats[i][j], "[ ]") == 0)
                printf("\033[1;32m");
            else
                printf("\033[1;31m");
            printf("%s ", theatre->seats[i][j]);
        }
        printf("\033[0m\n");
    }

    printf("\t\tPrime $300\n");
    for (int i = 8; i > 5; i--)
    {
        printf("\033[38;5;250m\t\t%c ", 'A' + i);
        printf("\033[0m");
        for (int j = 0; j < 15; j++)
        {
            if (strcmp(theatre->seats[i][j], "[ ]") == 0)
                printf("\033[1;32m");
            else
                printf("\033[1;31m");
            printf("%s ", theatre->seats[i][j]);
        }
        printf("\033[0m\n");
    }

    printf("\t\tNormal $200\n");
    for (int i = 5; i >= 0; i--)
    {
        printf("\033[38;5;250m\t\t%c ", 'A' + i);
        printf("\033[0m");
        for (int j = 0; j < 15; j++)
        {
            if (strcmp(theatre->seats[i][j], "[ ]") == 0)
                printf("\033[1;32m");
            else
                printf("\033[1;31m");
            printf("%s ", theatre->seats[i][j]);
        }
        printf("\033[0m\n");
    }

    printf("\t\t   1   2   3   4   5   6   7   8   9   10  11  12  13  14  15\n\n");
}

void BookSeat(Theatre *theatre, char *booked_seat, Details *user)
{
    char row;
    int num;

    printf("   >>> Enter the row you would like: ");
    scanf(" %c", &row);
    printf("   >>> Enter the seat number you would like (1-15): ");
    scanf("%d", &num);

    int row_index = row - 'A';

    if (strcmp(theatre->seats[row_index][num - 1], booked_seat) == 0)
    {
        printf("   Sorry, the seat %c%d is already booked.\n", row, num);
    }
    else
    {
        strcpy(theatre->seats[row_index][num - 1], booked_seat);
        printf("   Seat %c%d successfully booked!\n", row, num);
        user->row = row;
        user->col = num;

        // Update CSV with movie and seat details
        FILE *file = fopen("data.csv", "a");
        if (file)
        {
            fprintf(file, "%s,%s,%s,%s,%c,%d\n", user->name, user->email, user->mobile, user->movie_selected, user->row, user->col);
            fclose(file);
        }
    }
}

void Book()
{
    printf("   >>> Enter your first or last name: ");
    char search[50];
    scanf("%s", search);

    for (int i = 0; i < count; i++)
    {
        if (strstr(dynamic_array[i].name, search) != NULL)
        {
            printf("   Which Movie would you like to book?\n");
            printf("   (1) %s\n", one.movie_name);
            printf("   (2) %s\n", two.movie_name);
            printf("   (3) %s\n", three.movie_name);
            printf("   (4) %s\n", four.movie_name);
            printf("   (5) %s\n", five.movie_name);

            int option;
            scanf("%d", &option);

            Theatre *theatre = NULL;
            char *selected_movie = NULL;

            switch (option)
            {
            case 1:
                theatre = &one;
                selected_movie = one.movie_name;
                break;
            case 2:
                theatre = &two;
                selected_movie = two.movie_name;
                break;
            case 3:
                theatre = &three;
                selected_movie = three.movie_name;
                break;
            case 4:
                theatre = &four;
                selected_movie = four.movie_name;
                break;
            case 5:
                theatre = &five;
                selected_movie = five.movie_name;
                break;
            default:
                printf("   Invalid option.\n");
                return;
            }

            strcpy(dynamic_array[i].movie_selected, selected_movie);
            ReadCSVAndUpdateSeats("data.csv", theatre, selected_movie);
            DisplaySeats(theatre, selected_movie);
            BookSeat(theatre, "[X]", &dynamic_array[i]);
        }
    }
}

void GenerateBill()
{
    printf("   >>> Enter first or last name: ");
    char search[50];
    scanf("%s", search);
    int found = 0;
    printf("\033[0;31m");
    for (int i = 0; i < count; i++)
    {
        if (strstr(dynamic_array[i].name, search) != NULL)
        {
            found = 1;
            printf("\n\n\t\t   *************************************\n");
            printf("\t\t     * Name : %s\n", dynamic_array[i].name);
            printf("\t\t     * Email id : %s\n", dynamic_array[i].email);
            printf("\t\t     * Mobile No : %s\n", dynamic_array[i].mobile);

            time_t currentTime;
            struct tm *localTime;
            currentTime = time(NULL);
            localTime = localtime(&currentTime);
            printf("\t\t     * %s", asctime(localTime)); 

            printf("\t\t     * Movie Selected: %s\n", dynamic_array[i].movie_selected);
            printf("\t\t     * Seat : %c-%d\n", dynamic_array[i].row, dynamic_array[i].col);

            if ('A' <= dynamic_array[i].row && dynamic_array[i].row <= 'F')
                printf("\t\t     * Price = $200\n");
            else if ('G' <= dynamic_array[i].row && dynamic_array[i].row <= 'I')
                printf("\t\t     * Price = $300\n");
            else if (dynamic_array[i].row == 'J')
                printf("\t\t     * Price = $500\n");

            printf("\t\t   *************************************\n\n");
        }
    }
    if (!found)
    {
        printf("\t\t   No matching customer found.\n");
    }
    printf("\033[0m");
}

int main()
{
    one.movie_name = "Dune 2";
    two.movie_name = "Transformers One";
    three.movie_name = "Oppenheimer";
    four.movie_name = "Inception";
    five.movie_name = "Tenet";

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            strcpy(one.seats[i][j], "[ ]");
            strcpy(two.seats[i][j], "[ ]");
            strcpy(three.seats[i][j], "[ ]");
            strcpy(four.seats[i][j], "[ ]");
            strcpy(five.seats[i][j], "[ ]");
        }
    }

    int choice = 0;
    while (choice != 5)
    {
        PrintMenu();
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            InputDetails();
            break;
        case 2:
            ShowDetails();
            break;
        case 3:
            Book();
            break;
        case 4:
            GenerateBill();
            break;
        case 5:
            printf("Bye!\n");
            break;
        default:
            printf("Invalid choice, please select again.\n");
            break;
        }
    }

    free(dynamic_array);
    return 0;
}
