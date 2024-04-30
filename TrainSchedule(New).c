#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
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
    printf("\nPress enter to return to the main menu");
    getchar();
    while (getchar() != '\n');
    
    fclose(fptr);
    main();
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

    TS_showSchedule2();

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
        do {
            printf("Enter the Departure Time (HH:MM): ");
            if (scanf("%d:%d", &train.departureTime.hour, &train.departureTime.min) != 2 ||
                train.departureTime.hour < 0 || train.departureTime.hour > 23 ||
                train.departureTime.min < 0 || train.departureTime.min > 59) {
                printf("Invalid input! Please enter a valid time.\n");
                while (getchar() != '\n');
            }
            else {
                break;
            }
        } while (1);

        do {
            printf("Enter the Arrival Time (HH:MM): ");
            if (scanf("%d:%d", &train.arrivalTime.hour, &train.arrivalTime.min) != 2 ||
                train.arrivalTime.hour < 0 || train.arrivalTime.hour > 23 ||
                train.arrivalTime.min < 0 || train.arrivalTime.min > 59) {
                printf("Invalid input! Please enter a valid time.\n");
                while (getchar() != '\n'); 
            }
            else {
                break;
            }
        } while (1);

        do {
            printf("Enter the Available Seats (40, 80, or 120): ");
            rewind(stdin);
            if (scanf("%d", &train.availableSeat) != 1 ||
                (train.availableSeat != 40 && train.availableSeat != 80 && train.availableSeat != 120)) {
                printf("Invalid input! Available seats must be 40, 80, or 120.\n");
                while (getchar() != '\n');
            }
            else {
                break;
            }
        } while (1);

        do {
            printf("Enter the Departure Date (DD/MM/YYYY): ");
            if (scanf("%d/%d/%d", &train.departureDate.day, &train.departureDate.month, &train.departureDate.year) != 3 ||
                train.departureDate.day < 1 || train.departureDate.day > 31 ||
                train.departureDate.month < 1 || train.departureDate.month > 12 ||
                train.departureDate.year < 2024) { 
                printf("Invalid input! Please enter a valid date.\n");
                while (getchar() != '\n'); 
            }
            else {
                break;
            }
        } while (1);
        fprintf(fptr2, "\n%s|%s|%s|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d\n", train.trainID, train.departureStation, train.arrivalStation, train.departureTime.hour, train.departureTime.min, train.arrivalTime.hour, train.arrivalTime.min, train.availableSeat, train.departureDate.day, train.departureDate.month, train.departureDate.year);

    };
    fclose(fptr2);

    printf("\nPress enter to return to the main menu");
    getchar();
    while (getchar() != '\n');

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
        if (strcmp(searchArrivalStation, "quit") == 0) {
            fclose(fptr3);
            break;
        }
        else {
            printf("Please type again\n");
        };


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

    do {
        printf("Please enter the train ID you want to delete: ");
        scanf("%s", TrainID);

        for (i = 0; i < trainCount; i++) {
            if (strcmp(TrainID, train[i].trainID) == 0) {
                break;
            }
        }

        if (i == trainCount) {
            printf("\nThe Train ID cannot be deleted. Please enter a valid Train ID.\n");
        }
    } while (i == trainCount);

    if (strcmp(TrainID, "T0000") == 0) {
        printf("\nTrain ID 'T0000' cannot be deleted.\n");
        return;
    }

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
    int found = 0, i = 0, trainCount, count = 0, departureHour, departureMin, arrivalHour, arrivalMin, availableSeat, departureDay, departureMonth, departureYear, j;

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

    do {
        printf("\n\n\nTrain ID to be modified: ");
        scanf("%s", trainID);
        rewind(stdin);

        for (i = 0; i < trainCount; i++) {
            if (strcmp(trainID, train[i].trainID) == 0) {
                found = 1;
                break;
            }
        }

        if (found == 0) {
            printf("\nThe Train ID cannot be modified. Please enter a valid Train ID.\n");
        }
    } while (found == 0);

    if (strcmp(trainID, "T0000") == 0) {
        printf("\nTrain ID 'T0000' cannot be modified.\n");
        return;
    }

    for (int o = 0; o < trainCount; o++) {
        if (strcmp(trainID, train[o].trainID) == 0) {
            found = 1;
            printf("New Train ID : ");
            rewind(stdin);
            scanf("%[^\n]", &tempTrainID);
            printf("Enter the new Departure Station: ");
            rewind(stdin);
            scanf("%[^\n]", &tempDepartureStation);
            printf("Enter the new Arrival Station: ");
            rewind(stdin);
            scanf("%[^\n]", &tempArrivalStation);
            do {
                printf("Enter the new Departure Time (HH:MM): ");
                if (scanf("%d:%d", &departureHour, &departureMin) != 2 ||
                    departureHour < 0 || departureHour > 23 ||
                    departureMin < 0 || departureMin > 59) {
                    printf("Invalid input! Please enter a valid time.\n");
                    while (getchar() != '\n');
                }
                else {
                    break;
                }
            } while (1);

            do {
                printf("Enter the Arrival Time (HH:MM): ");
                if (scanf("%d:%d", &arrivalHour, &arrivalMin) != 2 ||
                    arrivalHour < 0 || arrivalHour > 23 ||
                    arrivalMin < 0 || arrivalMin > 59) {
                    printf("Invalid input! Please enter a valid time.\n");
                    while (getchar() != '\n');
                }
                else {
                    break;
                }
            } while (1);

            do {
                printf("Enter the Available Seats (40, 80, or 120): ");
                rewind(stdin);
                scanf("%d", &availableSeat);
                if (availableSeat != 40 && availableSeat != 80 && availableSeat != 120) {
                    printf("Invalid number of seats. Please enter 40, 80, or 120.\n");
                }
            } while (availableSeat != 40 && availableSeat != 80 && availableSeat != 120);


            do {
                printf("Enter the Departure Date (DD/MM/YYYY): ");
                if (scanf("%d/%d/%d", &departureDay, &departureMonth, &departureYear) != 3 ||
                    departureDay < 1 || departureDay > 31 ||
                    departureMonth < 1 || departureMonth > 12 ||
                    departureYear < 2024) {
                    printf("Invalid input! Please enter a valid date.\n");
                    while (getchar() != '\n');
                }
                else {
                    break;
                }
            } while (1);
            printf("\n\nModify Train Schedule ? (Y/N): ");
            rewind(stdin);
            decision = getchar();

            if (toupper(decision) == 'Y') {
                strcpy(train[o].trainID, tempTrainID);
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
