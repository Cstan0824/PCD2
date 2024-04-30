#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#pragma warning (disable:4996)

#define MAX_LINE_LENGTH 256

//Global Declarations
typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char memberID[11];
    char memberPass[11];
    char memberName[50];
    char memberPhoneNo[12];
    Date memberJoinDate;
    int memberRewards;
    double memberWallet;
} MI_Member;

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

// Function Declarations
Date MI_getDate();
void MI_shiftSpace(int MI_position);
void MI_drawTrain();
int MI_getNumberOfMembers();
MI_Member* MI_getMemberDetails(int rows);
char* getPassword();
int MI_verifyRegister(char memberID[11]);
void MI_registerMember();
void MI_mainMenu();
void MI_memberMenu(char* memberID);
void MI_memberLogin();
int MI_verifyLogin(char inputID[11], char inputPass[11]);
void MI_displayDetails(char* memberID);
void MI_displayDetailsProcess(MI_Member* allMembers, int rows, char* memberID);
void MI_topUpWallet(char* memberID);
void MI_memberEditDetails(char* memberID);
void MI_deleteAccount(char* memberID);
void TS_showSchedule();
int G_ContinueOrStopCRUDRecord(char* CRUD);
void G_ErrorMessage();
int G_MenuValidation(char decision[3], int range);
/*for admin use*/ void MI_displayAllMembers();
/*for admin use*/ void MI_staffSearchMember();
/*for admin use*/ void MI_displaySearchedMembers(char* memberID);
/*for admin use*/ void MI_staffDeleteMember();
/*for admin use*/ void MI_staffEditMemberDetails();
/*for admin use*/ void MI_staffAddMemberViaTxt();
/*for admin use*/ void MI_staffAddMember();
/*for admin use*/ int MI_getNumberOfNewMembersViaTxt(char* fileName);
/*for admin use*/ MI_Member* MI_getNewMemberDetailsViaTxt(char* fileName, int rowsOfNewMembers);
/*for admin use*/ void MI_displayMembersHeaderOrFooter(char HeaderOrFooter);


// Main function
int main() {
    
    MI_mainMenu();
   return 0;
}

//Global Functions

int G_ContinueOrStopCRUDRecord(char* CRUD) {
    //printf("AuditLog : int G_ContinueOrStopCRUDRecord(char * CRUD)\n");
    char continueCRUDRecord;
    do
    {
        printf("%s", CRUD);
        scanf("%c", &continueCRUDRecord);
        rewind(stdin);
        continueCRUDRecord = toupper(continueCRUDRecord);
    } while (!(continueCRUDRecord == 'Y' || continueCRUDRecord == 'N'));
    return continueCRUDRecord == 'Y';
}
void G_ErrorMessage() {

    printf("\n\nInvalid input. Please type again.\n\n");
    system("pause");
};
int G_MenuValidation(char decision[3], int range) {
    char digit[3] = ""; // Changed to 2 to hold one digit and the null terminator
    for (int i = 1; i <= range; i++) {
        sprintf(digit, "%d", i);
        if (strcmp(decision, digit) != 0) {

            return 1;

        }
        else {
            return 0;
        }
            
    }

};


// Functions
void MI_mainMenu() {
    int validation = 0;
    int MI_menuDecision;
    do {
        system("cls");

        char tempDecision[3];
        printf("Welcome to Train Ticketing System\n\n");
        printf("Choose your mode: \n");
        printf("1. Member Login\n");
        printf("2. Member Register\n");
        printf("3. Exit Program\n\n\n");

        printf("Mode: ");
        scanf("%s", &tempDecision);
        rewind(stdin);
        
        if (G_MenuValidation(tempDecision, 3) != 0) {
            G_ErrorMessage();
            validation = 1;
        }
        else {
            MI_menuDecision = atoi(tempDecision);
            validation = 0;
        }

    } while (validation != 0);


    switch (MI_menuDecision) {
    case 1:
        MI_memberLogin();
        break;
    case 2:
        MI_registerMember();
        break;
    case 3:
        exit(-1);
    default:
        printf("Invalid input. Redirecting...\n\n");
        system("pause");
        system("cls");
        MI_mainMenu();
    }
}

