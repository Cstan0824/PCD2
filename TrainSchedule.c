#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include <Windows.h>
#pragma warning (disable:4996)
#define ISERROR -1

typedef char* string;
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
    int isCancelled;
} TrainSchedule;

int TS_mainMenu();

void TS_addSchedule();
void TS_serachSchedule();
void TS_modifySchedule();
void TS_deleteSchedule();
void TS_displaySchedule();

int G_IntIsValidated(string prompt, int range, int* output); 
int G_ConfirmationIsValidated(string prompt);
int G_GetTxtFileNumRow(string fileName);
void G_ErrorMessage();
Date G_GetCurrentDate();
Time G_GetTime(string prompt);
int G_CalDiffDate(Date date, Date dateToCmp);
int main() {
    system("\033[47m");
    while (TS_mainMenu());
    return 0;
}


int TS_mainMenu() {
    int select;
    int inputIsError = 1;
    do
    {
        if (inputIsError == -1) return 0;
        printf("You are successfully enter into Train Schedule Module\n");
        printf("=====================================================\n");
        printf("1.Show the train schedule\n");
        printf("2.Add the train schedule\n");
        printf("3.Search the train schedule\n");
        printf("4.Delete the train schedule\n");
        printf("5.Modify the train schedule\n");
    } while (inputIsError = G_IntIsValidated("Please select the mode you want", 5, &select));
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
    default:
        printf("Please select again!\n");
        break;
    };
    return 1;
}

void TS_displaySchedule() {
    int count = 0;
    TrainSchedule train = { 0 };

    FILE* fptr;
    fptr = fopen("TrainSchedule.txt", "r");
    if (fptr == NULL) {
        printf("Error: Unable to open file.\n");
        exit(ISERROR);
    }

    printf("Train ID      Departure Station                Arrival Station             Departure Time   Arrival Time  Available Seats  Departure Date\n");
    printf("===============================================================================================================================================\n");
    while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d|%d\n",
        &train.trainID, &train.departureStation, &train.arrivalStation,
        &train.departureTime.hour, &train.departureTime.min,
        &train.arrivalTime.hour, &train.arrivalTime.min,
        &train.availableSeat,
        &train.departureDate.day, &train.departureDate.month, &train.departureDate.year,
        &train.isCancelled) != EOF) {
        if (train.isCancelled == 0) {

            printf("%-10s %-30s %-28s \t%02d:%02d\t\t%02d:%02d\t\t %-10d %02d/%02d/%04d\n",
                train.trainID, train.departureStation, train.arrivalStation,
                train.departureTime.hour, train.departureTime.min,
                train.arrivalTime.hour, train.arrivalTime.min,
                train.availableSeat,
                train.departureDate.day, train.departureDate.month, train.departureDate.year);
        }
        count++;
    }
    printf("Total %d record in train schedule", count);
    fclose(fptr);
    system("pause");
}

