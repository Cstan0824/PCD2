#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#pragma warning (disable: 4996)
#define ISERROR -1
typedef char* string;
typedef struct {
	int day;
	int month;
	int year;
}Date;

typedef struct {
	int hour;
	int min;
}Time;

typedef struct {
	char staffID[11], name[20];
	char password[30];
	char position[20];
	Date joinedDate;
	double salary;
}Staff;



void lineDesign();


int SI_staffMenu();
void SI_addStaff();
void SI_searchStaff();
void SI_modifyStaff();
void SI_displayStaff();
void SI_deleteStaff();

int G_ConfirmationIsValidated(string prompt);
int G_IntIsValidated(string prompt, int range, int* output);
void G_ErrorMessage();
Date G_GetCurrentDate();
int G_CalDiffDate(Date date, Date dateToCmp);


int main() 
{
	while (SI_staffMenu());
	return 0;
}
void lineDesign() {
	for (int i = 0; i < 40; i++) {
		printf("=");
	}
	printf("\n");
}


int SI_staffMenu() {
	int decision;
	int inputIsError = 0;
	do {
		if (inputIsError == -1) return 0;
		if (inputIsError == 1) G_ErrorMessage();
		printf("\n\t< Staff Information >\n");
		lineDesign();
		printf("  1. Add\n");
		printf("  2. Search\n");
		printf("  3. Modify\n");
		printf("  4. Display\n");
		printf("  5. Delete\n");
		lineDesign();
	} while (G_IntIsValidated("Select The Memu", 5, &decision));
	system("cls");
	switch (decision) {
	case 1:
		SI_addStaff();
		break;
	case 2:
		SI_searchStaff();
		break;
	case 3:
		SI_modifyStaff();
		break;
	case 4:
		SI_displayStaff();
		break;
	case 5:
		SI_deleteStaff();
		break;
	}
	return 1;

}
void SI_addStaff() {
	Staff staffList[20] = { 0 };
	int dayOfMonth[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
	Date currDate = G_GetCurrentDate();
	int diffDate = 0;
	FILE* fptr = fopen("staffs.txt", "r");
	if (!fptr) {
		printf("  Unable to open file.\n");
		exit(ISERROR);
	}
	for (int i = 0; fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%f\n",
		&staffList[i].staffID, &staffList[i].name, &staffList[i].password, &staffList[i].position,
		&staffList[i].joinedDate.day, &staffList[i].joinedDate.month, &staffList[i].joinedDate.year,
		&staffList[i].salary) != EOF; i++);
	fclose(fptr);
	int inputIsError = 0;

	Staff staff = { 0 };
	printf("\n \t< Add Staff >\n");
	lineDesign();

	//Staff ID
	do
	{
		inputIsError = 0;
		printf("Staff ID (Enter \'0000\' to exit):");
		scanf("%[^\n]", &staff.staffID);
		rewind(stdin);
		if (strncmp(staff.staffID, "0000", 11)) {
			return;
		}
		for (int i = 0; i < 20; i++)
		{
			if (strncmp(staff.staffID, staffList[i].staffID, 11) == 0) {
				inputIsError = 1;
				printf("ID repeated please try another ID.");
				break;
			}
		}
	} while (inputIsError);
	

	//Name
	printf("Name (Enter \'0000\' to exit):");
	scanf("%[^\n]", &staff.name);
	rewind(stdin);
	if (strncmp(staff.name, "0000", 11)) {
		return;
	}

	//Password
	printf("Password (Enter \'0000\' to exit):");
	scanf("%[^\n]", &staff.password);
	rewind(stdin);
	if (strncmp(staff.password, "0000", 11)) {
		return;
	}
	
	//Position
	printf("Position (Enter \'0000\' to exit):");
	scanf("%[^\n]", &staff.position);
	rewind(stdin);
	if (strncmp(staff.position, "0000", 11)) {
		return;
	}

	//Joined date
	do
	{
		if (diffDate < 0) printf("Joined Date can\'t be before current date.\n");

		//Joined Date (Year)
		while (inputIsError =
			G_IntIsValidated("Enter the Joined Date(DD/MM/\033[0;34mYYYY\033[0;37m)", currDate.year, &staff.joinedDate.year)) {
			if (inputIsError == -1) {
				return;
			}
			if (inputIsError == 1)  G_ErrorMessage();
		}

		//Joined Date(Month)
		while (inputIsError =
			G_IntIsValidated("Enter the Joined Date(DD/\033[0;34mMM\033[0;37m/YYYY)", 12, &staff.joinedDate.month)) {
			if (inputIsError == -1) {
				return;
			}
			if (inputIsError == 1)  G_ErrorMessage();
		}

		//Calculate the day of febuary
		dayOfMonth[1] = staff.joinedDate.year % 4 + 28;

		//Joined Date(Day)
		while (inputIsError =
			G_IntIsValidated("Enter the Joined Date(\033[0;34mDD\033[0;37m/MM/YYYY)", dayOfMonth[staff.joinedDate.month - 1], &staff.joinedDate.day)) {
			if (inputIsError == -1) {
				return;
			}
			if (inputIsError == 1)  G_ErrorMessage();
		}

		diffDate = G_CalDiffDate(currDate, staff.joinedDate);
	} while (diffDate < 0);
	diffDate = 0;

	//vadition get from member Information
	printf("  Monthly salary: ");
	scanf("%lf", &staff.salary);
	rewind(stdin);
	printf("\n");

	FILE* fptr = fopen("staffs.txt", "a");
	if (!fptr) {
		printf("Unable to open file.\n");
		exit(ISERROR);
	}
	fprintf(fptr, "%s|%s|%s|%s|%d/%d/%d|%.2f\n",
		staff.staffID, staff.name, staff.password, staff.position, 
		staff.joinedDate.day, staff.joinedDate.month, staff.joinedDate.year, 
		staff.salary);
	fclose(fptr);
}

