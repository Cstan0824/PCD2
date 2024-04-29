#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdio.h>
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

void TS_displaySchedule();
void TS_addSchedule();
void TS_serachSchedule();
void TS_modifySchedule();
void TS_deleteSchedule();
void TS_showSchedule2();

void TS_showSchedule2() {
    TrainSchedule train;

    FILE* fptr;
    fptr = fopen("TrainSchedule.txt", "r");
    if (fptr == NULL) {
        printf("Error: Unable to open file.\n");
        return;
    }

    printf("Train ID      Departure Station                Arrival Station             Departure Time   Arrival Time  Available Seats  Departure Date\n");
    printf("===============================================================================================================================================\n");
    while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d\n", &train.trainID, &train.departureStation, &train.arrivalStation, &train.departureTime.hour, &train.departureTime.min, &train.arrivalTime.hour, &train.arrivalTime.min, &train.availableSeat, &train.departureDate.day, &train.departureDate.month, &train.departureDate.year) != EOF) {
        printf(" %-10s %-30s %-28s \t%02d:%02d\t\t%02d:%02d\t\t %-10d %02d/%02d/%04d\n", train.trainID, train.departureStation, train.arrivalStation, train.departureTime.hour, train.departureTime.min, train.arrivalTime.hour, train.arrivalTime.min, train.availableSeat, train.departureDate.day, train.departureDate.month, train.departureDate.year);

    }
    fclose(fptr);
}

void TS_displaySchedule() {
    int count = 0;
    char choice;
    TrainSchedule train;

    FILE* fptr;
    fptr = fopen("TrainSchedule.txt", "r");
    if (fptr == NULL) {
        printf("Error: Unable to open file.\n");
        return;
    }

    printf("Train ID      Departure Station                Arrival Station             Departure Time   Arrival Time  Available Seats  Departure Date\n");
    printf("===============================================================================================================================================\n");
    while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d\n", &train.trainID, &train.departureStation, &train.arrivalStation, &train.departureTime.hour, &train.departureTime.min, &train.arrivalTime.hour, &train.arrivalTime.min, &train.availableSeat, &train.departureDate.day, &train.departureDate.month, &train.departureDate.year) != EOF) {
        printf(" %-10s %-30s %-28s \t%02d:%02d\t\t%02d:%02d\t\t %-10d %02d/%02d/%04d\n", train.trainID, train.departureStation, train.arrivalStation, train.departureTime.hour, train.departureTime.min, train.arrivalTime.hour, train.arrivalTime.min, train.availableSeat, train.departureDate.day, train.departureDate.month, train.departureDate.year);
        count++;
    }
    printf("Total %d record in train schedule", count);
    printf("\nPress enter 'quit' to return to the main menu : ");
    choice = getchar();
    while (getchar() != '\n');

    if (choice != 'quit') {
        main();
    }
    fclose(fptr);
}

void TS_addSchedule() {
    char choice;
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
        rewind(stdin);
        scanf("%02d:%02d", &train.departureTime.hour, &train.departureTime.min);
        printf("Enter the Arrival Time (HH:MM): ");
        rewind(stdin);
        scanf("%02d:%02d", &train.arrivalTime.hour, &train.arrivalTime.min);
        printf("Enter the Available Seats: ");
        rewind(stdin);
        scanf("%d", &train.availableSeat);
        printf("Enter the Departure Date (DD/MM/YYYY): ");
        rewind(stdin);
        scanf("%02d/%02d/%04d", &train.departureDate.day, &train.departureDate.month, &train.departureDate.year);
        fprintf(fptr2, "\n%s|%s|%s|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d\n", train.trainID, train.departureStation, train.arrivalStation, train.departureTime.hour, train.departureTime.min, train.arrivalTime.hour, train.arrivalTime.min, train.availableSeat, train.departureDate.day, train.departureDate.month, train.departureDate.year);

    };
    fclose(fptr2);
    printf("\nPress enter 'quit' to return to the main menu : ");
    choice = getchar();
    while (getchar() != '\n');

    if (choice != 'quit') {
        main();
    }
    main();
}


