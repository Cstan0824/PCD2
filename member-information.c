#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

#pragma warning (disable:4996)

#define MAX_LINE_LENGTH 256

typedef char* string;


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
} Member;

typedef struct {
    char memberID[11];
    char memberPass[11];
    char memberName[50];
    char memberPhoneNo[12];
    char memberJoinDate[11];
    char memberRewards[10];
    char memberWallet[10];
} ValidateTxt;

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

Member loggedInMember;


// Function Declarations
Date MI_getDate();
void MI_shiftSpace(int MI_position);
void MI_drawTrain();

// Get info from binary file
int MI_getNumberOfMembers();
Member* MI_getMemberDetails(int rows);
void MI_loggedInMemberDetails(string memberID);

//Member Login/Register
string MI_getPassword();
int MI_verifyRegister(string memberID);
void MI_registerMember();
int MI_mainMenu();
void MI_memberMenu();
void MI_memberLogin();
int MI_verifyLogin(string inputID, string inputPass);

//Member functions
void MI_displayDetails(string memberID);
void MI_displayDetailsProcess(Member* allMembers, int rows, string memberID);
void MI_topUpWallet(string memberID);
void MI_MemberEditDetails(string memberID);
void MI_deleteAccount(string memberID);

//Validation
int G_ConfirmationIsValidated(string prompt);
void G_ErrorMessage();
int G_MenuValidation(string decision, int range);
int G_IntIsValidated(string prompt, int range, int* output);
int MI_InputDetailsValidation(string value, string mode);
void MI_ErrorMessageForInputLength(int min, int max);

//Admin Functions
/*for admin use*/ void MI_displayAllMembers();
/*for admin use*/ void MI_displayMembersHeaderOrFooter(char HeaderOrFooter);
/*for admin use*/ void MI_staffSearchMember();
/*for admin use*/ void MI_displaySearchedMembers(string memberID);
/*for admin use*/ void MI_staffDeleteMember();
/*for admin use*/ void MI_staffEditMemberDetails();
/*for admin use*/ void MI_staffAddMember();

/*for admin use*/ void MI_staffAddMemberViaTxt();
void AddMemberViaTxtFileProcess(const char* fileName);


// Main function
int main() {

    MI_drawTrain();

    return 0;
}

//Global Functions
int G_ConfirmationIsValidated(string prompt) {
    char confirmation[3] = "";
    int inputIsError = 0;
    do
    {
        if (inputIsError) G_ErrorMessage();

        printf("%s", prompt);
        scanf("%[^\n]", &confirmation);
        rewind(stdin);

        strcpy(confirmation, strupr(confirmation));
    } while (inputIsError = !(strncmp(confirmation, "Y", 3) == 0 || strncmp(confirmation, "N", 3) == 0));

    return strncmp(confirmation, "Y", 3) == 0;
}
void G_ErrorMessage() {

    printf("\n\nInvalid input. Please type again.\n\n");
    system("pause");
};
int G_MenuValidation(string decision, int range) {
    char digit[3];
    for (int i = 1; i <= range; i++) {
        sprintf(digit, "%d", i);
        if (strcmp(decision, digit) == 0) {

            return 1; //return true if same else continue the looping

        }
    }
    return 0; // same return false if input not in range
};
int G_IntIsValidated(string prompt, int range, int* output) {
    char digit[3] = "";
    char decision[1000] = "";
    printf("%s", prompt);
    scanf("%s", decision);
    rewind(stdin);
    if (strcmp(decision, "0") == 0) {
        return -1;
    }
    for (int i = 1; i <= range; i++) {
        sprintf(digit, "%d", i);
        if (strcmp(decision, digit) == 0) {
            *output = i;
            return 0; //return true if same else continue the looping
            //end the do while looping
        }
    }
    return 1; // same return false if input not in range
}


// Functions
int MI_mainMenu() {
    int validation = 0;
    int MI_menuDecision;
    do {

        system("cls");
        printf("Welcome to Train Ticketing System\n\n");
        printf("Choose your mode: \n");
        printf("1. Member Login\n");
        printf("2. Member Register\n");
        printf("3. Exit Program\n\n\n");

    } while (G_IntIsValidated("Mode: ", 3, &MI_menuDecision));


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

    return 1;
}