void TS_addSchedule() {
    const char* color[] =
    {
        "\033[0;37m", // White[0]  
        "\033[0;32m", // Green[1] 
        "\033[0;33m", // Yellow[2]  
        "\033[0;34m", // Blue[3]
        "\033[0;31m", // Red[4]  
        "\033[1;36m" // Bright Cyan[5]
    };

    int dayOfMonth[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
    Date currDate = G_GetCurrentDate();
    int diffDate = 0;

    TrainSchedule train = { 0 };
    int inputIsError = 0;
    int numRow = G_GetTxtFileNumRow("TrainSchedule.txt");
    FILE* fptr = fopen("TrainSchedule.txt", "a");
    if (!fptr) {
        printf("Error: Unable to open file.\n");
        exit(ISERROR);
    }
    
    TS_displaySchedule();

    //Input the schedule details
    do{
        //Departure Station
        printf("Enter the Departure Station (Enter \'0000\' to exit):");
        rewind(stdin);
        scanf("%[^\n]", &train.departureStation);
        if (strncmp(train.departureStation,"0000",50)) {
            fclose(fptr);
            return;
        }
        //Arrival Station
        printf("Enter the Arrival Station (Enter \'0000\' to exit):");
        rewind(stdin);
        scanf("%[^\n]", &train.arrivalStation);
        if (strncmp(train.arrivalStation, "0000", 50)) {
            fclose(fptr);
            return;
        }
        //Departure Time
        train.departureTime = G_GetTime("Departure Time");
        if (train.departureTime.hour == -1) {
            fclose(fptr);
            return;
        }

        //Arrival Time
        train.arrivalTime = G_GetTime("Departure Time");
        if (train.arrivalTime.hour == -1) {
            fclose(fptr);
            return;
        }

        //Available Seat
        do {
            if (inputIsError == -1) {
                fclose(fptr);
                return;
            }
            if (inputIsError == 1)  G_ErrorMessage();
        } while (inputIsError =
            G_IntIsValidated("Enter the number of Coach(each coach contains 40 seat, maximum 5 coach)", 5, &train.availableSeat));
        
        //Departure Date
        do
        {
            if (diffDate < 0) printf("Departure Date can\'t be before current date.\n");

            //Departure Date (Year)
            while (inputIsError =
                G_IntIsValidated("Enter the Departure Date(DD/MM/\033[0;34mYYYY\033[0;37m)", currDate.year, &train.departureDate.year)) {
                if (inputIsError == -1) {
                    fclose(fptr);
                    return;
                }
                if (inputIsError == 1)  G_ErrorMessage();
            }

            //Departure Date(Month)
            while (inputIsError =
                G_IntIsValidated("Enter the Departure Date(DD/\033[0;34mMM\033[0;37m/YYYY)", 12, &train.departureDate.month)) {
                if (inputIsError == -1) {
                    fclose(fptr);
                    return;
                }
                if (inputIsError == 1)  G_ErrorMessage();
            }

            //Calculate the day of febuary
            dayOfMonth[1] = train.departureDate.year % 4 + 28;

            //Departure Date(Day)
            while (inputIsError =
                G_IntIsValidated("Enter the Departure Date(\033[0;34mDD\033[0;37m/MM/YYYY)", dayOfMonth[train.departureDate.month - 1], &train.departureDate.day)) {
                if (inputIsError == -1) {
                    fclose(fptr);
                    return;
                }
                if (inputIsError == 1)  G_ErrorMessage();
            }

            diffDate = G_CalDiffDate(currDate, train.departureDate);
        } while (diffDate < 0);
        diffDate = 0;
        
        sprintf(train.trainID, "T%04d", numRow + 1); //newTrainID = numOfRow + 1
        train.availableSeat *= 40; // numOfSeat = numOfCoach * 40
        train.isCancelled = 0;
        //Add Schedule
        fprintf(fptr, "%s|%s|%s|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d|%d\n",
            train.trainID, train.departureStation, train.arrivalStation,
            train.departureTime.hour, train.departureTime.min,
            train.arrivalTime.hour, train.arrivalTime.min,
            train.availableSeat,
            train.departureDate.day, train.departureDate.month, train.departureDate.year,
            train.isCancelled);
    } while (G_ConfirmationIsValidated("Do you want to continue add train Schedule?"));
    fclose(fptr);
    system("pause");
}


void TS_serachSchedule() {
    int count = 0;
    char searchArrivalStation[50] = "";
    int isFound = 0;
    TrainSchedule train = { 0 };

    FILE* fptr = fopen("TrainSchedule.txt", "r");

    if (!fptr) {
        printf("Error: Unable to open file.\n");
        exit(ISERROR);
    }


    printf("Welcom to the search mode\n");
    printf("Please enter the arrival station you want (Enter 'quit' to back to main menu) : ");
    scanf("%[^\n]", &searchArrivalStation);
    rewind(stdin);
    if (strncmp(searchArrivalStation, "quit", 50) == 0) {
        fclose(fptr);
        return;
    }

    printf("\n");
    printf("Train ID      Departure Station                Arrival Station             Departure Time   Arrival Time  Available Seats  Departure Date\n");
    printf("===============================================================================================================================================\n");

    while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d|%d\n",
        train.trainID, train.departureStation, train.arrivalStation,
        &train.departureTime.hour, &train.departureTime.min,
        &train.arrivalTime.hour, &train.arrivalTime.min,
        &train.availableSeat,
        &train.departureDate.day, &train.departureDate.month, &train.departureDate.year,
        &train.isCancelled) != EOF) {
        if (strstr(train.arrivalStation, searchArrivalStation) && train.isCancelled == 0) {
            printf(" %-10s %-30s %-28s \t%02d:%02d\t\t%02d:%02d\t\t %-10d %02d/%02d/%04d\n",
                train.trainID, train.departureStation, train.arrivalStation,
                train.departureTime.hour, train.departureTime.min,
                train.arrivalTime.hour, train.arrivalTime.min,
                train.availableSeat,
                train.departureDate.day, train.departureDate.month, train.departureDate.year);
            count++;
            isFound = 1;
        }
    }
    if (isFound) {
        printf("Total %d record in train schedule\n", count);
    }
    else {
        printf("Not result found.");
    }
    fclose(fptr);

    if (G_ConfirmationIsValidated("Do you want to continue to search?")) {
        TS_serachSchedule();
    }
}