void SI_searchStaff() {
	Staff staff[20] = { 0 };
	char searchStaff[11] = "";
	int r = 0;
	FILE* fptr = fopen("staffs.txt", "r");
	if (!fptr) {
		printf("  Unable to open file.\n");
		exit(ISERROR);
	}
	for (r = 0; fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%lf\n",
		staff[r].staffID, staff[r].name, &staff[r].password, staff[r].position,
		&staff[r].joinedDate.day, &staff[r].joinedDate.month, &staff[r].joinedDate.year,
		&staff[r].salary) != EOF; r++);
	
	fclose(fptr);

	printf("\n  Number of records: %d\n", r);
	printf("  Existing ID: ");

	for (int i = 0; i < r; i++) {
		printf(" %-11s ", staff[i].staffID);
	}
	printf("\n");
	printf("\n\t< Search Staff >\n");
	lineDesign();
	printf("  Enter staff ID (Enter \'0000\' to exit):");
	scanf("%[^\n]", &searchStaff);
	rewind(stdin);
	if (strncmp(searchStaff, "0000", 11)) return;

	for (int i = 0; i < r; i++) 
	{
		if (strstr(staff[i].staffID, searchStaff))
		{
			printf("  Staff ID	 : %s\n", staff[i].staffID);
			printf("  Name		 : %s\n", staff[i].name);
			printf("  Password	 : %s\n", staff[i].password);
			printf("  Position	 : %s\n", staff[i].position);
			printf("  Joined date(NO/ DDMMYY)    : %d/%d/%d\n", staff[i].joinedDate.day, staff[i].joinedDate.month, staff[i].joinedDate.year);
			printf("  Salary	 : RM%.2f\n\n", staff[i].salary);
			break;
		}
	}
}

