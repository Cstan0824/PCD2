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
}Date;

typedef struct {
    char memberID[11];
    char memberPass[11];
    char memberName[50];
    char memberPhoneNo[12];
    Date memberJoinDate;
    int memberRewards;
    double memberWallet;
}MI_Member;

//Function Declarations
Date MI_getDate();
MI_Member* MI_getMemberDetails(int rows);
void MI_shiftSpace(int MI_position);
void MI_drawTrain();
int MI_getNumberOfMembers();
void MI_verifyLogin(char inputID[11], char inputPass[11]);
int MI_memberLogin();
int MI_verifyRegister(char memberID[11]);
void MI_registerMember();
void MI_displayDetailsProcess(MI_Member* allMembers, int rows, char* memberID);
void MI_displayDetails(char* memberID);
void MI_searchMember();
void MI_deleteMember();
void MI_mainMenu();
void MI_topupWallet();


//Main function
int main() {

    MI_mainMenu();

    return 0;
}

//Functions
void MI_mainMenu() {
    MI_drawTrain();

    int MI_menuDecision;
    printf("Welcome to Train Ticketing System\n\n");
    printf("Choose your mode: \n");
    printf("1. Member Login\n");
    printf("2. Member Register\n");
    printf("3. Exit Program\n\n\n");

    printf("Mode: ");
    scanf("%d", &MI_menuDecision);
    rewind(stdin);

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
        MI_mainMenu();
    }

};

void MI_registerMember() {

    MI_Member newMember;
    int verify;

    FILE* fptr = fopen("member-details.txt", "a");
    if (fptr == NULL) {
        printf("\nError opening file.");
        system("pause");
        exit(-1);
    }

    MI_drawTrain();

    do {
        printf("Please enter a new Member ID (7-10 characters): ");
        scanf("%s", &newMember.memberID);
        rewind(stdin);
        verify = MI_verifyRegister(newMember.memberID);

        if (verify == 1) {
            printf("\nInvalid member ID. Member ID already exist.\n");
        }
    } while (verify == 1);

    printf("\n\nPlease enter a password (7-10 characters): ");
    int i = 0;
    while (i < 10) {
        char c = _getch();
        if (c == '\r') // Enter key
            break;
        newMember.memberPass[i++] = c;
        printf("*");
    }
    newMember.memberPass[i] = '\0'; // Null-terminate the password string
    rewind(stdin);

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
    printf("\n\nSaving...\n\n");
    system("pause");
    fprintf(fptr, "%s|%s|%s|%s|%d %d %d|%d|%.2lf\n", newMember.memberID, newMember.memberPass, newMember.memberName, newMember.memberPhoneNo,
        newMember.memberJoinDate.day, newMember.memberJoinDate.month, newMember.memberJoinDate.year, newMember.memberRewards, newMember.memberWallet);
    fclose(fptr);
    printf("Member %s had registered sucessfully! \n\n", newMember.memberName);
};

int MI_memberLogin() {
    char inputID[11], inputPass[11];
    MI_drawTrain();

    printf("Please enter your Member ID (7-10 characters): ");
    scanf("%s", &inputID);
    rewind(stdin);
    printf("\n\nPlease enter your password (7-10 characters): ");
    int i = 0;
    while (i < 10) {
        char c = _getch();
        if (c == '\r') // Enter key
            break;
        inputPass[i++] = c;
        printf("*");
    }
    inputPass[i] = '\0'; // Null-terminate the password string

    MI_verifyLogin(inputID, inputPass);
    return 0;
};

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