void MI_memberMenu(char* memberID) {
    char tempDecision[3];
    int MI_memberMenuDecision;
    int validation = 0;
    do {
        system("cls");
        printf("Welcome, %s\n\n\n", memberID);
        MI_displayDetails(memberID);
        printf("Choose your mode: \n");
        printf("1. Top Up Wallet\n");
        printf("2. Edit Details\n");
        printf("3. Check Ticket Schedule\n");
        printf("4. Book Tickets\n");
        printf("5. Booking Details\n");
        printf("6. Delete Account\n");
        printf("7. Exit Program\n\n\n");

        printf("Mode: ");
        scanf("%s", &tempDecision);
        rewind(stdin);

        if (G_MenuValidation(tempDecision, 7) != 0) {
            G_ErrorMessage();
            validation = 1;
        }
        else {
            MI_memberMenuDecision = atoi(tempDecision);
            validation = 0;
        }
    } while (validation != 0);

    switch (MI_memberMenuDecision) {
    case 1:
        MI_topUpWallet(memberID);
        break;
    case 2:
        MI_memberEditDetails(memberID);
        break;
    case 3:
        TS_showSchedule();
        break;
    case 4:
       /* TB_BookingTicket();*/
        break; 
    case 5:
        /*TB_GetBookingDetails();*/
        break;
    case 6:
        MI_deleteAccount(memberID);
        break;
    case 7:
        exit(-1);
        break;
    default:
        printf("Invalid input. Redirecting...\n\n");
        system("pause");
        system("cls");
        MI_memberMenu(memberID);
    }
};

int MI_getNumberOfMembers() {
    int rows;
    FILE* fptr = fopen("member-details.bin", "rb");
    if (fptr == NULL) {
        printf("\nError opening file.");
        exit(-1);
    }

    // Get the number of members in the file
    fseek(fptr, 0, SEEK_END);
    long fileSize = ftell(fptr);
    rewind(fptr);
    rows = fileSize / sizeof(MI_Member);
    fclose(fptr);
    return rows;
};

MI_Member* MI_getMemberDetails(int rows) {
    FILE* fptr = fopen("member-details.bin", "rb");
    if (fptr == NULL) {
        printf("\nError opening file.");
        return NULL;
    }

    // Allocate memory for member details
    MI_Member* allMembers = (MI_Member*)malloc((rows) * sizeof(MI_Member));
    if (allMembers == NULL) {
        printf("\nMemory allocation failed.");
        fclose(fptr);
        return NULL;
    }

    // Read member details from the binary file
    fread(allMembers, sizeof(MI_Member), rows, fptr);
    return allMembers;
}

void MI_registerMember() {
    MI_Member newMember;
    int verify;

    system("cls");
    do {
        system("cls");
        printf("Please enter a new Member ID (7-10 characters): ");
        scanf("%s", &newMember.memberID);
        rewind(stdin);
        verify = MI_verifyRegister(newMember.memberID);

        if (verify == 1) {
            printf("\nInvalid member ID. Member ID already exists.\n");
            return;
        }
    } while (verify == 1);

    verify = 0;

    do {
        system("cls");
        printf("\nPlease enter a password (7-10 characters): ");

        char* tempPass = getPassword();

        printf("\nPlease re-enter to confirm password: ");
        char* confirmPass = getPassword();

        verify = strcmp(tempPass, confirmPass);
        if (verify != 0) {
            printf("\n\nIncorrect password. Please enter again\n\n");
        }
        else {
            strcpy(newMember.memberPass, confirmPass);
        }
    } while (verify != 0);

    printf("\n\nPlease enter your name: ");
    scanf("%[^\n]", &newMember.memberName);
    rewind(stdin);
    printf("\n\nPlease enter your phone number: ");
    scanf("%s", &newMember.memberPhoneNo);
    rewind(stdin);
    printf("\n\n");
    newMember.memberJoinDate = MI_getDate();
    newMember.memberRewards = 0;
    newMember.memberWallet = 0;

    // Write member details to binary file
    FILE* fptr = fopen("member-details.bin", "ab");
    if (fptr == NULL) {
        printf("\nError opening file.");
        system("pause");
        exit(-1);
    }
    fwrite(&newMember, sizeof(MI_Member), 1, fptr);
    fclose(fptr);

    printf("Member %s has been registered successfully!\n\n", newMember.memberName);
    system("pause");
    system("cls");
    MI_mainMenu();
}

int MI_verifyRegister(char memberID[11]) {
    int rows = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(rows);
    if (allMembers == NULL) {
        printf("\nFailed to read member details.\n");
        return -1; // Error
    }

    for (int i = 0; i < rows; i++) {
        if (strcmp(memberID, allMembers[i].memberID) == 0) {
            free(allMembers);
            return 1; // ID exists
        }
    }

    free(allMembers);
    return 0; // ID doesn't exist
}

void MI_memberLogin() {
    char inputID[11];
    int verify;

    printf("Please enter your Member ID (7-10 characters): ");
    scanf("%s", &inputID);
    rewind(stdin);
    printf("\n\nPlease enter your password (7-10 characters): ");
    char* inputPass = getPassword();

    verify = MI_verifyLogin(inputID, inputPass);
    if (verify == 0) {
        MI_memberMenu(inputID);
    }
    else {
        printf("Wrong ID or password.");
        exit(-1);
    }

}