void TS_deleteSchedule() {
    TrainSchedule train[100] = { 0 };
    char TrainID[10];
    int trainCount = 0;
    int inputIsError = 1;
    FILE* fptrForRead = fopen("TrainSchedule.txt", "r");
    if (!fptrForRead) {
        printf("File cannot be opened.");
        exit(-1);
    }

    while (fscanf(fptrForRead, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d|%d\n",
        &train[trainCount].trainID, &train[trainCount].departureStation,
        &train[trainCount].arrivalStation, &train[trainCount].departureTime.hour, &train[trainCount].departureTime.min, &train[trainCount].arrivalTime.hour,
        &train[trainCount].arrivalTime.min, &train[trainCount].availableSeat, &train[trainCount].departureDate.day, &train[trainCount].departureDate.month,
        &train[trainCount].departureDate.year,
        &train[trainCount].isCancelled) != EOF) {
        trainCount++;
    }
    fclose(fptrForRead);

    TS_displaySchedule();

    do {
        inputIsError = 1;
        printf("Please enter the train ID you want to delete:");
        scanf("%[^\n]", &TrainID);

        for (int i = 0; i < trainCount; i++) {
            if (strncmp(TrainID, train[i].trainID, 10) == 0 && train[i].isCancelled == 0) 
            {
                inputIsError = 0;
                break;
            }
        }

        if (inputIsError == 1) {
            printf("\nThe Train ID cannot be deleted. Please enter a valid Train ID.\n");
        }
    } while (inputIsError);

    if (G_ConfirmationIsValidated("Are you sure you want to delete this train ID (All the related booking will be cancelled also)?")) {
        FILE* fptrForWrite = fopen("TrainSchedule.txt", "w");
        if (!fptrForWrite) {
            printf("File cannot be opened.");
            exit(ISERROR);
        }
        for (int j = 0; j < trainCount; j++) {
            if (strncmp(train[j].trainID, TrainID, 10) == 0) train[j].isCancelled = 1;
            fprintf(fptrForWrite, "%s|%s|%s|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d|%d\n",
                train[j].trainID, train[j].departureStation, train[j].arrivalStation,
                train[j].departureTime.hour, train[j].departureTime.min,
                train[j].arrivalTime.hour, train[j].arrivalTime.min,
                train[j].availableSeat, train[j].departureDate.day,
                train[j].departureDate.month, train[j].departureDate.year, 
                train[j].isCancelled);
        }
        fclose(fptrForWrite);
        printf("\nTrain with ID %s has been deleted.\n", TrainID);
    }
    else {
        printf("\nDeletion cancelled.\n");
    }
    //cancel the related booking also if the train id is cancelled
}


