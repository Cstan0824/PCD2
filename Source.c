#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#pragma warning (disable:4996)

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int hour;
    int min;
} Time;

typedef struct {
    char trainID[10];
    char departureStation[50];
    char arrivalStation[50];
    Date departureDate;
    Time departureTime;
    Time arrivalTime;
    int availableSeat;
} TrainSchedule;

void showSchedule();
void addSchedule();
void serachSchedule();
void modifySchedule();
void deleteSchedule();

void showSchedule() {
    TrainSchedule train;

    FILE* fptr;
    fptr = fopen("TrainSchedule.txt", "r");
    if (fptr == NULL) {
        printf("Error: Unable to open file.\n");
        return;
    }

    printf("Train ID      Departure Station                Arrival Station             Departure Time   Arrival Time  Available Seats  Departure Date\n");
    printf("===============================================================================================================================================\n");
    while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d\n", &train.trainID, &train.departureStation, &train.arrivalStation, &train.departureTime.hour, &train.departureTime.min, &train.arrivalTime.hour, &train.arrivalTime.min, &train.availableSeat, &train.departureDate.day, &train.departureDate.month, &train.departureDate.year) == 11) {
        printf(" %-10s %-30s %-28s \t%02d:%02d\t\t%02d:%02d\t\t %-10d %02d/%02d/%04d\n", train.trainID, train.departureStation, train.arrivalStation, train.departureTime.hour, train.departureTime.min, train.arrivalTime.hour, train.arrivalTime.min, train.availableSeat, train.departureDate.day, train.departureDate.month, train.departureDate.year);
    }
    fclose(fptr);
}

void addSchedule() {
    char n[10] = "\0";
    TrainSchedule train;

    FILE* fptr2;
    fptr2 = fopen("TrainSchedule.txt", "a");
    if (fptr2 == NULL) {
        printf("Error: Unable to open file.\n");
        return;
    }


    while (1) {
        printf("Enter the Train ID (Enter 'T9999' to stop adding schedules): ");
        scanf("%s", &train.trainID);
        if (strcmp(&train.trainID, "T9999") == 0) {
            break;
        }
        printf("Enter the Departure Station: ");
        rewind(stdin);
        scanf("%[^\n]", &train.departureStation);
        printf("Enter the Arrival Station: ");
        rewind(stdin);
        scanf("%[^\n]", &train.arrivalStation);
        printf("Enter the Departure Time (HH:MM): ");
        scanf("%02d:%02d", &train.departureTime.hour, &train.departureTime.min);
        rewind(stdin);
        printf("Enter the Arrival Time (HH:MM): ");
        scanf("%02d:%02d", &train.arrivalTime.hour, &train.arrivalTime.min);
        rewind(stdin);
        printf("Enter the Available Seats: ");
        scanf("%d", &train.availableSeat);
        rewind(stdin);
        printf("Enter the Departure Date (DD/MM/YYYY): ");
        scanf("%02d/%02d/%04d", &train.departureDate.day, &train.departureDate.month, &train.departureDate.year);
        rewind(stdin);
        fprintf(fptr2, "%s|%s|%s|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d\n", train.trainID, train.departureStation, train.arrivalStation, train.departureTime.hour, train.departureTime.min, train.arrivalTime.hour, train.arrivalTime.min, train.availableSeat, train.departureDate.day, train.departureDate.month, train.departureDate.year);

    };
    fclose(fptr2);
}


void serachSchedule() {
    char searchArrivalStation[50];

    TrainSchedule train;

    FILE* fptr3;

    fptr3 = fopen("TrainSchedule.txt", "r");

    if (fptr3 == NULL) {
        printf("Error: Unable to open file.\n");
        return;
    }
    
    printf("Please enter the arrival station you want : ");
    rewind(stdin);
    scanf("%[^\n]", &searchArrivalStation);

    printf("The is the result of <%s>\n", searchArrivalStation);
    printf("\n");
    printf("Train ID      Departure Station                Arrival Station             Departure Time   Arrival Time  Available Seats  Departure Date\n");
    printf("===============================================================================================================================================\n");

    while (fscanf(fptr3, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d\n", train.trainID, train.departureStation, train.arrivalStation, &train.departureTime.hour, &train.departureTime.min, &train.arrivalTime.hour, &train.arrivalTime.min, &train.availableSeat, &train.departureDate.day, &train.departureDate.month, &train.departureDate.year) == 11) {
        if (strcmp(searchArrivalStation, train.arrivalStation) == 0) {
            printf(" %-10s %-30s %-28s \t%02d:%02d\t\t%02d:%02d\t\t %-10d %02d/%02d/%04d\n", train.trainID, train.departureStation, train.arrivalStation, train.departureTime.hour, train.departureTime.min, train.arrivalTime.hour, train.arrivalTime.min, train.availableSeat, train.departureDate.day, train.departureDate.month, train.departureDate.year);
        }
    }
    fclose(fptr3);
}

void deleteSchedule() {
    TrainSchedule train;

    FILE* fptr4, * fptr5;

    fptr4 = fopen("TrainSchedule.txt", "r");
    fptr5 = fopen("TrainSchedule.txt", "w");

    if (fptr4 == NULL || fptr5 == NULL) {
        printf("Error: Unable to open file.\n");
        return;
    }

};
int main() {
    int select;

    printf("Please select the mode : ");
    scanf("%d", &select);

    switch (select) {
    case 1:
        showSchedule();
        break;
    case 2:
        addSchedule();
        break;
    case 3:
        serachSchedule();
        break;
    default:
        printf("Please select again!");
    };
        
    return 0;
}