void TS_serachSchedule() {
    char answer;
    int count = 0;
    char searchArrivalStation[50];

    TrainSchedule train;

    FILE* fptr3;

    fptr3 = fopen("TrainSchedule.txt", "r");

    if (fptr3 == NULL) {
        printf("Error: Unable to open file.\n");
        return;
    }

    while (1) {
        printf("Welcom to the search mode\n");
        printf("Please enter the arrival station you want (Enter 'quit' to back to main menu) : ");
        rewind(stdin);
        scanf("%[^\n]", &searchArrivalStation);
        if (strcmp(&searchArrivalStation, "quit") == 0) {
            fclose(fptr3);
            main();
            break;
        }
        else printf("Please type angin\n");
        printf("This is the result of <%s>\n", &searchArrivalStation);
        printf("\n");
        printf("Train ID      Departure Station                Arrival Station             Departure Time   Arrival Time  Available Seats  Departure Date\n");
        printf("===============================================================================================================================================\n");

        while (fscanf(fptr3, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d\n", train.trainID, train.departureStation, train.arrivalStation, &train.departureTime.hour, &train.departureTime.min, &train.arrivalTime.hour, &train.arrivalTime.min, &train.availableSeat, &train.departureDate.day, &train.departureDate.month, &train.departureDate.year) != EOF) {
            if (strcmp(&searchArrivalStation, train.arrivalStation) == 0) {
                printf(" %-10s %-30s %-28s \t%02d:%02d\t\t%02d:%02d\t\t %-10d %02d/%02d/%04d\n", train.trainID, train.departureStation, train.arrivalStation, train.departureTime.hour, train.departureTime.min, train.arrivalTime.hour, train.arrivalTime.min, train.availableSeat, train.departureDate.day, train.departureDate.month, train.departureDate.year);
                count++;
            }
        }
        printf("Total %d record in train schedule\n", count);
        fclose(fptr3);

        printf("Do you want to continue to search\n");
        rewind(stdin);
        scanf("%c", &answer);
        if (answer == 'Y' || answer == 'y') {
            TS_serachSchedule();
        }
        else
            main();
    }
}


void TS_deleteSchedule() {
    TrainSchedule train[100];
    char TrainID[10], decision;
    int trainCount, i = 0, j;

    FILE* fptr = fopen("TrainSchedule.txt", "r");
    if (!fptr) {
        printf("File cannot be opened.");
        exit(-1);
    }

    while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d\n", &train[i].trainID, &train[i].departureStation,
        &train[i].arrivalStation, &train[i].departureTime.hour, &train[i].departureTime.min, &train[i].arrivalTime.hour,
        &train[i].arrivalTime.min, &train[i].availableSeat, &train[i].departureDate.day, &train[i].departureDate.month,
        &train[i].departureDate.year) != EOF) {
        i++;
    }
    trainCount = i;
    fclose(fptr);

    TS_showSchedule2();

    printf("Please enter the train ID you want to delete: ");
    rewind(stdin);
    scanf("%s", &TrainID);
    do {
        printf("Please enter the train ID you want to delete: ");
        rewind(stdin);
        scanf("%s", &TrainID);
    }while(strcmp(&Train.ID, train.trainID));
    printf("\nAre you sure you want to delete train with ID %s? (Y/N): ", TrainID);
    rewind(stdin);
    decision = getchar();


    if (toupper(decision) == 'Y') {
        FILE* fptr2 = fopen("TrainSchedule.txt", "w");
        if (!fptr2) {
            printf("File cannot be opened.");
            exit(-1);
        }
        for (j = 0; j < trainCount; j++) {
            if (strcmp(train[j].trainID, TrainID) != 0) {
                fprintf(fptr2, "%s|%s|%s|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d\n",
                    train[j].trainID, train[j].departureStation, train[j].arrivalStation,
                    train[j].departureTime.hour, train[j].departureTime.min,
                    train[j].arrivalTime.hour, train[j].arrivalTime.min,
                    train[j].availableSeat, train[j].departureDate.day,
                    train[j].departureDate.month, train[j].departureDate.year);
            }
        }
        fclose(fptr2);
        printf("\nTrain with ID %s has been deleted.\n", TrainID);
    }
    else {
        printf("\nDeletion cancelled.\n");
    }
    main();
}