int MI_verifyLogin(char inputID[11], char inputPass[11]) {
    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);
    if (allMembers == NULL) {
        printf("\nFailed to read member details.\n");
        exit(-1);
    }

    for (int i = 0; i < numOfMembers; i++) {
        if (strcmp(allMembers[i].memberID, inputID) == 0) {
            if (strcmp(allMembers[i].memberPass, inputPass) == 0) {
                printf("\n\n\nLogin successful, %s\n", allMembers[i].memberName);
                free(allMembers);
                system("pause");
                return 0;
            }
            else {
                printf("\n\n\nLogin failed, wrong ID or password.\n");
                free(allMembers);
                return 1;
            }
        }
    }
    free(allMembers);
    return 1;
}

void MI_topUpWallet(char* memberID) {
    double tempWallet;
    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);
    MI_Member newDetails = { 0 };
    for (int i = 0; i < numOfMembers; i++) {

        if (strcmp(allMembers[i].memberID, memberID) == 0) {

            printf("\n\nCurrent Wallet: %.2lf\n\n", allMembers[i].memberWallet);
            printf("Top up amount: ");
            scanf("%lf", &tempWallet);
            rewind(stdin);
            tempWallet = tempWallet + allMembers[i].memberWallet;
            printf("\n\nWallet amount after top up: %.2lf\n\n", tempWallet);
            system("pause");
            strcpy(newDetails.memberID, memberID);
            strcpy(newDetails.memberPass, allMembers[i].memberPass);
            strcpy(newDetails.memberName, allMembers[i].memberName);
            strcpy(newDetails.memberPhoneNo, allMembers[i].memberPhoneNo);
            newDetails.memberJoinDate.day = allMembers[i].memberJoinDate.day;
            newDetails.memberJoinDate.month = allMembers[i].memberJoinDate.month;
            newDetails.memberJoinDate.year = allMembers[i].memberJoinDate.year;
            newDetails.memberRewards = allMembers[i].memberRewards;
            newDetails.memberWallet = tempWallet;
        }

    }
    // Update the binary file
    FILE* fptrBin = fopen("member-details.bin", "wb");
    if (!fptrBin) {
        printf("File cannot be opened.");
        exit(-1);
    }
    for (int i = 0; i < numOfMembers; i++)
    {
        if (strcmp(allMembers[i].memberID, memberID) != 0) {
            fwrite(&allMembers[i], sizeof(MI_Member), 1, fptrBin);
        }
    }
    fwrite(&newDetails, sizeof(MI_Member), 1, fptrBin);
    printf("Wallet updated.\n\n");
    fclose(fptrBin);
    free(allMembers);
    MI_memberMenu(memberID);
}

void MI_memberEditDetails(char* memberID) {
    int numOfMembers = MI_getNumberOfMembers();
    if (numOfMembers == -1) {
        printf("Failed to get number of members.\n");
        exit(-1);
    }

    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);
    if (allMembers == NULL) {
        printf("Failed to get member details.\n");
        exit(-1);
    }

    printf("Current details: \n\n");
    MI_displayDetails(memberID);

    char decision;
    MI_Member newDetails;
    int verify;
    for (int i = 0; i < numOfMembers; i++) {
        if (strcmp(memberID, allMembers[i].memberID) == 0) {
            strcpy(newDetails.memberID, memberID);
            printf("\n\n Do you want to edit password? (Y/N): ");
            scanf("%c", &decision);
            rewind(stdin);
            if (toupper(decision) == 'Y') {
                do {
                    system("cls");
                    printf("\nPlease enter a new password (7-10 characters): ");

                    char* tempPass = getPassword();

                    printf("\nPlease re-enter to confirm new password: ");
                    char* confirmPass = getPassword();

                    verify = strcmp(tempPass, confirmPass);
                    if (verify != 0) {
                        printf("\n\nIncorrect password. Please enter again\n\n");
                    }
                    else {
                        strcpy(newDetails.memberPass, confirmPass);
                    }
                } while (verify != 0);
            }
            else {
                strcpy(newDetails.memberPass, allMembers[i].memberPass);
            }
            printf("\n\n Do you want to edit name? (Y/N): ");
            scanf("%c", &decision);
            rewind(stdin);
            if (toupper(decision) == 'Y') {
                printf("New name: ");
                scanf("%s", &newDetails.memberName);
                rewind(stdin);
            }
            else {
                strcpy(newDetails.memberName, allMembers[i].memberName);
            }
            printf("\n\n Do you want to edit phone number? (Y/N): ");
            scanf("%c", &decision);
            rewind(stdin);
            if (toupper(decision) == 'Y') {
                printf("New phone number (XXXXXXXXXX or XXXXXXXXXXX): ");
                scanf("%s", &newDetails.memberPhoneNo);
                rewind(stdin);
            }
            else {
                strcpy(newDetails.memberPhoneNo, allMembers[i].memberPhoneNo);
            }
            newDetails.memberJoinDate.day = allMembers[i].memberJoinDate.day;
            newDetails.memberJoinDate.month = allMembers[i].memberJoinDate.month;
            newDetails.memberJoinDate.year = allMembers[i].memberJoinDate.year;
            newDetails.memberRewards = allMembers[i].memberRewards;
            newDetails.memberWallet = allMembers[i].memberWallet;

            // Update the binary file
            FILE* fptrBin = fopen("member-details.bin", "wb");
            if (!fptrBin) {
                printf("File cannot be opened.");
                exit(-1);
            }
            for (int u = 0; u < numOfMembers; u++)
            {
                if (strcmp(allMembers[u].memberID, memberID) != 0) {
                    fwrite(&allMembers[u], sizeof(MI_Member), 1, fptrBin);
                }
            }
            fwrite(&newDetails, sizeof(MI_Member), 1, fptrBin);
            printf("\n\nDetails updated.\n\n");
            fclose(fptrBin);
        }
    }
    free(allMembers);
};