void MI_memberMenu() {
    int MI_memberMenuDecision;
    int validation = 0;
    do {
        system("cls");
        printf("Welcome, %s\n\n\n", loggedInMember.memberName);
        MI_displayDetails(loggedInMember.memberID);
        printf("Choose your mode: \n");
        printf("1. Top Up Wallet\n");
        printf("2. Edit Details\n");
        printf("3. Check Ticket Schedule\n");
        printf("4. Book Tickets\n");
        printf("5. Booking Details\n");
        printf("6. Delete Account\n");
        printf("7. Exit Program\n\n\n");

    } while (G_IntIsValidated("Mode: ", 3, &MI_memberMenuDecision));


    switch (MI_memberMenuDecision) {
    case 1:
        MI_topUpWallet(loggedInMember.memberID);
        break;
    case 2:
        MI_MemberEditDetails(loggedInMember.memberID);
        break;
    case 3:
        /*TS_showSchedule();*/
        break;
    case 4:
        /* TB_BookingTicket();*/
        break;
    case 5:
        /*TB_GetBookingDetails();*/
        break;
    case 6:
        MI_deleteAccount(loggedInMember.memberID);
        break;
    case 7:
        exit(-1);
        break;
    default:
        printf("Invalid input. Redirecting...\n\n");
        system("pause");
        system("cls");
        MI_memberMenu();
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
    rows = fileSize / sizeof(Member);
    fclose(fptr);
    return rows;
};

Member* MI_getMemberDetails(int rows) {
    FILE* fptr = fopen("member-details.bin", "rb");
    if (fptr == NULL) {
        printf("\nError opening file.");
        exit(-1);
    }

    // Allocate memory for member details
    Member* allMembers = (Member*)malloc((rows) * sizeof(Member));
    if (allMembers == NULL) {
        printf("\nMemory allocation failed.");
        fclose(fptr);
        exit(-1);
    }

    // Read member details from the binary file
    fread(allMembers, sizeof(Member), rows, fptr);
    return allMembers;
}

void MI_loggedInMemberDetails(string memberID) {

    int numOfMembers = MI_getNumberOfMembers();
    Member* allMembers = MI_getMemberDetails(numOfMembers);

    for (int i = 0; i < numOfMembers; i++) {
        if (strcmp(allMembers[i].memberID, memberID) == 0) {

            loggedInMember = allMembers[i];

        }

    }
    free(allMembers);
};

void MI_registerMember() {
    Member newMember;
    int verify;
    int validation = 0;
    char tempPass[11] = "", confirmPass[11] = "";

    do {
        do {
            system("cls");
            printf("\n\nPlease enter a new Member ID (at least 7, maximum 10 characters): ");
            scanf("%s", &newMember.memberID);
            rewind(stdin);
        } while (MI_InputDetailsValidation(newMember.memberID, "idOrPass") != 0);
        verify = MI_verifyRegister(newMember.memberID);

        if (verify == 1) {
            printf("\nInvalid member ID. Member ID already exists.\n");
            system("pause");
        }
    } while (verify == 1);

    verify = 0;

    do {
        do {
            system("cls");
            printf("\n\nPlease enter a password (at least 7, maximum 10 characters): ");

            strncpy(tempPass, MI_getPassword(), 11);
            validation = MI_InputDetailsValidation(tempPass, "idOrPass");
        } while (validation != 0);


        printf("\n\nPlease re-enter to confirm password: ");
        strncpy(confirmPass, MI_getPassword(), 11);

        verify = strncmp(tempPass, confirmPass, 11);
        if (verify != 0) {
            printf("\n\nIncorrect password. Please enter again\n\n");
            system("pause");
        }
        else {
            strncpy(newMember.memberPass, confirmPass, 11);
        }
    } while (verify != 0);

    do {
        printf("\n\nPlease enter your name: ");
        scanf("%[^\n]", &newMember.memberName);
        rewind(stdin);
    } while (MI_InputDetailsValidation(newMember.memberName, "name") != 0);

    do {
        printf("\n\nPlease enter your phone number (01XXXXXXXX or 01XXXXXXXXX): ");
        scanf("%s", &newMember.memberPhoneNo);
        rewind(stdin);
    } while (MI_InputDetailsValidation(newMember.memberPhoneNo, "phoneNo") != 0);

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
    fwrite(&newMember, sizeof(Member), 1, fptr);
    fclose(fptr);

    printf("Member %s has been registered successfully!\n\n", newMember.memberName);
    system("pause");
    system("cls");
    MI_mainMenu();
}

int MI_verifyRegister(string memberID) {
    int rows = MI_getNumberOfMembers();
    Member* allMembers = MI_getMemberDetails(rows);
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
    do {
        printf("\n\nPlease enter your Member ID (at least 7, maximum 10 characters): ");
        scanf("%s", &inputID);
        rewind(stdin);
        printf("\n\nPlease enter your password (at least 7, maximum 10 characters): ");
        string inputPass = MI_getPassword();

        verify = MI_verifyLogin(inputID, inputPass);
        if (verify == 0) {
            MI_loggedInMemberDetails(inputID);
            MI_memberMenu();
        }
        else {
            continue;
        }
    } while (verify != 0);

}

int MI_verifyLogin(string inputID, string inputPass) {
    int numOfMembers = MI_getNumberOfMembers();
    Member* allMembers = MI_getMemberDetails(numOfMembers);
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

void MI_topUpWallet(string memberID) {
    char tempWallet[11];
    double tempWallet2;
    int numOfMembers = MI_getNumberOfMembers();
    Member* allMembers = MI_getMemberDetails(numOfMembers);
    Member newDetails = { 0 };
    for (int i = 0; i < numOfMembers; i++) {

        if (strcmp(allMembers[i].memberID, memberID) == 0) {
            do {
                printf("\n\nCurrent Wallet: %.2lf\n\n", allMembers[i].memberWallet);
                printf("Top up amount: ");
                scanf("%s", &tempWallet);
                rewind(stdin);
            } while (MI_InputDetailsValidation(tempWallet, "topUp") != 0);
            strtod(tempWallet, &tempWallet2);
            tempWallet2 = tempWallet2 + allMembers[i].memberWallet;
            printf("\n\nWallet amount after top up: %.2lf\n\n", tempWallet2);
            system("pause");
            strcpy(newDetails.memberID, memberID);
            strcpy(newDetails.memberPass, allMembers[i].memberPass);
            strcpy(newDetails.memberName, allMembers[i].memberName);
            strcpy(newDetails.memberPhoneNo, allMembers[i].memberPhoneNo);
            newDetails.memberJoinDate.day = allMembers[i].memberJoinDate.day;
            newDetails.memberJoinDate.month = allMembers[i].memberJoinDate.month;
            newDetails.memberJoinDate.year = allMembers[i].memberJoinDate.year;
            newDetails.memberRewards = allMembers[i].memberRewards;
            newDetails.memberWallet = tempWallet2;
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
            fwrite(&allMembers[i], sizeof(Member), 1, fptrBin);
        }
    }
    fwrite(&newDetails, sizeof(Member), 1, fptrBin);
    printf("Wallet updated.\n\n");
    fclose(fptrBin);
    free(allMembers);
    MI_memberMenu(memberID);
}

void MI_MemberEditDetails(string memberID) {
    int numOfMembers = MI_getNumberOfMembers();
    if (numOfMembers == -1) {
        printf("Failed to get number of members.\n");
        exit(-1);
    }

    Member* allMembers = MI_getMemberDetails(numOfMembers);
    if (allMembers == NULL) {
        printf("Failed to get member details.\n");
        exit(-1);
    }

    printf("Current details: \n\n");
    MI_displayDetails(memberID);

    Member newDetails;
    int verify;
    int validation = 0;
    char tempPass[11] = "", confirmPass[11] = "";
    for (int i = 0; i < numOfMembers; i++) {
        if (strcmp(memberID, allMembers[i].memberID) == 0) {
            strcpy(newDetails.memberID, memberID);
            if (G_ConfirmationIsValidated("\n\nDo you want to edit password? (Y/N): ")) {
                do {
                    do {
                        system("cls");
                        printf("\n\nPlease enter a new password (at least 7, maximum 10 characters): ");

                        strncpy(tempPass, MI_getPassword(), 11);
                        validation = MI_InputDetailsValidation(tempPass, "idOrPass");

                    } while (validation != 0);
                    printf("\nPlease re-enter to confirm new password: ");
                    strncpy(confirmPass, MI_getPassword(), 11);
                    verify = strncmp(tempPass, confirmPass, 11);
                    if (verify != 0) {
                        printf("\n\nIncorrect password. Please enter again\n\n");
                        system("pause");
                    }
                    else {
                        strcpy(newDetails.memberPass, confirmPass);
                    }
                } while (verify != 0);
            }
            else {
                strcpy(newDetails.memberPass, allMembers[i].memberPass);
            }
            if (G_ConfirmationIsValidated("\n\nDo you want to edit name? (Y/N): ")) {
                do {
                    printf("New name: ");
                    scanf("%s", &newDetails.memberName);
                    rewind(stdin);
                } while (MI_InputDetailsValidation(newDetails.memberName, "name") != 0);
            }
            else {
                strcpy(newDetails.memberName, allMembers[i].memberName);
            }
            if (G_ConfirmationIsValidated("\n\nDo you want to edit phone number? (Y/N): ")) {
                do {
                    printf("New Phone Number (01XXXXXXXX or 01XXXXXXXXX): ");
                    scanf("%s", &newDetails.memberPhoneNo);
                    rewind(stdin);
                } while (MI_InputDetailsValidation(newDetails.memberPhoneNo, "phoneNo") != 0);
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
                    fwrite(&allMembers[u], sizeof(Member), 1, fptrBin);
                }
            }
            fwrite(&newDetails, sizeof(Member), 1, fptrBin);
            printf("\n\nDetails updated.\n\n");
            fclose(fptrBin);
        }
    }
    free(allMembers);
};

void MI_deleteAccount(string memberID) {
    int numOfMembers = MI_getNumberOfMembers();
    Member* allMembers = MI_getMemberDetails(numOfMembers);

    if (G_ConfirmationIsValidated("Are you sure you want to delete account? (Y/N): ")) {
        // Update the binary file
        FILE* fptrBin = fopen("member-details.bin", "wb");
        if (!fptrBin) {
            printf("File cannot be opened.");
            exit(-1);
        }
        for (int i = 0; i < numOfMembers; i++)
        {

            if (strcmp(allMembers[i].memberID, memberID) != 0) {
                fwrite(&allMembers[i], sizeof(Member), 1, fptrBin);
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

void MI_displayDetails(string memberID) {
    int numOfMembers = MI_getNumberOfMembers();
    if (numOfMembers == -1) {
        printf("Failed to get number of members.\n");
        return;
    }

    Member* allMembers = MI_getMemberDetails(numOfMembers);
    if (allMembers == NULL) {
        printf("Failed to get member details.\n");
        return;
    }

    MI_displayDetailsProcess(allMembers, numOfMembers, memberID);

    // Free dynamically allocated memory
    free(allMembers);
}

void MI_displayDetailsProcess(Member* allMembers, int rows, string memberID) {


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

void MI_drawTrain() {
    int MI_position = 0;
    int MI_maxPosition = 50;
    for (int i = 0; i < MI_maxPosition; i++) {
        system("cls");
        MI_shiftSpace(MI_position);
        printf("   ___________   ___________   ___________   _______________________________________^__\n");
        MI_shiftSpace(MI_position);
        printf("||| ___   ___ ||| ___   ___ ||| ___   ___ |||  ___   ___   ___    ___ ___  |   __  ,----\\\n");
        MI_shiftSpace(MI_position);
        printf("||||   | |   |||||   | |   |||||   | |   |||| |   | |   | |   |  |   |   | |  |  | |_____\\\n");
        MI_shiftSpace(MI_position);
        printf("||||___| |___|||||___| |___|||||___| |___|||| |___| |___| |___|  | O | O | |  |  |        \\\n");
        MI_shiftSpace(MI_position);
        printf("|||           |||           |||           |||                                        /--------\n");
        MI_shiftSpace(MI_position);
        printf("'''-----------'''-----------'''-----------'''---------------------------------------'\n");
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

string MI_getPassword() {
    int i = 0;
    static char tempPass[11]; // Static array to persist after function returns

    do {
        char c = _getch();

        if (c == '\r') { // Enter key
            break;
        }
        else if (c == '\b') { // Backspace
            if (i > 0) {
                i--;
                printf("\b \b"); // Move cursor back, print space, move cursor back again
            }
        }
        else if ((int)c <= 33 || (int)c >= 126) { // Check if printable character
            continue;
        }
        else {
            if (i < 10) { // Limiting the password length
                tempPass[i++] = c;
                printf("*");
            }
        }
    } while (1);

    tempPass[i] = '\0'; // Null-terminate the password string
    printf("\n"); // Print newline after password entry
    return tempPass;
}

void MI_ErrorMessageForInputLength(int min, int max) {
    //for input data length (strlen)
    printf("\n\nInput format wrong. Please enter at least %d, maximum %d characters only.\n\n", min, max);

};

int MI_InputDetailsValidation(string value, string mode) {
    //to check memberID or Pass to follow format
    if (strcmp(mode, "idOrPass") == 0) {
        if (strlen(value) < 7 || strlen(value) > 10) {

            MI_ErrorMessageForInputLength(7, 10);
            system("pause");
            return 1;
        }
        return 0;
    }
    //check name length
    else if (strcmp(mode, "name") == 0) {

        if (strlen(value) > 50 || strlen(value) < 3) {
            MI_ErrorMessageForInputLength(3, 50);
            system("pause");
            return 1;
        }
        else {
            //check name format
            for (int i = 0; i < strlen(value); i++) {

                if (isdigit(value[i]) != 0 || ispunct(value[i]) != 0) {
                    printf("\n\nYour name has invalid input (number/symbol). Please re-enter again.\n\n");
                    system("pause");
                    return 1;
                }

            }


        };
        return 0;
    }
    else if (strcmp(mode, "phoneNo") == 0) {
        //check phone number length
        if (strlen(value) < 10 || strlen(value) > 11) {

            MI_ErrorMessageForInputLength(10, 11);
            system("pause");
            return 1;
        }
        else {
            //check phone number format
            for (int i = 0; i < strlen(value); i++) {

                if (isdigit(value[i]) == 0) {
                    printf("\n\nYour phone number has invalid input (character/symbol). Please re-enter again.\n\n");
                    system("pause");
                    return 1;
                };

            };
            return 0;
        }
    }
    else if (strcmp(mode, "date-year") == 0) {
        //check search date format
        if (strlen(value) != 4) {
            G_ErrorMessage();
            system("pause");
            return 1;
        }
        else {
            for (int i = 0; i < strlen(value); i++) {

                if (isdigit(value[i]) != 0) {

                    printf("\n\nYour search year has invalid input (character/symbol). Please re-enter again.\n\n");
                    system("pause");
                    return 1;

                };

            };
        }
        return 0;

    }
    else if (strcmp(mode, "date-monthOrDay") == 0) {
        if (strlen(value) > 2) {
            G_ErrorMessage();
            system("pause");
            return 1;
        }
        else {
            for (int i = 0; i < strlen(value); i++) {

                if (isdigit(value[i]) != 0) {

                    printf("\n\nYour search has invalid input (character/symbol). Please re-enter again.\n\n");
                    system("pause");
                    return 1;

                };

            };
        }
        return 0;

    }
    else if (strcmp(mode, "rewards") == 0) {
        //check input rewards data type
        for (int i = 0; i < strlen(value); i++) {

            if (isdigit(value[i]) != 0) {

                printf("\n\nYour reward points has invalid input (character/symbol). Please re-enter again.\n\n");
                system("pause");
                return 1;

            };

        };
        return 0;

    }
    else if (strcmp(mode, "topUp") == 0) {
        //check topup amount format
        for (int i = 0; i < strlen(value); i++) {

            if (isdigit(value[i]) != 0) {
                if (value[i] != '.') {

                    printf("\n\nYour top up value has invalid input (character/symbol). Please re-enter again.\n\n");
                    system("pause");
                    return 1;
                };
            };

        };
        return 0;
    }


    return 0;

};


/*for admin use*/ void MI_displayAllMembers() {

    int numOfMembers = MI_getNumberOfMembers();
    Member* allMembers = MI_getMemberDetails(numOfMembers);
    MI_displayMembersHeaderOrFooter('H');
    for (int i = 0; i < numOfMembers; i++) {
        // Create a new string with asterisks for the password
        int passLength = (int)strlen(allMembers[i].memberPass);
        string asteriskPassword = (string)malloc((passLength + 1) * sizeof(char)); // +1 for null terminator
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
/*for admin use*/ void MI_displaySearchedMembers(string memberID) {
    int numOfMembers = MI_getNumberOfMembers();
    Member* allMembers = MI_getMemberDetails(numOfMembers);

    for (int i = 0; i < numOfMembers; i++) {
        if (strncmp(memberID, allMembers[i].memberID, 11) == 0) {
            // Create a new string with asterisks for the password
            int passLength = (int)strlen(allMembers[i].memberPass);
            string asteriskPassword = (string)malloc((passLength + 1) * sizeof(char)); // +1 for null terminator
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
    Member* allMembers = MI_getMemberDetails(numOfMembers);
    int MI_menuDecision;
    do {
        do {
            system("cls");

            printf("Search member through:\n\n");
            printf("1. Joined year\n");
            printf("2. Joined month\n");
            printf("3. Joined day\n");
            printf("4. Member ID\n");
            printf("5. Member Name\n");
            printf("6. Back to menu\n\n\n");


        } while (G_IntIsValidated("Mode: ", 6, &MI_menuDecision));

        char tempDate[10];
        switch (MI_menuDecision) {

        case 1:
            //based on year
            do {
                printf("Search member joined on year: ");
                scanf("%s", &tempDate);
                rewind(stdin);
            } while (MI_InputDetailsValidation(tempDate, "date") != 0);
            MI_searchYear = atoi(tempDate);
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
            do {
                printf("Search member joined on month: ");
                scanf("%s", &tempDate);
                rewind(stdin);
            } while (MI_InputDetailsValidation(tempDate, "date") != 0);
            printf("\n\n");
            MI_searchMonth = atoi(tempDate);
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
            do {
                printf("Search member joined on day: ");
                scanf("%s", &tempDate);
                rewind(stdin);
            } while (MI_InputDetailsValidation(tempDate, "date") != 0);
            printf("\n\n");
            MI_searchDay = atoi(tempDate);
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
            do {
                printf("Search member by member ID: ");
                scanf("%s", &MI_searchID);
                rewind(stdin);
            } while (MI_InputDetailsValidation(MI_searchID, "idOrPass") != 0);
            printf("\n\n");
            for (int i = 0; i < numOfMembers; i++) {

                if (strcmp(MI_searchID, allMembers[i].memberID) == 0) {
                    MI_displaySearchedMembers(allMembers[i].memberID);
                }
            }
            break;
        case 5:
            //based on member name
            do {
                printf("Search member by member name: ");
                scanf("%[^\n]", &MI_searchName);
                rewind(stdin);
            } while (MI_InputDetailsValidation(MI_searchName, "name") != 0);
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
            continue;
        }
    } while (G_ConfirmationIsValidated("Do you want to continue search for members? (Y/N)") == 1);
    free(allMembers);
};
/*for admin use*/ void MI_staffAddMember() {

    Member newMember;
    int verify;
    char tempPass[11] = "", confirmPass[11] = "";

    do {
        do {
            do {
                system("cls");
                printf("Please enter a new Member ID (at least 7, maximum 10 characters): ");
                scanf("%s", &newMember.memberID);
                rewind(stdin);
            } while (MI_InputDetailsValidation(newMember.memberID, "idOrPass") != 0);
            verify = MI_verifyRegister(newMember.memberID);

            if (verify == 1) {
                printf("\nInvalid member ID. Member ID already exists.\n");
                system("pause");
            }
        } while (verify == 1);

        verify = 0;

        do {
            do {
                system("cls");
                printf("\nPlease enter a password (at least 7, maximum 10 characters): ");

                strcpy(tempPass, MI_getPassword());
            } while (MI_InputDetailsValidation(tempPass, "idOrPass") != 0);

            printf("\nPlease re-enter to confirm password: ");
            strcpy(confirmPass, MI_getPassword());

            verify = strcmp(tempPass, confirmPass);
            if (verify != 0) {
                printf("\n\nIncorrect password. Please enter again\n\n");
            }
            else {
                strcpy(newMember.memberPass, confirmPass);
            }
        } while (verify != 0);


        do {
            printf("\n\nPlease enter new member's name: ");
            scanf("%[^\n]", &newMember.memberName);
            rewind(stdin);
        } while (MI_InputDetailsValidation(newMember.memberName, "name") != 0);

        do {
            printf("\n\nPlease enter new member's phone number: ");
            scanf("%s", &newMember.memberPhoneNo);
            rewind(stdin);
        } while (MI_InputDetailsValidation(newMember.memberPhoneNo, "phoneNo") != 0);


        newMember.memberJoinDate = MI_getDate();
        char tempReward[10];
        do {
            printf("\n\nPlease enter new member's reward points: ");
            scanf("%s", &tempReward);
            rewind(stdin);
        } while (MI_InputDetailsValidation(tempReward, "rewards") != 0);

        newMember.memberRewards = atoi(tempReward);
        newMember.memberWallet = 0;

        // Write member details to binary file
        FILE* fptr = fopen("member-details.bin", "ab");
        if (fptr == NULL) {
            printf("\nError opening file.");
            system("pause");
            exit(-1);
        }
        fwrite(&newMember, sizeof(Member), 1, fptr);
        fclose(fptr);

        printf("Member %s has been registered successfully!\n\n", newMember.memberName);
        system("pause");
        system("cls");
    } while (G_ConfirmationIsValidated("Do you want to add more members? (Y/N): ") == 1);
};
/*for admin use*/ void MI_staffDeleteMember() {
    char tempID[11] = "";
    int numOfMembers = MI_getNumberOfMembers();
    Member* allMembers = MI_getMemberDetails(numOfMembers);
    do {
        MI_displayAllMembers();

        printf("\n\nEnter member ID to cancel membership: ");
        scanf("%s", tempID);
        rewind(stdin);


        for (int i = 0; i < numOfMembers; i++) {
            if (strcmp(allMembers[i].memberID, tempID) == 0) {
                MI_displayDetails(allMembers[i].memberID);

                if (G_ConfirmationIsValidated("Are you sure you want to delete? (Y/N): ")) {
                    // Update the binary file
                    FILE* fptrBin = fopen("member-details.bin", "wb");
                    if (!fptrBin) {
                        printf("File cannot be opened.");
                        exit(-1);
                    }
                    for (int i = 0; i < numOfMembers; i++)
                    {

                        if (strcmp((allMembers + i)->memberID, tempID) != 0) {
                            fwrite(&allMembers[i], sizeof(Member), 1, fptrBin);
                        }
                        else {
                            printf("\n\nMember Deleted\n");
                        }

                    }
                    fclose(fptrBin);
                }

            }
        }
    } while (G_ConfirmationIsValidated("Do you want to continue delete more members? (Y/N): ") == 1);

    free(allMembers);

}
/*for admin use*/ void MI_staffEditMemberDetails() {
    char tempID[11];
    int numOfMembers = MI_getNumberOfMembers();
    Member* allMembers = MI_getMemberDetails(numOfMembers);
    do {
        MI_displayAllMembers();
        do {
            printf("Enter member ID to edit details: ");
            scanf("%s", &tempID);
            rewind(stdin);
        } while (MI_InputDetailsValidation(tempID, "idOrPass") != 0);
        MI_displayDetails(tempID);

        Member newDetails;
        int verify;
        char tempPass[11], confirmPass[11], tempYear[5], tempMonth[3], tempDay[3], tempReward[10];
        for (int i = 0; i < numOfMembers; i++) {
            if (strcmp(tempID, allMembers[i].memberID) == 0) {
                strcpy(newDetails.memberID, tempID);

                if (G_ConfirmationIsValidated("Do you want to edit password? (Y/N): ")) {
                    do {
                        do {
                            system("cls");
                            printf("\nPlease enter a new password (at least 7, maximum 10 characters): ");

                            strncpy(tempPass, MI_getPassword(), 11);
                        } while (MI_InputDetailsValidation(tempPass, "idOrPass") != 0);

                        printf("\nPlease re-enter to confirm new password: ");
                        strncpy(confirmPass, MI_getPassword(), 11);

                        verify = strcmp(tempPass, confirmPass);
                        if (verify != 0) {
                            printf("\n\nIncorrect password. Please enter again\n\n");
                        }
                        else {
                            strncpy(newDetails.memberPass, confirmPass, 11);
                        }
                    } while (verify != 0);
                }
                else {
                    strncpy(newDetails.memberPass, allMembers[i].memberPass, 11);
                }

                if (G_ConfirmationIsValidated("Do you want to edit name? (Y/N): ")) {
                    do {
                        printf("New name: ");
                        scanf("%s", &newDetails.memberName);
                        rewind(stdin);
                    } while (MI_InputDetailsValidation(newDetails.memberName, "name") != 0);
                }
                else {
                    strncpy(newDetails.memberName, allMembers[i].memberName, 11);
                }


                if (G_ConfirmationIsValidated("Do you want to edit phone number? (Y/N): ")) {
                    do {
                        printf("New phone number (XXXXXXXXXX or XXXXXXXXXXX): ");
                        scanf("%s", &newDetails.memberPhoneNo);
                        rewind(stdin);
                    } while (MI_InputDetailsValidation(newDetails.memberPhoneNo, "phoneNo") != 0);
                }
                else {
                    strncpy(newDetails.memberPhoneNo, allMembers[i].memberPhoneNo, 12);
                }

                if (G_ConfirmationIsValidated("Do you want to edit Joined Year? (Y/N): ")) {
                    do {
                        printf("New joined year: ");
                        scanf("%s", &tempYear);
                        rewind(stdin);
                    } while (MI_InputDetailsValidation(tempYear, "date-year") != 0);
                    newDetails.memberJoinDate.year = atoi(tempYear);

                }
                else {
                    newDetails.memberJoinDate.year = allMembers[i].memberJoinDate.year;
                }
                verify = 0;

                if (G_ConfirmationIsValidated("Do you want to edit Joined Month? (Y/N): ")) {

                    do {

                        do {
                            printf("New joined month: ");
                            scanf("%s", &tempMonth);
                            rewind(stdin);
                        } while (MI_InputDetailsValidation(tempMonth, "date-monthOrDay") != 0);

                        newDetails.memberJoinDate.month = atoi(tempMonth);

                        if (newDetails.memberJoinDate.month < 1 || newDetails.memberJoinDate.month > 12) {

                            printf("\n\nInvalid input. Month should be between 1 to 12.\n\n");
                            verify = 1;
                            system("pause");

                        }

                    } while (verify = 1);
                }
                else {
                    newDetails.memberJoinDate.month = allMembers[i].memberJoinDate.month;
                }
                verify = 0;
                int dayOfMonth[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
                if (G_ConfirmationIsValidated("Do you want to edit Joined Day? (Y/N): ")) {
                    do {
                        do {
                            printf("New joined day: ");
                            scanf("%s", &tempDay);
                            rewind(stdin);
                        } while (MI_InputDetailsValidation(tempDay, "date-monthOrDay") != 0);
                        newDetails.memberJoinDate.day = atoi(tempDay);

                        dayOfMonth[1] = newDetails.memberJoinDate.year % 4 + 28;

                        for (int i = 0; i < 12; i++) {
                            if (newDetails.memberJoinDate.month == i + 1) {
                                if (newDetails.memberJoinDate.day < 0 || newDetails.memberJoinDate.day > dayOfMonth[i]) {
                                    G_ErrorMessage();
                                    verify = 1;
                                }
                            }
                        }
                    } while (verify == 1);
                }
                else {
                    newDetails.memberJoinDate.day = allMembers[i].memberJoinDate.day;
                }

                if (G_ConfirmationIsValidated("Do you want to edit Reward Points? (Y/N): ")) {
                    do {
                        printf("New reward points: ");
                        scanf("%s", &tempReward);
                        rewind(stdin);
                    } while (MI_InputDetailsValidation(tempReward, "rewards") != 0);
                    newDetails.memberRewards = atoi(tempReward);
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
                        fwrite(&allMembers[u], sizeof(Member), 1, fptrBin);
                    }
                }
                fwrite(&newDetails, sizeof(Member), 1, fptrBin);
                printf("\n\nDetails updated.\n\n");
                fclose(fptrBin);
            }
        }
    } while (G_ConfirmationIsValidated("Do you want to edit more members? (Y/N): ") == 1);
    free(allMembers);


};


/*for admin use*/ void MI_staffAddMemberViaTxt() {
    char fileName[100];
    do {
        system("cls");
        printf("Enter File Name: ");
        scanf("%s", &fileName);
        rewind(stdin);

        AddMemberViaTxtFileProcess(fileName);


    } while (G_ConfirmationIsValidated("Do you want to add more members via txt file? (Y/N): "));
};
/*for admin use*/ int NewMemberIsValidated(const ValidateTxt* member) {
    // Check memberID length
    if (!(strlen(member->memberID) >= 7 && strlen(member->memberID) <= 10))
        return 0;

    // Check memberPass length
    if (!(strlen(member->memberPass) >= 7 && strlen(member->memberPass) <= 10))
        return 0;

    // Check memberName length and characters
    if (strlen(member->memberName) < 2 || strlen(member->memberName) > 50)
        return 0;

    for (int i = 0; member->memberName[i]; i++) {
        if (!isalpha(member->memberName[i]) && member->memberName[i] != ' ')
            return 0;
    }

    // Check memberPhoneNo length and digits
    if (strlen(member->memberPhoneNo) < 10 || strlen(member->memberPhoneNo) > 11)
        return 0;

    for (int i = 0; member->memberPhoneNo[i]; i++) {
        if (!isdigit(member->memberPhoneNo[i]))
            return 0;
    }

    //Check join date
    if (!(strlen(member->memberJoinDate) == 10)) {
        return 0;
    }
    else {
        char tempDay[3] = "", tempMonth[3] = "", tempYear[5] = "";
        sscanf(member->memberJoinDate, "%s %s %s", tempDay, tempMonth, tempYear);
        //check length of date
        if (!((strlen(tempYear) == 4) && (strlen(tempMonth) == 2) && (strlen(tempDay) == 2))) {
            return 0;
        }
        //check date format
        for (int i = 0; tempYear[i]; i++) {
            if (!isdigit(tempYear[i]))
                return 0;
        }
        for (int i = 0; tempMonth[i]; i++) {
            if (!isdigit(tempMonth[i]))
                return 0;
        }
        //check validity of month
        if (!(((atoi(tempMonth)) >= 1) && ((atoi(tempMonth)) <= 12))) {
            return 0;
        }
        int dayOfMonth[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
        for (int i = 0; tempDay[i]; i++) {
            if (!isdigit(tempDay[i]))
                return 0;
        }
        dayOfMonth[1] = (atoi(tempYear) % 4 + 28);
        //check validity of day based on month
        if (!((atoi(tempDay) <= dayOfMonth[atoi(tempMonth) - 1]) && (atoi(tempDay) > 0))) {
            return 0;
        }

    }

    //check member rewards length and format
    for (int i = 0; i < strlen(member->memberRewards); i++) {

        if (!(isdigit(member->memberRewards[i]))) {
            return 0;
        };
    }
    //check topup amount length and format

    int dotCount = 0;
    for (int i = 0; i < strlen(member->memberRewards[i]); i++) {
        if (!isdigit(member->memberRewards[i])) {
            if (member->memberRewards[i] == '.') {
                dotCount++;
                // Check only one dot is present
                if (dotCount > 1)
                    return 0;
            }
            else {
                return 0;
            }
        }
    }

    return 1;

}
/*for admin use*/ void AddMemberViaTxtFileProcess(const char* fileName) {
    FILE* fptr = fopen(fileName, "r");
    if (fptr == NULL) {
        printf("\nError opening file.\n");
        exit(-1);
    }

    FILE* fptr2 = fopen("member-details.bin", "ab");
    if (fptr2 == NULL) {
        printf("\nError opening file.\n");
        fclose(fptr);
        exit(-1);
    }
    int numOfMembers = MI_getNumberOfMembers();
    Member* allMembers = MI_getMemberDetails(numOfMembers);

    ValidateTxt validate;
    Member newMembers;
    while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
        &validate.memberID, &validate.memberPass, &validate.memberName, &validate.memberPhoneNo,
        &validate.memberJoinDate, &validate.memberRewards, &validate.memberWallet) == 7) {
        // Validation
        if (NewMemberIsValidated(&validate)) {
            // If all fields are valid
            // Pass value into Member* struct
            char tempDay[3] = "", tempMonth[3] = "", tempYear[5] = "";
            sscanf(validate.memberJoinDate, "%s %s %s", tempDay, tempMonth, tempYear);
            strcpy(newMembers.memberID, validate.memberID);
            strcpy(newMembers.memberPass, validate.memberPass);
            strcpy(newMembers.memberName, validate.memberName);
            strcpy(newMembers.memberPhoneNo, validate.memberPhoneNo);
            newMembers.memberJoinDate.day = atoi(tempDay);
            newMembers.memberJoinDate.month = atoi(tempMonth);
            newMembers.memberJoinDate.year = atoi(tempYear);
            newMembers.memberRewards = atoi(validate.memberRewards);
            newMembers.memberWallet = strtod(validate.memberWallet, NULL);

            //write validated data into binary

            int verify = MI_verifyRegister(newMembers.memberID);
            if (verify == 1) {
                printf("Member with member ID \"%s\" already existed.\n\n", newMembers.memberID);
            }
            else {
                fwrite(&newMembers, sizeof(Member), 1, fptr2);
                printf("Member %s has been validated registered successfully!\n\n", newMembers.memberName);
            }

        }
        else {
            printf("Validation failed for a member.\n");
        }
    }

    fclose(fptr);
    fclose(fptr2);
}