void TS_modifySchedule() {
    TrainSchedule train[20] = { 0 };
    TrainSchedule tempTrainSchedule = { 0 };
    char trainID[10] = "";
    int found = 0, trainCount = 0, count = 0;
    int inputIsError = 0;

    const char* color[] =
    {
        "\033[0;37m", // White[0]  
        "\033[0;32m", // Green[1] 
        "\033[0;33m", // Yellow[2]  
        "\033[0;34m", // Blue[3]
        "\033[0;31m", // Red[4]  
        "\033[1;36m" // Bright Cyan[5]
    };

    int dayOfMonth[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
    Date currDate = G_GetCurrentDate();
    int diffDate = 0;

    FILE* fptrForRead = fopen("TrainSchedule.txt", "r");
    if (!fptrForRead) {
        printf("File cannot be opened.");
        exit(ISERROR);
    }

    while (fscanf(fptrForRead, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d|%d\n", &train[trainCount].trainID, &train[trainCount].departureStation,
        &train[trainCount].arrivalStation, &train[trainCount].departureTime.hour, &train[trainCount].departureTime.min, &train[trainCount].arrivalTime.hour,
        &train[trainCount].arrivalTime.min, &train[trainCount].availableSeat, &train[trainCount].departureDate.day, &train[trainCount].departureDate.month,
        &train[trainCount].departureDate.year,
        &train[trainCount].isCancelled) != EOF) trainCount++;

    fclose(fptrForRead);

    TS_displaySchedule();

    //Select Train ID to modified
    do {
        found = 0;
        printf("\n\n\nTrain ID to be modified (Enter\'0000\' to exit):");
        scanf("%[^\n]", &trainID);
        rewind(stdin);
        if (strncmp(trainID,"0000",10) == 0) {
            return;
        }

        for (int i = 0; i < trainCount; i++) {
            if (strncmp(trainID, train[i].trainID, 10) == 0 && train[i].isCancelled == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("\nThe Train ID cannot be modified. Please enter a valid Train ID.\n");
        }
    } while (found == 1);

    found = 0;
    for (int i = 0; i < trainCount; i++) {
        if (strncmp(trainID, train[i].trainID, 10) == 0 && train[i].isCancelled == 0) {
            found = 1;
            //Train ID
            strncpy(tempTrainSchedule.trainID, train[i].trainID, 10);

            //Departure Station
            printf("Enter the Departure Station (Enter \'0000\' to exit):");
            rewind(stdin);
            scanf("%[^\n]", &tempTrainSchedule.departureStation);
            if (strncmp(tempTrainSchedule.departureStation, "0000", 50)) {
                return;
            }

            //Arrival Station
            printf("Enter the Arrival Station (Enter \'0000\' to exit):");
            rewind(stdin);
            scanf("%[^\n]", &tempTrainSchedule.arrivalStation);
            if (strncmp(tempTrainSchedule.arrivalStation, "0000", 50)) {
                return;
            }

            //Departure Time
            tempTrainSchedule.departureTime = G_GetTime("Departure Time");
            if (tempTrainSchedule.departureTime.hour == -1) {
                return;
            }

            //Arrival Time
            tempTrainSchedule.arrivalTime = G_GetTime("Departure Time");
            if (tempTrainSchedule.arrivalTime.hour == -1) {
                return;
            }

            //Available Seat
            do {
                if (inputIsError == -1) {
                    return;
                }
                if (inputIsError == 1)  G_ErrorMessage();
            } while (inputIsError =
                G_IntIsValidated("Enter the number of Coach(each coach contains 40 seat, maximum 5 coach)", 5, &tempTrainSchedule.availableSeat));

            //Departure Date
            do
            {
                if (diffDate < 0) printf("Departure Date can\'t be before current date.\n");

                //Departure Date (Year)
                while (inputIsError =
                    G_IntIsValidated("Enter the Departure Date(DD/MM/\033[0;34mYYYY\033[0;37m)", currDate.year, &tempTrainSchedule.departureDate.year)) {
                    if (inputIsError == -1) {
                        return;
                    }
                    if (inputIsError == 1)  G_ErrorMessage();
                }

                //Departure Date(Month)
                while (inputIsError =
                    G_IntIsValidated("Enter the Departure Date(DD/\033[0;34mMM\033[0;37m/YYYY)", 12, &tempTrainSchedule.departureDate.month)) {
                    if (inputIsError == -1) {
                        return;
                    }
                    if (inputIsError == 1)  G_ErrorMessage();
                }

                //Calculate the day of febuary
                dayOfMonth[1] = tempTrainSchedule.departureDate.year % 4 + 28;

                //Departure Date(Day)
                while (inputIsError =
                    G_IntIsValidated("Enter the Departure Date(\033[0;34mDD\033[0;37m/MM/YYYY)",
                        dayOfMonth[tempTrainSchedule.departureDate.month - 1], &tempTrainSchedule.departureDate.day)) {
                    if (inputIsError == -1) {
                        return;
                    }
                    if (inputIsError == 1)  G_ErrorMessage();
                }

                diffDate = G_CalDiffDate(currDate, tempTrainSchedule.departureDate);
            } while (diffDate < 0);
            diffDate = 0;
            
            //isCancelled
            tempTrainSchedule.isCancelled = 0;


            if (G_ConfirmationIsValidated("\nModify Train Schedule ?")) 
            {
                train[i] = tempTrainSchedule;
            }
            else {
                printf("No changes made\n");
            }
        }
    }
    if (found == 0) {
        printf("No train id found\n");
    }

    FILE * fptrForWrite = fopen("TrainSchedule.txt", "w");
    if (!fptrForWrite) 
    {
        printf("File cannot be opened.");
        exit(ISERROR);
    }
    
    for (int j = 0; j < trainCount; j++) {
        fprintf(fptrForWrite, "%s|%s|%s|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d|%d\n",
            train[j].trainID, train[j].departureStation, train[j].arrivalStation,
            train[j].departureTime.hour, train[j].departureTime.min,
            train[j].arrivalTime.hour, train[j].arrivalTime.min,
            train[j].availableSeat, train[j].departureDate.day,
            train[j].departureDate.month, train[j].departureDate.year,
            train[j].isCancelled);
    }
    fclose(fptrForWrite);
}

int G_ConfirmationIsValidated(string prompt) {
    char confirmation[3] = "";
    int inputIsError = 0;
    do
    {
        if (inputIsError) G_ErrorMessage();

        printf("%s [Y/N] >>", prompt);
        scanf("%[^\n]", &confirmation);
        rewind(stdin);

        strcpy(confirmation, strupr(confirmation));
    } while (inputIsError = !(strncmp(confirmation, "Y", 3) == 0 || strncmp(confirmation, "N", 3) == 0));

    return strncmp(confirmation, "Y", 3) == 0;
}

int G_IntIsValidated(string prompt, int range, int* output) {
    char digit[3] = "";
    char decision[3] = "";
    printf("%s  [ENTER \'0\' to escape] >>", prompt);
    scanf("%[^\n]", &decision);
    rewind(stdin);

    if (strncmp(decision, "0", 3) == 0) {
        return -1;
    }
    for (int i = 1; i <= range; i++) {
        sprintf(digit, "%d", i);
        if (strncmp(decision, digit, 3) == 0) {
            *output = i;
            return 0; //return true if same else continue the looping
            //end the do while looping
        }
    }
    G_ErrorMessage();
    return 1; // same return false if input not in range
}

int G_GetTxtFileNumRow(string fileName)
{
    char buffer[250] = ""; 
    int numRow = 0;
    FILE* fptr = fopen(fileName, "r");
    if (!fptr) {
        printf("FIle unable to open.");
        exit(ISERROR);
    }
    while (fscanf(fptr, "%[^\n]\n",
        &buffer) != EOF) {
        numRow++;
    }
    fclose(fptr);
    return numRow;
}
void G_ErrorMessage() {
    printf("\n\nInvalid input. Please type again.\n\n");
    system("pause");
}
Date G_GetCurrentDate() {
    //printf("Audit Log : Date G_GetCurrentDate()\n");
    SYSTEMTIME getDate;
    GetLocalTime(&getDate);
    Date date = { getDate.wDay, getDate.wMonth, getDate.wYear };
    return date;
}
int G_CalDiffDate(Date date, Date dateToCmp) {
    return (date.year <= dateToCmp.year)
        ?
        abs(
            (date.year * 365 + date.month * 30 + date.day)
            -
            (dateToCmp.year * 365 + dateToCmp.month * 30 + dateToCmp.day)
        )
        :
        ISERROR;
}
Time G_GetTime(string prompt) {
    const char* color[] =
    {
        "\033[0;37m", // White[0]  
        "\033[0;32m", // Green[1] 
        "\033[0;33m", // Yellow[2]  
        "\033[0;34m", // Blue[3]
        "\033[0;31m", // Red[4]  
        "\033[1;36m" // Bright Cyan[5]
    };
    Time tempTime = { 0 };
    Time returnError = { -1 };

    char digit[4] = "";
    char decision[4] = "";

    int inputIsError = 1;

    //Time (Hour)
    while (inputIsError) {
        printf("Enter %s (\033[0;34mHH\033[0;37m:MM) [ENTER \'-1\' to escape] >>", prompt);
        scanf("%[^\n]", &decision);
        rewind(stdin);

        if (strncmp(decision, "-1", 3) == 0) return returnError;

        for (int i = 0; i < 24; i++) {
            sprintf(digit, "%d", i);
            if (strncmp(decision, digit, 3) == 0) {
                inputIsError = 0;
                break;
            }
        }
        if (inputIsError) G_ErrorMessage();
    }

    inputIsError = 1;
    //Time (Minute)
    while (inputIsError) {
        printf("Enter %s (HH:\033[0;34mMM\033[0;37m) [ENTER \'-1\' to escape] >>", prompt);
        scanf("%[^\n]", &decision);
        rewind(stdin);

        if (strncmp(decision, "-1", 3) == 0)  return returnError;

        for (int i = 0; i < 60; i++) {
            sprintf(digit, "%d", i);
            if (strncmp(decision, digit, 3) == 0) {
                inputIsError = 0;
                break;
            }
        }
        if (inputIsError) G_ErrorMessage();
    }
    return tempTime;
}