void SI_modifyStaff() {
	Staff staff[20] = { 0 };
	char searchStaff[11] = "";
	int decision, i = 0, r;
	int inputIsError = 0;

	int dayOfMonth[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
	Date currDate = G_GetCurrentDate();
	int diffDate = 0;

	FILE* fptr = fopen("staffs.txt", "r");
	if (!fptr) {
		printf("  Unable to open file.\n");
		exit(ISERROR);
	}
	for (r = 0; fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%f\n",
		&staff[r].staffID, &staff[r].name, &staff[r].password, &staff[r].position,
		&staff[r].joinedDate.day, &staff[r].joinedDate.month, &staff[r].joinedDate.year,
		&staff[r].salary) != EOF; r++);
	fclose(fptr);

	printf("\n  Number of records: %d\n", r);
	printf("  Existing ID: ");
	for (i = 0; i < r; i++) 
	{
		printf(" %s ", staff[i].staffID);
	}
	printf("\n");
	printf("\n\t< Modify Staff >\n");
	lineDesign();
	printf("  Enter staff ID (Enter \'0000\' to exit):");
	scanf("%[^\n]", &searchStaff);
	rewind(stdin);
	if(strncmp(searchStaff, "0000", 11)) return;

	for (i = 0; i < r; i++) {
		if (strcmp(searchStaff, staff[i].staffID) == 0) {
			do {
				printf("\n\t< Modification - Staff >\n");
				lineDesign();
				printf("  1. Staff ID\n");
				printf("  2. Name\n");
				printf("  3. Password\n");
				printf("  4. Position\n");
				printf("  5. Joined date(NO/ DDMMYY)\n");
				printf("  6. Salary\n");
				lineDesign();
			} while (inputIsError =
				G_IntIsValidated("Select the attribute to edit", 6, &decision));
			switch (decision) {
			case 1:
				printf("  Modifying - Staff ID: ");
				scanf("%[^\n]", &staff[i].staffID);
				break;
			case 2:
				printf("  Modifying - Name: ");
				scanf("%[^\n]", &staff[i].name);
				break;
			case 3:
				printf("  Modifying - Password: ");
				scanf("%[^\n]", &staff[i].password);
				break;
			case 4:
				printf("  Modifying - Position: ");
				scanf("%[^\n]", &staff[i].position);
				break;
			case 5:
				do
				{
					if (diffDate < 0) printf("Joined Date can\'t be before current date.\n");
					//Joined Date (Year)
					while (inputIsError =
						G_IntIsValidated("Enter the Joined Date(DD/MM/\033[0;34mYYYY\033[0;37m)", currDate.year, &staff[i].joinedDate.year)) {
						if (inputIsError == 1)  G_ErrorMessage();
					}

					//Joined Date(Month)
					while (inputIsError =
						G_IntIsValidated("Enter the Joined Date(DD/\033[0;34mMM\033[0;37m/YYYY)", 12, &staff[i].joinedDate.month)) {
						if (inputIsError == -1) {
							return;
						}
						if (inputIsError == 1)  G_ErrorMessage();
					}

					//Calculate the day of febuary
					dayOfMonth[1] = staff[i].joinedDate.year % 4 + 28;

					//Joined Date(Day)
					while (inputIsError =
						G_IntIsValidated("Enter the Joined Date(\033[0;34mDD\033[0;37m/MM/YYYY)", dayOfMonth[staff[i].joinedDate.month - 1], &staff[i].joinedDate.day)) {
						if (inputIsError == -1) {
							return;
						}
						if (inputIsError == 1)  G_ErrorMessage();
					}

					diffDate = G_CalDiffDate(currDate, staff[i].joinedDate);
				} while (diffDate < 0);
				diffDate = 0;
				break;
			case 6:
				printf("  Modifying - Salary: ");
				scanf("%lf", &staff[i].salary);
				break;
			}
			rewind(stdin);
			printf("  Updated.\n");
			printf("  Staff ID	 : %s\n", staff[i].staffID);
			printf("  Name		 : %s\n", staff[i].name);
			printf("  Password	 : %s\n", staff[i].password);
			printf("  Position	 : %s\n", staff[i].position);
			printf("  Joined date(NO/ DDMMYY)    : %d/%d/%d\n", staff[i].joinedDate.day, staff[i].joinedDate.month, staff[i].joinedDate.year);
			printf("  Salary	 : RM%.2f\n\n", staff[i].salary);
			break;
		}
	}

	fptr = fopen("staffs.txt", "w");
	if (!fptr) {
		printf("  Unable to open file\n");
		exit(-1);
	}
	for (i = 0; i < r; i++) {
		fptrrintf(fptr, "%s|%s|%s|%s|%d/%d/%d|%.2f\n", staff[i].staffID, staff[i].name, staff[i].password, staff[i].position, staff[i].joinedDate.day, staff[i].joinedDate.month, staff[i].joinedDate.year, staff[i].salary);
	}
	fclose(fptr);
}