void MI_deleteAccount(char* memberID) {
    char MI_deleteDecision;
    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);

    printf("Are you sure you want to delete account? (Y/N): ");
    MI_deleteDecision = getchar();
    rewind(stdin);

    if (toupper(MI_deleteDecision) == 'Y') {
        // Update the binary file
        FILE* fptrBin = fopen("member-details.bin", "wb");
        if (!fptrBin) {
            printf("File cannot be opened.");
            exit(-1);
        }
        for (int i = 0; i < numOfMembers; i++)
        {

            if (strcmp(allMembers[i].memberID, memberID) != 0) {
                fwrite(&allMembers[i], sizeof(MI_Member), 1, fptrBin);
            }
            else {
                printf("\n\nAccount Deleted.\n");
                
            }

        }
        fclose(fptrBin);
    }
    free(allMembers);
    system("pause");
    system("cls");
    MI_mainMenu();
}

void MI_displayDetails(char* memberID) {
    int numOfMembers = MI_getNumberOfMembers();
    if (numOfMembers == -1) {
        printf("Failed to get number of members.\n");
        return;
    }

    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);
    if (allMembers == NULL) {
        printf("Failed to get member details.\n");
        return;
    }

    MI_displayDetailsProcess(allMembers, numOfMembers, memberID);

    // Free dynamically allocated memory
    free(allMembers);
}

void MI_displayDetailsProcess(MI_Member* allMembers, int rows, char* memberID) {
    

    for (int i = 0; i < rows; i++) {
        if (strcmp(allMembers[i].memberID, memberID) == 0) {
            printf("Member ID: %s\n", allMembers[i].memberID);
            printf("Password: ");
            for (int p = 0; p < strlen(allMembers[i].memberPass); p++) {

                printf("*");

            }
            printf("\nMember Name: %s\n", allMembers[i].memberName);
            printf("Phone Number: %s\n", allMembers[i].memberPhoneNo);
            printf("Joined Date: %02d-%02d-%-04d\n", allMembers[i].memberJoinDate.day, 
                allMembers[i].memberJoinDate.month, allMembers[i].memberJoinDate.year);
            printf("Member Points: %d\n", allMembers[i].memberRewards);
            printf("Member Wallet: %.2lf\n\n\n", allMembers[i].memberWallet);
            
        }
    }
}

void TS_showSchedule() {
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

void MI_drawTrain() {
    int MI_position = 0;
    int MI_maxPosition = 35;
    for (int i = 0; i < MI_maxPosition; i++) {
        system("cls");
        MI_shiftSpace(MI_position);
        printf("     _________________________________   _____________\n");
        MI_shiftSpace(MI_position);
        printf("    / _|_   _|_ \\ / _|_   _|_ \\ / _|_ \\ / / \\ / \\  __()\n");
        MI_shiftSpace(MI_position);
        printf("   | |   | |   | | |   | |   | | |   | |  \\_/ \\_/ |  \n");
        MI_shiftSpace(MI_position);
        printf("   | |   | |   | | |   | |   | | |   | | | _ _ _ ||\n");
        MI_shiftSpace(MI_position);
        printf("   | |_ _| |_ _| | |_ _| |_ _| | |_ _| | |/_\\_/_\\||___\n");
        MI_shiftSpace(MI_position);
        printf("    \\__|_____|__/_\\__|_____|__/_\\__|__/_\\____________/\n");
        MI_shiftSpace(MI_position);
        printf(" -=@=@==@=@==@==@==@==@==@==@==@==@==@==@==@==@==@==@=-\n");
        MI_shiftSpace(MI_position);
        printf(" -=@=@==@=@==@==@==@==@==@==@==@==@==@==@==@==@==@==@=-\n");
        MI_position = (MI_position + 1) % MI_maxPosition;
        Sleep(20);
    }
    printf("\n\n\n");
}

Date MI_getDate() {
    Date MI_currentDate;

    SYSTEMTIME t;
    GetLocalTime(&t);

    MI_currentDate.day = t.wDay;
    MI_currentDate.month = t.wMonth;
    MI_currentDate.year = t.wYear;

    return MI_currentDate;
}

void MI_shiftSpace(int MI_position) {
    for (int i = 0; i < MI_position; i++) {
        printf(" ");
    }
}

char* getPassword() {
    int i = 0;
    char tempPass[11];
    do {
        char c = _getch();
        if (c == '\r') { // Enter key
            break;
        }
        if (c == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b"); // Move cursor back, print space, move cursor back again
            }
        }
        else {
            if (i < 10) { // Limiting the password length to 10 characters
                tempPass[i++] = c;
                printf("*");
            }
        }
    } while (1);
    tempPass[i] = '\0'; // Null-terminate the password string
    rewind(stdin);
    return tempPass;
}