/*for admin use*/ void MI_searchMember() {
    int MI_searchYear, MI_searchMonth, MI_searchDay;
    char MI_searchID[11];
    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);

    //based on year
    printf("Search member joined on year: ");
    scanf("%d", &MI_searchYear);
    rewind(stdin);
    printf("\n\n");
    for (int i = 0; i < numOfMembers; i++) {
        // Extract the year part from the member's join date and compare with the desired year
        int year = allMembers[i].memberJoinDate.year;

        if (year == MI_searchYear) {
            // Print the member's details if they joined in the desired year
            printf("%-15s %-15s %-50s %-15s %-15s %-15s %-20s\n", "Member ID", "Password", "Name", "Phone Number", "Join Date",
                "Reward Points", "Wallet");
            printf("%-15s %-15s %-50s %-15s %02d-%02d-%-09d %-15d %-12.2lf\n", allMembers[i].memberID, allMembers[i].memberPass,
                allMembers[i].memberName, allMembers[i].memberPhoneNo,
                allMembers[i].memberJoinDate.day, allMembers[i].memberJoinDate.month, allMembers[i].memberJoinDate.year, allMembers[i].memberRewards,
                allMembers[i].memberWallet);
        }
    }

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
            printf("%-15s %-15s %-50s %-15s %-15s %-15s %-20s\n", "Member ID", "Password", "Name", "Phone Number", "Join Date",
                "Reward Points", "Wallet");
            printf("%-15s %-15s %-50s %-15s %02d-%02d-%-09d %-15d %-12.2lf\n", allMembers[i].memberID, allMembers[i].memberPass,
                allMembers[i].memberName, allMembers[i].memberPhoneNo,
                allMembers[i].memberJoinDate.day, allMembers[i].memberJoinDate.month, allMembers[i].memberJoinDate.year, allMembers[i].memberRewards,
                allMembers[i].memberWallet);
        }
    }
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
            printf("%-15s %-15s %-50s %-15s %-15s %-15s %-20s\n", "Member ID", "Password", "Name", "Phone Number", "Join Date",
                "Reward Points", "Wallet");
            printf("%-15s %-15s %-50s %-15s %02d-%02d-%-09d %-15d %-12.2lf\n", allMembers[i].memberID, allMembers[i].memberPass,
                allMembers[i].memberName, allMembers[i].memberPhoneNo,
                allMembers[i].memberJoinDate.day, allMembers[i].memberJoinDate.month, allMembers[i].memberJoinDate.year, allMembers[i].memberRewards,
                allMembers[i].memberWallet);
        }
    }
    //based on memberID
    printf("Search member by member ID: ");
    scanf("%s", &MI_searchID);
    rewind(stdin);
    printf("\n\n");
    for (int i = 0; i < numOfMembers; i++) {

        if (strcmp(MI_searchID, allMembers[i].memberID) == 0) {
            printf("%-15s %-15s %-50s %-15s %-15s %-15s %-20s\n", "Member ID", "Password", "Name", "Phone Number", "Join Date",
                "Reward Points", "Wallet");
            printf("%-15s %-15s %-50s %-15s %02d-%02d-%-09d %-15d %-12.2lf\n", allMembers[i].memberID, allMembers[i].memberPass,
                allMembers[i].memberName, allMembers[i].memberPhoneNo,
                allMembers[i].memberJoinDate.day, allMembers[i].memberJoinDate.month, allMembers[i].memberJoinDate.year, allMembers[i].memberRewards,
                allMembers[i].memberWallet);
        }
    }
    free(allMembers);
};

/*for admin use*/ void MI_deleteMember() {
    char tempID[11];
    char decision;
    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);



    printf("Enter member ID to cancel membership: ");
    scanf("%s", tempID);
    rewind(stdin);


    for (int i = 0; i < numOfMembers; i++) {
        if (strcmp(allMembers[i].memberID, tempID) == 0) {
            MI_displayDetails(allMembers[i].memberID);
            printf("\n\n Are you sure you want to delete? (Y/N): ");
            scanf("%c", &decision);
            rewind(stdin);
            if (decision == 'Y' || decision == 'y') {
                // Update the text file
                FILE* fptrTxt = fopen("member-details.txt", "w");
                if (!fptrTxt) {
                    printf("File cannot be opened.");
                    exit(-1);
                }
                for (int i = 0; i < numOfMembers; i++)
                {

                    if (strcmp((allMembers + i)->memberID, tempID) != 0) {
                        fprintf(fptrTxt, "%s|%s|%s|%s|%d %d %d|%d|%.2lf\n", allMembers[i].memberID,
                            allMembers[i].memberPass, allMembers[i].memberName, allMembers[i].memberPhoneNo,
                            allMembers[i].memberJoinDate.day, allMembers[i].memberJoinDate.month,
                            allMembers[i].memberJoinDate.year, allMembers[i].memberRewards, allMembers[i].memberWallet);
                    }
                    else {
                        printf("Record Deleted\n");
                    }


                }
                fclose(fptrTxt);

            }



        }
    }





    /*for (int i = 0; i < numOfMembers; i++) {
        fprintf(fptrTxt, "%s|%s|%s|%s|%d %d %d|%d\n", allMembers[i].memberID, allMembers[i].memberPass, allMembers[i].memberName, allMembers[i].memberPhoneNo, allMembers[i].memberJoinDate.day, allMembers[i].memberJoinDate.month, allMembers[i].memberJoinDate.year, allMembers[i].memberRewards);
    }
    */

    // Free dynamically allocated memory
    free(allMembers);
}

