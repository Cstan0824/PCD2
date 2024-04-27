#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#pragma warning (disable: 4996)

struct employDate {
	int days, months, years;
};

struct staffInfo {
	char staffID[10], name[20];
	char password[30];
	char position[20];
	struct employDate joinedDate;
	float salary;
};



void lineDesign();


void staffInfo();
void addStaff();
void searchStaff();
void modifyStaff();
void displayStaff();
void deleteStaff();






void main() {


	struct employDate joinedDate;
	struct staffInfo staff;
	staffInfo();
	addStaff();
	searchStaff();
	modifyStaff();
	displayStaff();
	deleteStaff();


	system("pause");
}
void lineDesign() {
	for (int i = 0; i < 40; i++) {
		printf("=");
	}
	printf("\n");
}


void staffInfo() {
	int sCode;
	do {
		printf("\n\t< Staff Information >\n");
		lineDesign();
		printf("  1. Add\n");
		printf("  2. Search\n");
		printf("  3. Modify\n");
		printf("  4. Display\n");
		printf("  5. Delete\n");
		printf("  6. Return to Menu\n");
		lineDesign();
		printf("  Enter code: ");
		scanf("%d", &sCode);
		rewind(stdin);
		system("cls");
		switch (sCode) {
		case 1:
			addStaff();
			break;
		case 2:
			searchStaff();
			break;
		case 3:
			modifyStaff();
			break;
		case 4:
			displayStaff();
			break;
		case 5:
			deleteStaff();
			break;
		default:
			printf("  Invalid code.\n");
		}
	} while (sCode != 1 || sCode != 2 || sCode != 3 || sCode != 4 || sCode != 5 || sCode != 6);
}
void addStaff() {
	struct staffInfo staff;
	printf("\n \t< Add Staff >\n");
	lineDesign();
	printf("  Enter information below >\n");
	printf("  Staff ID: ");
	scanf("%[^\n]", staff.staffID);
	rewind(stdin);
	printf("  Name: ");
	scanf("%[^\n]", staff.name);
	rewind(stdin);
	printf("  Password: ");
	scanf("%[^\n]", &staff.password);
	rewind(stdin);
	printf("  Position: ");
	scanf("%[^\n]", staff.position);
	rewind(stdin);
	printf("  Joined date(NO/ DDMMYY): ");
	scanf("%d %d %d", &staff.joinedDate.days, &staff.joinedDate.months, &staff.joinedDate.years);
	rewind(stdin);
	printf("  Monthly salary: ");
	scanf("%f", &staff.salary);
	rewind(stdin);
	printf("\n");
	FILE* fp = fopen("staffs.txt", "a");
	if (!fp) {
		printf("  Unable to open file.\n");
		exit(-1);
	}
	fprintf(fp, "%s|%s|%s|%s|%d/%d/%d|%.2f\n", staff.staffID, staff.name, staff.password, staff.position, staff.joinedDate.days, staff.joinedDate.months, staff.joinedDate.years, staff.salary);
	fclose(fp);
}
void searchStaff() {
	struct staffInfo staff[20];
	char searchStaff[10];
	int i = 0, r = 0;
	FILE* fp = fopen("staffs.txt", "r");
	if (!fp) {
		printf("  Unable to open file.\n");
		exit(-1);
	}
	while (fscanf(fp, "%[^|]|%[^|]|[^|]|%[^|]|%d/%d/%d|%f\n", staff[r].staffID, staff[r].name, &staff[r].password, staff[r].position, &staff[r].joinedDate.days, &staff[r].joinedDate.months, &staff[r].joinedDate.years, &staff[r].salary) != EOF) {
		r++;
	}
	fclose(fp);
	printf("\n  Number of records: %d\n", r);
	printf("  Existing ID: ");

	for (i = 0; i < r; i++) {
		printf(" %s ", staff[i].staffID);
	}
	printf("\n");
	printf("\n\t< Search Staff >\n");
	lineDesign();
	printf("  Enter staff ID: ");
	scanf("%[^\n]", searchStaff);
	rewind(stdin);

	for (i = 0; i < r; i++) {
		if (strcmp(searchStaff, staff[i].staffID) == 0) {
			printf("  Staff ID	 : %s\n", staff[i].staffID);
			printf("  Name		 : %s\n", staff[i].name);
			printf("  Password	 : %s\n", staff[i].password);
			printf("  Position	 : %s\n", staff[i].position);
			printf("  Joined date(NO/ DDMMYY)    : %d/%d/%d\n", staff[i].joinedDate.days, staff[i].joinedDate.months, staff[i].joinedDate.years);
			printf("  Salary	 : RM%.2f\n\n", staff[i].salary);
			break;
		}
	}

}
void modifyStaff() {
	struct staffInfo staff[10];
	char searchStaff[10];
	int mCode, i = 0, r = 0;

	FILE* fp = fopen("staffs.txt", "r");
	if (!fp) {
		printf("  Unable to open file.\n");
		exit(-1);
	}
	while (fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%f\n", staff[r].staffID, staff[r].name, &staff[r].password, staff[r].position, &staff[r].joinedDate.days, &staff[r].joinedDate.months, &staff[r].joinedDate.years, &staff[r].salary) != EOF) {
		r++;
	}
	fclose(fp);
	printf("\n  Number of records: %d\n", r);
	printf("  Existing ID: ");
	for (i = 0; i < r; i++) {
		printf(" %s ", staff[i].staffID);
	}
	printf("\n");
	printf("\n\t< Modify Staff >\n");
	lineDesign();
	printf("  Enter staff ID: ");
	scanf("%[^\n]", searchStaff);
	rewind(stdin);

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
				printf("  7. Return to Staff Menu\n");
				lineDesign();
				printf("  Enter code: ");
				scanf("%d", &mCode);
				rewind(stdin);
				if (mCode == 1) {
					printf("  Modifying - Staff ID: ");
					scanf("%[^\n]", staff[i].staffID);
					rewind(stdin);
					printf("  Updated.\n");
					break;
				}
				else if (mCode == 2) {
					printf("  Modifying - Name: ");
					scanf("%[^\n]", staff[i].name);
					rewind(stdin);
					printf("  Updated.\n");
					break;
				}
				else if (mCode == 3) {
					printf("  Modifying - Password: ");
					scanf("%[^\n]", &staff[i].password);
					rewind(stdin);
					printf("Updated.\n");
					break;
				}
				else if (mCode == 4) {
					printf("  Modifying - Position: ");
					scanf("%[^\n]", staff[i].position);
					rewind(stdin);
					printf("  Updated.\n");
					break;
				}
				else if (mCode == 5) {
					printf("  Modifying - Joined date(NO/ DDMMYY): ");
					scanf("%d %d %d", &staff[i].joinedDate.days, &staff[i].joinedDate.months, &staff[i].joinedDate.years);
					rewind(stdin);
					printf("  Updated.\n");
					break;
				}
				else if (mCode == 6) {
					printf("  Modifying - Salary: ");
					scanf("%f", &staff[i].salary);
					rewind(stdin);
					printf("  Updated.\n");
					break;
				}
				else if (mCode == 7) {
					staffInfo();
					break;
				}
				else {
					printf("  Invalid code\n");
				}
			} while (mCode != 1 || mCode != 2 || mCode != 3 || mCode != 4 || mCode != 5 || mCode != 6 || mCode != 7);
		}
	}

	fp = fopen("staffs.txt", "w");
	if (!fp) {
		printf("  Unable to open file\n");
		exit(-1);
	}
	for (i = 0; i < r; i++) {
		fprintf(fp, "%s|%s|%s|%s|%d/%d/%d|%.2f\n", staff[i].staffID, staff[i].name, staff[i].password, staff[i].position, staff[i].joinedDate.days, staff[i].joinedDate.months, staff[i].joinedDate.years, staff[i].salary);
	}
	fclose(fp);
}
void displayStaff() {
	struct staffInfo staff[20];
	char searchStaff[10];
	int i = 0, r = 0;
	FILE* fp = fopen("staffs.txt", "r");
	if (!fp) {
		printf("  Unable to open file.\n");
		exit(-1);
	}

	while (fscanf(fp, "%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%f\n", staff[r].staffID, staff[r].name, &staff[r].password, staff[r].position, &staff[r].joinedDate.days, &staff[r].joinedDate.months, &staff[r].joinedDate.years, &staff[r].salary) != EOF) {
		r++;
	}
	fclose(fp);
	printf("\n  Number of records: %d\n", r);
	printf("  Existing ID: ");
	for (i = 0; i < r; i++) {
		printf(" %s ", staff[i].staffID);
	}
	printf("\n");
	printf("\n\t< Display Staff >\n");
	lineDesign();
	printf("  Enter staff ID: ");
	scanf("%[^\n]", searchStaff);
	rewind(stdin);

	for (i = 0; i < r; i++) {
		if (strcmp(searchStaff, staff[i].staffID) == 0) {
			printf("  Staff ID	 : %s\n", staff[i].staffID);
			printf("  Name		 : %s\n", staff[i].name);
			printf("  Password	 : %s\n", staff[i].password);
			printf("  Position	 : %s\n", staff[i].position);
			printf("  Joined date(NO/ DDMMYY)   : %d/%d/%d\n", staff[i].joinedDate.days, staff[i].joinedDate.months, staff[i].joinedDate.years);
			printf("  Salary	 : RM%.2f\n\n", staff[i].salary);
			break;
		}
	}
}
void deleteStaff() {
	FILE* fp5;
	fp5 = fopen("staffs.txt", "r");
	if (fp5 == NULL)
	{
		printf("Cant open the file\n");
		exit(-2);
	}

	struct staffInfo staff[20];
	int i = 0, currentRe = 0;

	while (fscanf(fp5, "%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%f\n", staff[i].staffID, staff[i].name, &staff[i].password, staff[i].position, &staff[i].joinedDate.days, &staff[i].joinedDate.months, &staff[i].joinedDate.years, &staff[i].salary) != EOF) {
		i++;
		currentRe++;
	}
	fclose(fp5);

	printf("  + - - - - - - - - - - - - - - - - +\n");
	printf("  | Staff Existing ID:		    |\n");
	for (i = 0; i < currentRe; i++)
	{
		printf("  | %s		            |\n", staff[i].staffID);
	}
	printf("  + - - - - - - - - - - - - - - - - +\n");



	char staffDel[10];
	printf("Which Data do you wish to delete ? : ");
	scanf(" %[^\n]", &staffDel);
	rewind(stdin);

	int j;
	//two for loop
	for (i = 0; i < currentRe; i++)
	{
		if (strcmp(staffDel, staff[i].staffID) == 0)
		{
			currentRe--;
			for (j = i; j < currentRe; j++)
			{
				strcpy(staff[j].staffID, staff[j + 1].staffID);
				strcpy(staff[j].name, staff[j + 1].name);
				strcpy(staff[j].password, staff[j + 1].password);
				strcpy(staff[j].position, staff[j + 1].position);
				staff[j].joinedDate.days = staff[j + 1].joinedDate.days;
				staff[j].joinedDate.months = staff[j + 1].joinedDate.months;
				staff[j].joinedDate.years = staff[j + 1].joinedDate.years;
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

	fp5 = fopen("staffs.txt", "w");
	if (!fp5) {
		printf("  Unable to open file.\n");
		exit(-1);
	}
	for (i = 0; i < currentRe; i++)
	{
		fprintf(fp5, "%s|%s|%s|%s|%d/%d/%d|%.2f\n", staff[i].staffID, staff[i].name, staff[i].password, staff[i].position, staff[i].joinedDate.days, staff[i].joinedDate.months, staff[i].joinedDate.years, staff[i].salary);
	}
	fclose(fp5);

}