void SI_displayStaff() {
	Staff staff = { 0 };
	char searchStaff[10] = "";
	int r;
	FILE* fptr = fopen("staffs.txt", "r");
	if (!fptr) {
		printf("  Unable to open file.\n");
		exit(ISERROR);
	}

	for (r = 0; fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%f\n",
		&staff.staffID, &staff.name, &staff.password, &staff.position,
		&staff.joinedDate.day, &staff.joinedDate.month, &staff.joinedDate.year,
		&staff.salary) != EOF; r++) 
	{
		printf("  Staff ID	 : %s\n", staff.staffID);
		printf("  Name		 : %s\n", staff.name);
		printf("  Password	 : %s\n", staff.password);
		printf("  Position	 : %s\n", staff.position);
		printf("  Joined date(DD/MM/YYYY)   : %d/%d/%d\n", staff.joinedDate.day, staff.joinedDate.month, staff.joinedDate.year);
		printf("  Salary	 : RM%.2f\n\n", staff.salary);
	}
	fclose(fptr);
	printf("\n  Number of records: %d\n", r);
}

void SI_deleteStaff() {
	char staffDel[10] = "";
	Staff staff[20];
	int i = 0;

	FILE* fptr = fopen("staffs.txt", "r");
	if (!fptr)
	{
		printf("Cant open the file\n");
		exit(ISERROR);
	}


	for (i = 0;
		fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%f\n",
			staff[i].staffID, &staff[i].name, &staff[i].password, &staff[i].position,
			&staff[i].joinedDate.day, &staff[i].joinedDate.month, &staff[i].joinedDate.year,
			&staff[i].salary) != EOF; i++);
	fclose(fptr);

	printf("  + - - - - - - - - - - - - - - - - +\n");
	printf("  | Staff Existing ID:		    |\n");
	for (int j = 0; j < i; j++)
	{
		printf("  | %s		            |\n", staff[i].staffID);
	}
	printf("  + - - - - - - - - - - - - - - - - +\n\n");



	printf("Which Data do you wish to delete ? : ");
	scanf(" %[^\n]", &staffDel);
	rewind(stdin);
	int currentRe = i;
	
	//two for loop
	for (i = 0; i < currentRe; i++)
	{
		if (strcmp(staffDel, staff[i].staffID) == 0)
		{
			currentRe--;
			for (int j = i; j < currentRe; j++)
			{
				//if last index, currentRe-- [not store it to txt file when write]
				if ((j + 1) == currentRe)
				{
					continue;
				}
				strcpy(staff[j].staffID, staff[j + 1].staffID);
				strcpy(staff[j].name, staff[j + 1].name);
				strcpy(staff[j].password, staff[j + 1].password);
				strcpy(staff[j].position, staff[j + 1].position);
				staff[j].joinedDate.day = staff[j + 1].joinedDate.day;
				staff[j].joinedDate.month = staff[j + 1].joinedDate.month;
				staff[j].joinedDate.year = staff[j + 1].joinedDate.year;
				staff[j].salary = staff[j + 1].salary;
				break;
			}
			printf("\n Staff Data deleted successfully.\n");
		}
	}

	printf("  + - - - - - - - - - - - - - - - - +\n");
	printf("  | Existing Staff ID After Delete:       |\n");
	for (i = 0; i < currentRe; i++)
	{
		printf("  | %s		            |\n", staff[i].staffID);
	}
	printf("  + - - - - - - - - - - - - - - - - +\n");

	fptr = fopen("staffs.txt", "w");
	if (!fptr) {
		printf("  Unable to open file.\n");
		exit(ISERROR);
	}
	for (i = 0; i < currentRe; i++)
	{
		fprintf(fptr, "%s|%s|%s|%s|%d/%d/%d|%.2f\n", staff[i].staffID, staff[i].name, staff[i].password, staff[i].position, 
			staff[i].joinedDate.day, staff[i].joinedDate.month, staff[i].joinedDate.year,
			staff[i].salary);
	}
	fclose(fptr);
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
	printf("%s  [ENTER '0' to escape] >>", prompt);
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
	return	(date.year * 365 + date.month * 30 + date.day)
		-
		(dateToCmp.year * 365 + dateToCmp.month * 30 + dateToCmp.day);
}