void MI_topupWallet() {

    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);
    



}

//Sub-functions
int MI_getNumberOfMembers() {
    FILE* fptr = fopen("member-details.txt", "r");
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

MI_Member* MI_getMemberDetails(int rows) {
    FILE* fptr = fopen("member-details.txt", "r");
    if (fptr == NULL) {
        printf("\nError opening file.");
        return NULL;
    }

    // Allocate memory for the array of structures based on the number of lines
    MI_Member* allMembers = (MI_Member*)malloc(rows * sizeof(MI_Member));
    if (allMembers == NULL) {
        perror("Memory allocation failed");
        fclose(fptr);
        return NULL;
    }

    for (int i = 0; i < rows; i++) {
        if (fscanf(fptr, "%10[^|]|%10[^|]|%49[^|]|%11[^|]|%2d %2d %4d|%10d|%lf\n",
            allMembers[i].memberID, allMembers[i].memberPass,
            allMembers[i].memberName, allMembers[i].memberPhoneNo,
            &allMembers[i].memberJoinDate.day, &allMembers[i].memberJoinDate.month, &allMembers[i].memberJoinDate.year,
            &allMembers[i].memberRewards, &allMembers[i].memberWallet) != 9)
        {
            fprintf(stderr, "Error reading line %d\n", i + 1);
            fclose(fptr);
            free(allMembers);
            return NULL;
        }
        //// Debug print to check if data is read correctly
        //printf("Read member %d: %s %s %s %s %s\n", i + 1, allMembers[i].memberID,
        //    allMembers[i].memberPass, allMembers[i].memberName,
        //    allMembers[i].memberPhoneNo, allMembers[i].memberJoinDate);
    }

    fclose(fptr);
    return allMembers;
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

void MI_verifyLogin(char inputID[11], char inputPass[11]) {
    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);
    for (int i = 0; i < numOfMembers; i++) {
        if (strcmp(allMembers[i].memberID, inputID) == 0) {
            if (strcmp(allMembers[i].memberPass, inputPass) == 0) {
                printf("\n\n\nLogin successfull, %s", allMembers[i].memberName);
            }
            else {
                printf("\n\n\nLogin failed, wrong id or password.");
            }
        }
    }
    free(allMembers);

};

int MI_verifyRegister(char memberID[11]) {

    int numOfMembers = MI_getNumberOfMembers();
    MI_Member* allMembers = MI_getMemberDetails(numOfMembers);
    for (int i = 0; i < numOfMembers; i++) {

        if (strcmp(memberID, allMembers[i].memberID) == 0) {
            return 1;
            break;
        }

    }

};

void MI_displayDetailsProcess(MI_Member* allMembers, int rows, char* memberID) {
    printf("%-15s %-15s %-50s %-15s %-15s %-15s %-20s\n", "Member ID", "Password", "Name", "Phone Number", "Join Date", "Reward Points", "Wallet");

    for (int i = 0; i < rows; i++) {
        if (strcmp(allMembers[i].memberID, memberID) == 0) {
            printf("%-15s %-15s %-50s %-15s %02d-%02d-%-09d %-15d %-12.2lf\n", allMembers[i].memberID, allMembers[i].memberPass,
                allMembers[i].memberName, allMembers[i].memberPhoneNo,
                allMembers[i].memberJoinDate.day, allMembers[i].memberJoinDate.month, allMembers[i].memberJoinDate.year, allMembers[i].memberRewards,
                allMembers[i].memberWallet);
        }
    }
}

void MI_shiftSpace(int MI_position) {
    for (int i = 0; i < MI_position; i++) {
        printf(" ");
    }
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