void TS_modifySchedule() {
    TrainSchedule train[20];
    char trainID[6], decision, confirm, tempDepartureStation[50], tempArrivalStation[50], tempTrainID[10];
    int found = 0, i = 0, trainCount = 0, count = 0, departureHour, departureMin, arrivalHour, arrivalMin, availableSeat, departureDay, departureMonth, departureYear, j;

    FILE* fptr = fopen("TrainSchedule.txt", "r");
    if (!fptr) {
        printf("File cannot be opened.");
        exit(-1);
    }

    while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d\n", &train[i].trainID, &train[i].departureStation,
        &train[i].arrivalStation, &train[i].departureTime.hour, &train[i].departureTime.min, &train[i].arrivalTime.hour,
        &train[i].arrivalTime.min, &train[i].availableSeat, &train[i].departureDate.day, &train[i].departureDate.month,
        &train[i].departureDate.year) != EOF) {
        i++;
    }
    trainCount = i;
    fclose(fptr);

    TS_showSchedule2();

    printf("\n\n\nTrain ID to be modified: ");
    scanf("%s", &trainID);
    rewind(stdin);

    for (int o = 0; o < trainCount; o++) {
        if (strcmp(trainID, train[o].trainID) == 0) {
            found = 1;
            printf("Enter the Departure Station: ");
            rewind(stdin);
            scanf("%[^\n]", &tempDepartureStation);
            printf("Enter the Arrival Station: ");
            rewind(stdin);
            scanf("%[^\n]", &tempArrivalStation);
            printf("Enter the Departure Time (HH:MM): ");
            rewind(stdin);
            scanf("%02d:%02d", &departureHour, &departureMin);
            printf("Enter the Arrival Time (HH:MM): ");
            rewind(stdin);
            scanf("%02d:%02d", &arrivalHour, &arrivalMin);
            printf("Enter the Available Seats: ");
            rewind(stdin);
            scanf("%d", &availableSeat);
            printf("Enter the Departure Date (DD/MM/YYYY): ");
            rewind(stdin);
            scanf("%02d/%02d/%04d", &departureDay, &departureMonth, &departureYear);
            printf("\n\nModify Train Schedule ? (Y/N): ");
            rewind(stdin);
            decision = getchar();

            if (toupper(decision) == 'Y') {
                strcpy(train[o].departureStation, tempDepartureStation);
                strcpy(train[o].arrivalStation, tempArrivalStation);
                train[o].departureTime.hour = departureHour;
                train[o].departureTime.min = departureMin;
                train[o].arrivalTime.hour = arrivalHour;
                train[o].arrivalTime.min = arrivalMin;
                train[o].availableSeat = availableSeat;
                train[o].departureDate.day = departureDay;
                train[o].departureDate.month = departureMonth;
                train[o].departureDate.year = departureYear;

            }
            else {
                printf("No changes made\n");
            }
        }
    }
    if (found == 0) {
        printf("No train id found\n");
    }
    fptr = fopen("TrainSchedule.txt", "w");
    if (!fptr) {
        printf("File cannot be opened.");
        exit(-1);
    }
    for (j = 0; j < trainCount; j++) {
        fprintf(fptr, "%s|%s|%s|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d\n",
            train[j].trainID, train[j].departureStation, train[j].arrivalStation,
            train[j].departureTime.hour, train[j].departureTime.min,
            train[j].arrivalTime.hour, train[j].arrivalTime.min,
            train[j].availableSeat, train[j].departureDate.day,
            train[j].departureDate.month, train[j].departureDate.year);

    }
    fclose(fptr);
    main();
}


int main() {
    int select;

    printf("You are successfully enter into Train Schedule Module\n");
    printf("=====================================================\n");
    printf("Please select the mode you want\n");
    printf("1.Show the train schedule\n");
    printf("2.Add the train schedule\n");
    printf("3.Search the train schedule\n");
    printf("4.Delete the train schedule\n");
    printf("5.Modify the train schedule\n");
    printf("6.Exit\n");
    scanf("%d", &select);

    switch (select) {
    case 1:
        TS_displaySchedule();
        break;
    case 2:
        TS_addSchedule();
        break;
    case 3:
        TS_serachSchedule();
        break;
    case 4:
        TS_deleteSchedule();
        break;
    case 5:
        TS_modifySchedule();
        break;
    case 6:
        printf("You are successfully exit");
        exit(-1);
    default:
        printf("Please select again!\n");
        main();
        break;
    };

    return 0;
}