/*for admin use*/ void MI_displayAllMembers() {
    
    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);
    MI_displayMembersHeaderOrFooter('H');
    for (int i = 0; i < numOfMembers; i++) {
        // Create a new string with asterisks for the password
        int passLength = strlen(allMembers[i].memberPass);
        char* asteriskPassword = (char*)malloc((passLength + 1) * sizeof(char)); // +1 for null terminator
        if (asteriskPassword == NULL) {
            // Handle memory allocation failure
            printf("Memory allocation failed.\n");
            break;
        }
        for (int j = 0; j < passLength; j++) {
            asteriskPassword[j] = '*';
        }
        asteriskPassword[passLength] = '\0'; // Null-terminate the string

        printf("| %-13s | %-13s | %-50s | %-13s | %02d-%02d-%-05d | %-13d | %-12.2lf|\n", allMembers[i].memberID, asteriskPassword,
            allMembers[i].memberName, allMembers[i].memberPhoneNo,
            allMembers[i].memberJoinDate.day, allMembers[i].memberJoinDate.month, allMembers[i].memberJoinDate.year, allMembers[i].memberRewards,
            allMembers[i].memberWallet);
        MI_displayMembersHeaderOrFooter('F');

        // Free dynamically allocated memory for the password
        free(asteriskPassword);
    }
    free(allMembers);

};
/*for admin use*/ void MI_displayMembersHeaderOrFooter(char HeaderOrFooter) {

    if (HeaderOrFooter == 'H') {
        for (int i = 0; i < 146; i++) {
            printf("=");
        }
        printf("\n");
        printf("%-15s %-15s %-52s %-15s %-13s %-15s %-22s\n", "| Member ID", "| Password", "| Name", "| Phone Number", "| Join Date", "| Reward Points", "| Wallet      |");
        for (int i = 0; i < 146; i++) {
            printf("=");
        }
        printf("\n");
    }
    else if (HeaderOrFooter == 'F') {

        for (int i = 0; i < 146; i++) {
            printf("-");
        }
        printf("\n");

    }

};
/*for admin use*/ void MI_displaySearchedMembers(char* memberID){
    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);

    for (int i = 0; i < numOfMembers; i++) {
        if (strcmp(memberID, allMembers[i].memberID) == 0) {
            // Create a new string with asterisks for the password
            int passLength = strlen(allMembers[i].memberPass);
            char* asteriskPassword = (char*)malloc((passLength + 1) * sizeof(char)); // +1 for null terminator
            if (asteriskPassword == NULL) {
                // Handle memory allocation failure
                printf("Memory allocation failed.\n");
                break;
            }
            for (int j = 0; j < passLength; j++) {
                asteriskPassword[j] = '*';
            }
            asteriskPassword[passLength] = '\0'; // Null-terminate the string

            printf("| %-13s | %-13s | %-50s | %-13s | %02d-%02d-%-05d | %-13d | %-12.2lf|\n", allMembers[i].memberID, asteriskPassword,
                allMembers[i].memberName, allMembers[i].memberPhoneNo,
                allMembers[i].memberJoinDate.day, allMembers[i].memberJoinDate.month, allMembers[i].memberJoinDate.year, allMembers[i].memberRewards,
                allMembers[i].memberWallet);
            MI_displayMembersHeaderOrFooter('F');

            // Free dynamically allocated memory for the password
            free(asteriskPassword);
        }
    }
    free(allMembers);


};
/*for admin use*/ void MI_staffSearchMember() {
    int MI_searchYear, MI_searchMonth, MI_searchDay;
    char MI_searchID[11], MI_searchName[50];
    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);
    do {
        system("cls");
        int MI_menuDecision;
        printf("Search member through:\n\n");
        printf("1. Joined year\n");
        printf("2. Joined month\n");
        printf("3. Joined day\n");
        printf("4. Member ID\n");
        printf("5. Member Name\n");
        printf("6. Back to menu\n\n\n");

        printf("Mode: ");
        scanf("%d", &MI_menuDecision);
        rewind(stdin);

        switch (MI_menuDecision) {

        case 1:
            //based on year
            printf("Search member joined on year: ");
            scanf("%d", &MI_searchYear);
            rewind(stdin);

            MI_displayMembersHeaderOrFooter('H');
            for (int i = 0; i < numOfMembers; i++) {
                // Extract the year part from the member's join date and compare with the desired year
                int year = allMembers[i].memberJoinDate.year;

                if (year == MI_searchYear) {
                    // Print the member's details if they joined in the desired year
                    MI_displaySearchedMembers(allMembers[i].memberID);
                }
            }
            break;
        case 2:
            //based on month
            printf("Search member joined on month: ");
            scanf("%d", &MI_searchMonth);
            rewind(stdin);
            printf("\n\n");
            for (int i = 0; i < numOfMembers; i++) {
                // Extract the month part from the member's join date and compare with the searched month
                int month = allMembers[i].memberJoinDate.month;

                if (month == MI_searchMonth) {
                    // Print the member's details if they joined in the searched month
                    MI_displaySearchedMembers(allMembers[i].memberID);
                }
            }
            break;
        case 3:
            //based on day
            printf("Search member joined on day: ");
            scanf("%d", &MI_searchDay);
            rewind(stdin);
            printf("\n\n");
            for (int i = 0; i < numOfMembers; i++) {
                // Extract the day part from the member's join date and compare with the searched day
                int day = allMembers[i].memberJoinDate.day;

                if (day == MI_searchDay) {
                    // Print the member's details if they joined in the searched day
                    MI_displaySearchedMembers(allMembers[i].memberID);
                }
            }
            break;
        case 4:
            //based on memberID
            printf("Search member by member ID: ");
            scanf("%s", &MI_searchID);
            rewind(stdin);
            printf("\n\n");
            for (int i = 0; i < numOfMembers; i++) {

                if (strcmp(MI_searchID, allMembers[i].memberID) == 0) {
                    MI_displaySearchedMembers(allMembers[i].memberID);
                }
            }
            break;
        case 5:
            //based on member name
            printf("Search member by member name: ");
            scanf("%[^\n]", &MI_searchName);
            rewind(stdin);
            printf("\n\n");
            for (int i = 0; i < numOfMembers; i++) {

                if (strstr(allMembers[i].memberName, MI_searchName) != NULL) {
                    MI_displaySearchedMembers(allMembers[i].memberID);
                }
            }
        case 6:
            free(allMembers);
            exit(-1);
            break;
        default:
            printf("\nInvalid decision. Redirecting...\n");
            free(allMembers);
            MI_staffSearchMember();
        }
    } while (G_ContinueOrStopCRUDRecord("Do you want to continue search for members? (Y/N)") == 1);
    free(allMembers);
};
/*for admin use*/ void MI_staffAddMember() {

    MI_Member newMember;
    int verify;
    char tempPass[11], confirmPass[11];

    do {
        do {
            system("cls");
            printf("Please enter a new Member ID (7-10 characters): ");
            scanf("%s", &newMember.memberID);
            rewind(stdin);
            verify = MI_verifyRegister(newMember.memberID);

            if (verify == 1) {
                printf("\nInvalid member ID. Member ID already exists.\n");
                return;
            }
        } while (verify == 1);

        verify = 0;

        do {
            system("cls");
            printf("\nPlease enter a password (7-10 characters): ");

            char* tempPass = getPassword();

            printf("\nPlease re-enter to confirm password: ");
            char* confirmPass = getPassword();

            verify = strcmp(tempPass, confirmPass);
            if (verify != 0) {
                printf("\n\nIncorrect password. Please enter again\n\n");
            }
            else {
                strcpy(newMember.memberPass, confirmPass);
            }
        } while (verify != 0);



        printf("\n\nPlease enter new member's name: ");
        scanf("%[^\n]", &newMember.memberName);
        rewind(stdin);
        printf("\n\nPlease enter new member's phone number: ");
        scanf("%s", &newMember.memberPhoneNo);
        rewind(stdin);
        newMember.memberJoinDate = MI_getDate();
        printf("\n\nPlease enter new member's reward points: ");
        scanf("%d", &newMember.memberRewards);
        rewind(stdin);
        newMember.memberWallet = 0;

        // Write member details to binary file
        FILE* fptr = fopen("member-details.bin", "ab");
        if (fptr == NULL) {
            printf("\nError opening file.");
            system("pause");
            exit(-1);
        }
        fwrite(&newMember, sizeof(MI_Member), 1, fptr);
        fclose(fptr);

        printf("Member %s has been registered successfully!\n\n", newMember.memberName);
        system("pause");
        system("cls");
    } while (G_ContinueOrStopCRUDRecord("Do you want to add more members? (Y/N): ") == 1);
};
/*for admin use*/ void MI_staffDeleteMember() {
    char tempID[11];
    char decision;
    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);
    do {
        MI_displayAllMembers();

        printf("\n\nEnter member ID to cancel membership: ");
        scanf("%s", tempID);
        rewind(stdin);


        for (int i = 0; i < numOfMembers; i++) {
            if (strcmp(allMembers[i].memberID, tempID) == 0) {
                MI_displayDetails(allMembers[i].memberID);
                printf("\n\n Are you sure you want to delete? (Y/N): ");
                scanf("%c", &decision);
                rewind(stdin);
                if (decision == 'Y' || decision == 'y') {
                    // Update the binary file
                    FILE* fptrBin = fopen("member-details.bin", "wb");
                    if (!fptrBin) {
                        printf("File cannot be opened.");
                        exit(-1);
                    }
                    for (int i = 0; i < numOfMembers; i++)
                    {

                        if (strcmp((allMembers + i)->memberID, tempID) != 0) {
                            fwrite(&allMembers[i], sizeof(MI_Member), 1, fptrBin);
                        }
                        else {
                            printf("\n\nMember Deleted\n");
                        }

                    }
                    fclose(fptrBin);
                }

            }
        }
    } while (G_ContinueOrStopCRUDRecord("Do you want to continue delete more members? (Y/N): ") == 1);

    free(allMembers);

}
/*for admin use*/ void MI_staffEditMemberDetails() {
    char tempID[11];
    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);
    do{
        MI_displayAllMembers();
        printf("Enter member ID to edit details: ");
        scanf("%s", &tempID);
        rewind(stdin);

        MI_displayDetails(tempID);
        char decision;
        MI_Member newDetails;
        int verify;
    
        for (int i = 0; i < numOfMembers; i++) {
            if (strcmp(tempID, allMembers[i].memberID) == 0) {
                strcpy(newDetails.memberID, tempID);
                printf("\n\n Do you want to edit password? (Y/N): ");
                scanf("%c", &decision);
                rewind(stdin);
                if (toupper(decision) == 'Y') {
                    do {
                        system("cls");
                        printf("\nPlease enter a new password (7-10 characters): ");

                        char* tempPass = getPassword();

                        printf("\nPlease re-enter to confirm new password: ");
                        char* confirmPass = getPassword();

                        verify = strcmp(tempPass, confirmPass);
                        if (verify != 0) {
                            printf("\n\nIncorrect password. Please enter again\n\n");
                        }
                        else {
                            strcpy(newDetails.memberPass, confirmPass);
                        }
                    } while (verify != 0);
                }
                else {
                    strcpy(newDetails.memberPass, allMembers[i].memberPass);
                }
                printf("\n\n Do you want to edit name? (Y/N): ");
                scanf("%c", &decision);
                rewind(stdin);
                if (toupper(decision) == 'Y') {
                    printf("New name: ");
                    scanf("%s", &newDetails.memberName);
                    rewind(stdin);
                }
                else {
                    strcpy(newDetails.memberName, allMembers[i].memberName);
                }
                printf("\n\n Do you want to edit phone number? (Y/N): ");
                scanf("%c", &decision);
                rewind(stdin);
                if (toupper(decision) == 'Y') {
                    printf("New phone number (XXXXXXXXXX or XXXXXXXXXXX): ");
                    scanf("%s", &newDetails.memberPhoneNo);
                    rewind(stdin);
                }
                else {
                    strcpy(newDetails.memberPhoneNo, allMembers[i].memberPhoneNo);
                }
                printf("\n\n Do you want to edit Joined Day? (Y/N): ");
                scanf("%c", &decision);
                rewind(stdin);
                if (toupper(decision) == 'Y') {
                    printf("New joined day: ");
                    scanf("%d", &newDetails.memberJoinDate.day);
                    rewind(stdin);
                }
                else {
                    newDetails.memberJoinDate.day = allMembers[i].memberJoinDate.day;
                }
                printf("\n\n Do you want to edit Joined Month? (Y/N): ");
                scanf("%c", &decision);
                rewind(stdin);
                if (toupper(decision) == 'Y') {
                    printf("New joined month: ");
                    scanf("%d", &newDetails.memberJoinDate.month);
                    rewind(stdin);
                }
                else {
                    newDetails.memberJoinDate.month = allMembers[i].memberJoinDate.month;
                }
                printf("\n\n Do you want to edit Joined Year? (Y/N): ");
                scanf("%c", &decision);
                rewind(stdin);
                if (toupper(decision) == 'Y') {
                    printf("New joined year: ");
                    scanf("%d", &newDetails.memberJoinDate.year);
                    rewind(stdin);
                }
                else {
                    newDetails.memberJoinDate.year = allMembers[i].memberJoinDate.year;
                }
                printf("\n\n Do you want to edit Reward Points? (Y/N): ");
                scanf("%c", &decision);
                rewind(stdin);
                if (toupper(decision) == 'Y') {
                    printf("New reward points: ");
                    scanf("%d", &newDetails.memberRewards);
                    rewind(stdin);
                }
                else {
                    newDetails.memberRewards = allMembers[i].memberRewards;
                }

                newDetails.memberWallet = allMembers[i].memberWallet;


                // Update the binary file
                FILE* fptrBin = fopen("member-details.bin", "wb");
                if (!fptrBin) {
                    printf("File cannot be opened.");
                    exit(-1);
                }
                for (int u = 0; u < numOfMembers; u++)
                {
                    if (strcmp(allMembers[u].memberID, tempID) != 0) {
                        fwrite(&allMembers[u], sizeof(MI_Member), 1, fptrBin);
                    }
                }
                fwrite(&newDetails, sizeof(MI_Member), 1, fptrBin);
                printf("\n\nDetails updated.\n\n");
                fclose(fptrBin);
            }
        }
    }while (G_ContinueOrStopCRUDRecord("Do you want to edit more members? (Y/N): ") == 1);
    free(allMembers);


};
/*for admin use*/ void MI_staffAddMemberViaTxt() {
    char fileName[100];
    do {
        system("cls");
        printf("Enter File Name: ");
        scanf("%s", &fileName);
        rewind(stdin);

        int numOfMembers = MI_getNumberOfMembers();
        MI_Member* allMembers = MI_getMemberDetails(numOfMembers);

        int numOfNewMembers = MI_getNumberOfNewMembersViaTxt(fileName);
        MI_Member* newMembers = MI_getNewMemberDetailsViaTxt(fileName, numOfNewMembers);

        FILE* fptr2 = fopen("member-details.bin", "ab");
        if (fptr2 == NULL) {
            printf("\nError opening file.");
            system("pause");
            exit(-1);
        }

        for (int o = 0; o < numOfNewMembers; o++) {
            int verify = MI_verifyRegister(newMembers[o].memberID);
            if (verify == 1) {
                printf("Member with member ID \"%s\" already existed.\n\n", newMembers[o].memberID);
            }
            else {
                fwrite(&newMembers[o], sizeof(MI_Member), 1, fptr2);
                printf("Member %s has been registered successfully!\n\n", newMembers[o].memberName);
            }
        };
        fclose(fptr2);
        free(newMembers);
        free(allMembers);
    } while (G_ContinueOrStopCRUDRecord("Do you want to add more members via txt file? (Y/N): "));
};
/*for admin use*/ int MI_getNumberOfNewMembersViaTxt(char* fileName) {
    FILE* fptr = fopen(fileName, "r");
    if (fptr == NULL) {
        printf("\nError opening file.");
        return -1; // Return -1 to indicate an error
    }

    // Count the rows of data in the file
    int row = 0;
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), fptr) != NULL) {
        row++;
    }
    rewind(fptr);
    fclose(fptr); // Close the file before returning
    return row;
}
/*for admin use*/ MI_Member* MI_getNewMemberDetailsViaTxt(char* fileName, int rowsOfNewMembers) {
    FILE* fptr = fopen(fileName, "r");
    if (fptr == NULL) {
        printf("\nError opening file.");
        return NULL;
    }

    // Allocate memory for the array of structures based on the number of lines
    MI_Member* newMembers = (MI_Member*)malloc(rowsOfNewMembers * sizeof(MI_Member));
    if (newMembers == NULL) {
        perror("Memory allocation failed");
        fclose(fptr);
        return NULL;
    }

    for (int i = 0; i < rowsOfNewMembers; i++) {
        if (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d %d %d|%d|%lf\n",
            &newMembers[i].memberID, &newMembers[i].memberPass,
            &newMembers[i].memberName, &newMembers[i].memberPhoneNo,
            &newMembers[i].memberJoinDate.day, &newMembers[i].memberJoinDate.month, &newMembers[i].memberJoinDate.year,
            &newMembers[i].memberRewards, &newMembers[i].memberWallet) != 9)
        {
            fprintf(stderr, "Error reading line %d\n", i + 1);
            fclose(fptr);
            free(newMembers);
            return NULL;
        }
    
    }

    fclose(fptr);
    return newMembers;
}




