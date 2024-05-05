#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <Windows.h>
#define ISERROR -1
#pragma warning(disable:4996)
//jeremycjc
//chin123lol
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
	char trainID[10]; //user input
	char seatID[10]; //user input
	int seatType; // 50% Standard[1] : 20% Premium[2] : 20% Sleeping Berths[3] : 10% Special Needs Seats[4](For Child/Seniors)
	char coach; //
	double price; //auto calculate
}Ticket;

typedef struct {
	char bookingID[10];
	char memberID[11];
	Date bookingDate;
	Ticket ticketDetails;
	int isCancelled;
}Booking;

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

typedef struct {
	double discount;
	int pointRequired;
	const string desc;
	int minRequired;
	int maxLimits;
}Package;

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
	char staffID[11], name[20];
	char password[30];
	char position[20];
	Date joinedDate;
	double salary;
}Staff;


int isAdmin = 0;
Member loggedInMember = { 0 };

// Function Declarations

//Global use
Date G_GetCurrentDate();
Time G_GetCurrentTime();
int G_CalDiffDate(Date date, Date dateToCmp);
int G_GetBinFileNumRow(string fileName, unsigned int dataSize);
int G_InArray(void* arr, void* value, int elementSize, int length, int compare(void* a, void* b));
int G_CompareInt(void* a, void* b);
int G_ConfirmationIsValidated(string prompt);
int G_IntIsValidated(string prompt, int range, int* output);
void G_ErrorMessage();
int G_GetTxtFileNumRow(string fileName);
Time G_GetTime(string prompt);
int G_CalDiffTime(Time time, Time timeToCmp);
void G_shiftSpaceForDrawTrain(int G_position);
void G_DrawTrain();

//Menu
int G_systemMenu();
int TB_mainMenu();
int TS_mainMenu();
int SI_mainMenu();
int MI_mainMenu();

//Booking Ticket
//Read
void TB_DisplayBookingRecord(string searchFilter);
void TB_DisplayBookingRecordHdr(Booking booking, double* totalPrice, TrainSchedule trainSchedule);
void TB_DisplayBookingRecordDet(Booking booking, double* totalPrice, int count);
void TB_DisplayBookingRecordFoot(double totalPrice);
string TB_DisplaySeatType(int seatType);
//Create
void TB_BookingTicket(TrainSchedule trainSchedule);
Booking* TB_GetBookingDetails(int* length);
Ticket TB_GetTicketDetails(TrainSchedule trainSchedule, int* notAvailableSeat, int lengthOfNotAvailableSeat);
void TB_DisplaySeat(int seatQuantity, int** notAvailableSeat, int lengthOfNotAvailableSeat);
int* TB_GetSeatAvailability(int* length, string trainID);
void TB_GetSeatDetails(TrainSchedule trainSchedule, Ticket* ticket, int** notAvailableSeat, int lengthOfNotAvailableSeat);
int TB_GetSeatType(int selectedSeatID, int numOfSeat[], int length);
double TB_GetTicketPrice(int seatType, Time startTime, Date departureDate);
void TB_GetPackageDetails(Booking* booking, int length);
int TB_CheckAccountBalance(double priceToPaid);
//Edit
void TB_EditBooking(TrainSchedule trainSchedule);
int* TB_GetindexOfMemberBookingID(Booking* booking, int numRow, int* countMemberData, string trainID);
//Delete
void TB_CancelBooking(TrainSchedule trainSchedule);

//Train Schedule
TrainSchedule* TS_GetTrainSchedule(string trainID);
void TS_addSchedule();
void TS_searchSchedule();
void TS_modifySchedule();
void TS_deleteSchedule();
void TS_displaySchedule();

//Staff Information
void SI_addStaff();
void SI_searchStaff();
void SI_modifyStaff();
void SI_displayStaff();
void SI_deleteStaff();

//Get info from binary file
int MI_getNumberOfMembers();
Member* MI_getMemberDetails(int rows);
void MI_loggedInMemberDetails(string memberID);

//Member Login/Register
string MI_getPassword();
int MI_verifyRegister(string memberID);
void MI_registerMember();
void MI_memberLogin();
int MI_verifyLogin(string inputID, string inputPass);

//Member functions
void MI_displayDetails(string memberID);
void MI_displayDetailsProcess(Member* allMembers, int rows, string memberID);
void MI_topUpWallet(string memberID);
void MI_MemberEditDetails(string memberID);
void MI_memberDeleteAccount(string memberID);

//Validation
int MI_InputDetailsValidation(string value, string mode);
void MI_ErrorMessageForInputLength(int min, int max);

//Admin Functions
/*for staff use*/ void MI_displayAllMembers();
/*for staff use*/ void MI_displayMembersHeaderOrFooter(char HeaderOrFooter);
/*for staff use*/ void MI_staffSearchMember();
/*for staff use*/ void MI_displaySearchedMembers(string memberID);
/*for staff use*/ void MI_staffDeleteMember();
/*for staff use*/ void MI_staffEditMemberDetails();
/*for staff use*/ void MI_staffAddMember();
/*for staff use*/ void MI_staffAddMemberViaTxt();
/*for staff use*/ int MI_NewMemberIsValidated(const ValidateTxt* member);
/*for staff use*/ void MI_AddMemberViaTxtFileProcess(const char* fileName);
/*for staff use*/ void MI_ConvertBinToTxt();

//Function 
int main() {

	while (G_systemMenu());
	return 0;
}

Date G_GetCurrentDate() {
	//printf("Audit Log : Date G_GetCurrentDate()\n");
	SYSTEMTIME getDate;
	GetLocalTime(&getDate);
	Date date = { getDate.wDay, getDate.wMonth, getDate.wYear };
	return date;
}
int G_CalDiffDate(Date date, Date dateToCmp) {
	return
		(date.year * 365 + date.month * 30 + date.day)
		-
		(dateToCmp.year * 365 + dateToCmp.month * 30 + dateToCmp.day);
}
Time G_GetCurrentTime() {
	//printf("Audit Log : Time G_GetCurrentTime()\n");
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);
	Time time = { localTime.wHour,localTime.wMinute };
	return time;
}
int G_CalDiffTime(Time time, Time timeToCmp) {
	return (time.hour * 60 + time.min) - (timeToCmp.hour * 60 + timeToCmp.min);
}
int G_GetBinFileNumRow(string fileName, unsigned int dataSize) {
	//printf("Audit Log : int G_GetBinFileNumRow(string fileName, unsigned int dataSize)\n");
	FILE* fptr = fopen(fileName, "rb");
	if (!fptr) {
		printf("File unable to open.");
		exit(ISERROR);
	}
	fseek(fptr, 0, SEEK_END);
	int fileSize = ftell(fptr);
	fclose(fptr);
	return fileSize / dataSize;
}
int G_InArray(void* arr, void* value, int elementSize, int length, int compare(void* a, void* b)) {
	//printf("Audit Log : int G_InArray(void* arr, void* value, int elementSize, int length, compareFunction compare)\n");
	char* tempArr = (char*)arr;
	for (int i = 0; i < length; i++)
	{
		void* pointedAddress = tempArr + i * elementSize;
		if (compare(pointedAddress, value)) {
			return 1;
		}
	}
	return 0;
}
int G_CompareInt(void* a, void* b) {
	//printf("Audit Log : int G_CompareInt(void* a, void* b)\n");
	return *(int*)a == *(int*)b;
}
int G_ConfirmationIsValidated(string prompt) {
	char confirmation[3] = "";
	int inputIsError = 0;
	do
	{
		if (inputIsError) G_ErrorMessage();

		printf("%s [Y/N]: ", prompt);
		scanf("%[^\n]", &confirmation);
		rewind(stdin);

		strcpy(confirmation, strupr(confirmation));
	} while (inputIsError = !(strncmp(confirmation, "Y", 3) == 0 || strncmp(confirmation, "N", 3) == 0));

	return strncmp(confirmation, "Y", 3) == 0;
}
int G_IntIsValidated(string prompt, int range, int* output) {
	char digit[10] = "";
	char decision[10] = "";
	printf("%s  [ENTER '0' to return]: ", prompt);
	scanf("%9[^\n]", &decision);
	rewind(stdin);

	if (strncmp(decision, "0", 10) == 0) {
		return -1;
	}
	for (int i = 1; i <= range; i++) {
		sprintf(digit, "%d", i);
		if (strncmp(decision, digit, 10) == 0) {
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
int G_GetTxtFileNumRow(string fileName)
{
	char buffer[400] = ""; // might need to increase the buffer
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
void G_lineDesign() {
	for (int i = 0; i < 40; i++) {
		printf("=");
	}
	printf("\n");
}
Time G_GetTime(string prompt) {
	Time tempTime = { 0 };
	Time returnError = { -1 };

	char digit[4] = "";
	char decision[4] = "";

	int inputIsError = 1;

	//Time (Hour)
	while (inputIsError) {
		printf("Enter %s (Hours) [ENTER \'-1\' to escape] >>", prompt);
		scanf("%[^\n]", &decision);
		rewind(stdin);
		printf("\n");
		if (strncmp(decision, "-1", 3) == 0) return returnError;

		for (int i = 0; i < 24; i++) {
			sprintf(digit, "%d", i);
			if (strncmp(decision, digit, 3) == 0) {
				tempTime.hour = i;
				inputIsError = 0;
				break;
			}
		}
		if (inputIsError) G_ErrorMessage();
	}

	inputIsError = 1;
	//Time (Minute)
	while (inputIsError) {
		printf("Enter %s (Minutes) [ENTER \'-1\' to escape] >>", prompt);
		scanf("%[^\n]", &decision);
		rewind(stdin);
		printf("\n");
		if (strncmp(decision, "-1", 3) == 0)  return returnError;

		for (int i = 0; i < 60; i++) {
			sprintf(digit, "%d", i);
			if (strncmp(decision, digit, 3) == 0) {
				tempTime.min = i;
				inputIsError = 0;
				break;
			}
		}
		if (inputIsError) G_ErrorMessage();
	}
	return tempTime;
}
void G_DrawTrain() {
	int G_position = 0;
	int G_maxPosition = 30;
	for (int i = 0; i < G_maxPosition; i++) {
		system("cls");
		G_shiftSpaceForDrawTrain(G_position);
		printf("      _____________   _____________   _____________   _______________________________________^^_\n");
		G_shiftSpaceForDrawTrain(G_position);
		printf("     |  ___   ___  |||  ___   ___  |||  ___   ___  |||  ___   ___   ___    ___ ___  |   __  ,----\\\\\n");
		G_shiftSpaceForDrawTrain(G_position);
		printf("    || |   | |   | ||| |   | |   | ||| |   | |   | ||| |   | |   | |   |  |   |   | |  |  | |     \\\\\n");
		G_shiftSpaceForDrawTrain(G_position);
		printf("   ||| |   | |   | ||| |   | |   | ||| |   | |   | ||| |   | |   | |   |  |   |   | |  |  | |______\\\\\n");
		G_shiftSpaceForDrawTrain(G_position);
		printf("   ||| |___| |___| ||| |___| |___| ||| |___| |___| ||| |___| |___| |___|  | O | O | |  |  |         \\\\\n");
		G_shiftSpaceForDrawTrain(G_position);
		printf("   |||             |||             |||             |||                    |___|___| |  |__|           )\n");
		G_shiftSpaceForDrawTrain(G_position);
		printf("   |||_____________|||_____________|||_____________|||______________________________|_______________//\n");
		G_shiftSpaceForDrawTrain(G_position);
		printf("    ||             |||             |||             |||                                        /________\n");
		G_shiftSpaceForDrawTrain(G_position);
		printf("     '-------------'''-------------'''-------------'''---------------------------------------'\n");
		G_shiftSpaceForDrawTrain(G_position);
		printf("-=@==@==@=@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@=-\n");
		G_shiftSpaceForDrawTrain(G_position);
		printf("-=@==@==@=@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@==@=-\n");
		G_position = (G_position + 1) % G_maxPosition;
		Sleep(5);
	}
	printf("\n\n\n");
}
void G_shiftSpaceForDrawTrain(int G_position) {
	for (int i = 0; i < G_position; i++) {
		printf(" ");
	}
}

//Booking Ticket

//Read[Admin/Member]
void TB_DisplayBookingRecord(string searchFilter)
{
	double totalPrice = 0;

	int currTrainID = 0;
	int trainID;
	int numOfTrain = 0;
	int count = 0;

	int length = G_GetBinFileNumRow("Booking.bin", sizeof(Booking));

	Booking* booking = NULL;
	int* displayedTrainID = NULL;
	int searchAll = strncmp(searchFilter, "ALL", 3) == 0;

	FILE* fptr = fopen("Booking.bin", "rb");
	if (!fptr) {
		printf("Error when opening file.");
		exit(ISERROR);
	}

	booking = (Booking*)malloc(sizeof(Booking) * length);
	if (!booking) {
		printf("Memory allocation Failed.");
		exit(ISERROR);
	}
	fread(booking, sizeof(Booking), length, fptr);
	fclose(fptr);

	for (int i = 0; i < length; i++)
	{
		sscanf(booking[i].ticketDetails.trainID, "T%d", &trainID);
		if (G_InArray(displayedTrainID, &trainID, sizeof(int), numOfTrain, G_CompareInt)
			||
			strncmp(booking[i].memberID, loggedInMember.memberID, 11) != 0
			)
		{
			i++;
			continue;
		}
		else
		{
			int* tempDisplayedTrainID = (int*)realloc(displayedTrainID, sizeof(int) * ++numOfTrain);
			if (!tempDisplayedTrainID) {
				printf("Memory Allocation failed.");
				free(displayedTrainID);
				exit(ISERROR);
			}
			displayedTrainID = tempDisplayedTrainID;
			displayedTrainID[numOfTrain - 1] = trainID;
		}
		int search = searchAll
			?
			1
			:
			strncmp(searchFilter, booking[i].ticketDetails.trainID, 10) == 0;
		if (search)
		{
			TrainSchedule* trainSchedule = TS_GetTrainSchedule(booking[i].ticketDetails.trainID);
			if (trainSchedule == NULL) {
				return;
			}
			//Header
			TB_DisplayBookingRecordHdr(booking[i], &totalPrice, *trainSchedule);
		}
		for (int j = 0, num = 1; j < length; j++)
		{
			sscanf(booking[j].ticketDetails.trainID, "T%d", &trainID);
			if (displayedTrainID[currTrainID] != trainID || booking[j].isCancelled)
			{
				continue;
			}
			//Details
			int search = searchAll
				?
				1
				:
				strncmp(searchFilter, booking[j].ticketDetails.trainID, 10) == 0;
			if (search) {
				TB_DisplayBookingRecordDet(booking[j], &totalPrice, num++);
			}
			count++;
		}
		currTrainID++;
	}
	printf("%d of Result returned.\n", count);
	system("pause");
	free(booking);
	free(displayedTrainID);
}
void TB_DisplayBookingRecordHdr(Booking booking, double* totalPrice, TrainSchedule trainSchedule)
{
	Date bookingDate = G_GetCurrentDate();
	printf("%-15s %-20s\n%-15s %-20s~%-20s\n%-15s %02d/%02d/%04d \n%-10s [%02d:%02d~%02d:%02d]\n%-10s %02d/%02d/%04d\n",
		"TRAIN ID :",
		booking.ticketDetails.trainID,
		"STATION :",
		trainSchedule.departureStation,
		trainSchedule.arrivalStation,
		"DEPARTURE DATE :",
		trainSchedule.departureDate.day,
		trainSchedule.departureDate.month,
		trainSchedule.departureDate.year,
		"TIME : ",
		trainSchedule.departureTime.hour,
		trainSchedule.departureTime.min,
		trainSchedule.arrivalTime.hour,
		trainSchedule.arrivalTime.min,
		"BOOKING DATE :",
		bookingDate.day,
		bookingDate.month,
		bookingDate.year
	);

	printf("%-82s\n", "---------------------------------------------------------------------------------");
	printf("%-3s %-15s %-10s %-10s %-20s %-10s\n",
		"NO.", "BOOKING ID", "SEAT ID", "COACH", "SEAT TYPE", "PRICE");
	printf("%-3s %-15s %-10s %-10s %-20s %-10s\n",
		"---", "-----------", "----------", "------", "--------------", "------");
	*totalPrice = 0;
}
void TB_DisplayBookingRecordDet(Booking booking, double* totalPrice, int count)
{
	*totalPrice += booking.ticketDetails.price;
	printf("%02d. %-15s %-10s %-10c %-20s %4.2f\n",
		count,
		booking.bookingID,
		booking.ticketDetails.seatID,
		booking.ticketDetails.coach,
		TB_DisplaySeatType(booking.ticketDetails.seatType),
		booking.ticketDetails.price
	);
}
void TB_DisplayBookingRecordFoot(double totalPrice)
{
	//printf("Audit Log : void TB_DisplayBookingRecordFoot(int isChangeTrainID, double totalPrice)\n");

	printf("%-82s\n", "---------------------------------------------------------------------------------");
	printf("%s RM%.2f\n", "TOTAL PRICE :", totalPrice);
}
string TB_DisplaySeatType(int seatType) {
	//printf("Audit Log : string TB_DisplaySeatType(int seatType)\n");
	// 50% Standard[1] : 20% Premium[2] : 20% Sleeping Berths[3] : 10% Special Needs Seats[4](For Child/Seniors)
	switch (seatType)
	{
	case 1: return "Standard"; break;
	case 2: return "Premium"; break;
	case 3: return "Sleeping Berths"; break;
	case 4: return "Special Needs"; break;
	default: return "Not Available"; break;
	}
}

//Update[Admin]
void TB_EditBooking(TrainSchedule trainSchedule)
{
	Ticket newTicketDetails = { 0 };

	double priceDifference = 0;
	double accountBalance = 780.69;
	int selectedBooking = 0;

	double totalPrice = 0;

	int countMemberData = 0;
	int* indexOfMemberBookingID = NULL;

	int lengthOfNotAvailableSeat = 0;
	int preSeatID = 0, currSeatID = 0;

	int* notAvailableSeat = TB_GetSeatAvailability(&lengthOfNotAvailableSeat, trainSchedule.trainID);
	int numRow = G_GetBinFileNumRow("Booking.bin", sizeof(Booking));

	Booking* booking = (Booking*)malloc(sizeof(Booking) * numRow);
	if (!booking) {
		printf("Memory allocated failed.");
		exit(ISERROR);
	}

	//get all the data from the file
	FILE* fptrForRead = fopen("Booking.bin", "rb");
	if (!fptrForRead) {
		printf("Error when opening file.");
		exit(ISERROR);
	}
	fread(booking, sizeof(Booking), numRow, fptrForRead);
	fclose(fptrForRead);

	//get bookingID based on member ID and train ID
	indexOfMemberBookingID =
		TB_GetindexOfMemberBookingID(booking, numRow, &countMemberData, trainSchedule.trainID);
	if (!indexOfMemberBookingID)
	{
		printf("No User record.");
		return;
	}
	do
	{
		//Display User Booking details
		//Header
		int inputIsError = 1;
		do {
			if (inputIsError == -1) return;
			//Header
			TB_DisplayBookingRecordHdr(booking[indexOfMemberBookingID[0]], &totalPrice, trainSchedule);
			//Details
			for (int i = 0; i < countMemberData; TB_DisplayBookingRecordDet(booking[indexOfMemberBookingID[i]], &totalPrice, i + 1), i++);
			//Footer
			TB_DisplayBookingRecordFoot(totalPrice);
		} while (inputIsError = G_IntIsValidated("Select The Booking ID", numRow, &selectedBooking));



		//Edit seat details
		newTicketDetails = TB_GetTicketDetails(trainSchedule, notAvailableSeat, lengthOfNotAvailableSeat);
		priceDifference +=
			(booking[indexOfMemberBookingID[selectedBooking - 1]].ticketDetails.price < newTicketDetails.price) ?
			newTicketDetails.price - booking[indexOfMemberBookingID[selectedBooking - 1]].ticketDetails.price
			:
			0
			;
		sscanf(booking[indexOfMemberBookingID[selectedBooking - 1]].ticketDetails.seatID, "S%d", &preSeatID);
		sscanf(newTicketDetails.seatID, "S%d", &currSeatID);
		for (int i = 0; i < lengthOfNotAvailableSeat; i++)
		{
			if (notAvailableSeat[i] == preSeatID)
			{
				notAvailableSeat[i] = currSeatID;
				break;
			}
		}
		booking[indexOfMemberBookingID[selectedBooking - 1]].ticketDetails = newTicketDetails;
	} while (G_ConfirmationIsValidated("Do you want to continue edit booking details? "));

	if (!TB_CheckAccountBalance(priceDifference))
	{
		free(booking);
		return;
	}

	//Update the newest data
	FILE* fptrForWrite = fopen("Booking.bin", "wb");
	if (!fptrForWrite) {
		printf("File unable to open.");
		exit(ISERROR);
	}

	fwrite(booking, sizeof(Booking), numRow, fptrForWrite);

	fclose(fptrForWrite);
	free(booking);
	free(indexOfMemberBookingID);
}
int* TB_GetindexOfMemberBookingID(Booking* booking, int numRow, int* countMemberData, string trainID) {
	int* indexOfMemberBookingID = NULL, * tempindexOfMemberBookingID = NULL;
	for (int i = 0; i < numRow; i++)
	{
		if (strncmp(booking[i].ticketDetails.trainID, trainID, 10) ||
			strncmp(booking[i].memberID, loggedInMember.memberID, 11) ||
			booking[i].isCancelled) continue;

		tempindexOfMemberBookingID = (int*)realloc(indexOfMemberBookingID, sizeof(int) * ++(*countMemberData));
		if (!tempindexOfMemberBookingID) {
			printf("Memory Allocated failed.");
			free(indexOfMemberBookingID);
			return NULL;
		}
		indexOfMemberBookingID = tempindexOfMemberBookingID;
		indexOfMemberBookingID[*countMemberData - 1] = i;
	}
	if (!indexOfMemberBookingID) {
		printf("User dont have booking record.");
		free(indexOfMemberBookingID);
		return NULL;
	}
	return indexOfMemberBookingID;
}

//Delete[Admin/Member]
void TB_CancelBooking(TrainSchedule trainSchedule)
{
	double totalPrice = 0;


	int* indexOfMemberBookingID = NULL;
	int* dataToBeDeleted = NULL;
	int* tempDataToBeDeleted = NULL;
	int countSelectedData = 0;
	int countMemberData = 0;

	int inputIsError = 1;

	int selectedBooking = 1;
	int numRow = G_GetBinFileNumRow("Booking.bin", sizeof(Booking));

	Booking tempBooking = { 0 };
	Booking* booking = (Booking*)malloc(sizeof(Booking) * numRow);
	if (!booking) {
		printf("Memory allocated failed.");
		exit(ISERROR);
	}

	//get all the data from the file
	FILE* fptrForRead = fopen("Booking.bin", "rb");
	if (!fptrForRead) {
		printf("Error when opening file.");
		exit(ISERROR);
	}
	fread(booking, sizeof(Booking), numRow, fptrForRead);
	fclose(fptrForRead);

	//get bookingID based on member ID and train ID
	indexOfMemberBookingID = TB_GetindexOfMemberBookingID(booking, numRow, &countMemberData, trainSchedule.trainID);
	if (!indexOfMemberBookingID) {
		printf("No User record.");
		return;
	}
	do
	{
		tempDataToBeDeleted = (int*)realloc(dataToBeDeleted,
			sizeof(int) * ((size_t)countSelectedData + 1));
		if (!tempDataToBeDeleted) {
			printf("Memory Allocation Failed.");
			free(dataToBeDeleted);
			exit(ISERROR);
		}
		dataToBeDeleted = tempDataToBeDeleted;
		//Display User Booking details
		do
		{
			if (inputIsError == -1) return;
			//Header
			TB_DisplayBookingRecordHdr(booking[indexOfMemberBookingID[0]], &totalPrice, trainSchedule);
			//Details
			for (int i = 0, count = 0; i < countMemberData; i++)
			{
				if (!G_InArray(dataToBeDeleted, &indexOfMemberBookingID[i],
					sizeof(int), countMemberData, G_CompareInt) && !booking[indexOfMemberBookingID[i]].isCancelled)
				{
					TB_DisplayBookingRecordDet(booking[indexOfMemberBookingID[i]], &totalPrice, ++count);
				}
			}
			//Footer
			TB_DisplayBookingRecordFoot(totalPrice);
		} while (inputIsError = G_IntIsValidated("Select The Booking ID", numRow, &selectedBooking));

		dataToBeDeleted[countSelectedData++] = indexOfMemberBookingID[selectedBooking - 1];

	} while (G_ConfirmationIsValidated("Do you want to continue cancel booking?[Y/N] >>"));

	FILE* fptrForDelete = fopen("Booking.bin", "wb");
	if (!fptrForDelete) {
		printf("File unalbe to open");
		exit(ISERROR);
	}

	for (int i = 0; i < countSelectedData; i++)
	{
		if (G_InArray(dataToBeDeleted, &indexOfMemberBookingID[i],
			sizeof(int), countMemberData, G_CompareInt))
		{
			booking[indexOfMemberBookingID[i]].isCancelled = 1;
		}
	}

	fwrite(booking, sizeof(Booking), numRow, fptrForDelete);
	free(dataToBeDeleted);
	free(indexOfMemberBookingID);
	fclose(fptrForDelete);
}

//Create[Member]
void TB_BookingTicket(TrainSchedule trainSchedule)
{
	int length = 0;
	double totalPrice;

	//get from member information
	int currentPoint = 500;
	double accountBalance = 700.69;
	FILE* fptr = fopen("Booking.bin", "ab");
	if (!fptr)
	{
		printf("Error when opening file.");
		exit(ISERROR);
	}
	Booking* booking = TB_GetBookingDetails(&length);
	if (booking == NULL) {
		return;
	}

	//Display package, update the point
	TB_GetPackageDetails(booking, length);

	//Display User Booked Ticket
	//Header
	TB_DisplayBookingRecordHdr(booking[0], &totalPrice, trainSchedule);
	//Details
	for (int i = 0; i < length; i++) TB_DisplayBookingRecordDet(booking[i], &totalPrice, i + 1);
	//Footer
	TB_DisplayBookingRecordFoot(totalPrice);

	//Booking Confirmation check with member's eWallet's balance, update the balance
	if (!TB_CheckAccountBalance(totalPrice))
	{
		fclose(fptr);
		free(booking);
		return;
	}
	if (G_ConfirmationIsValidated("Booking Confirmation?"))
		fwrite(booking, sizeof(Booking), length, fptr);

	fclose(fptr);
	free(booking);

	//call the function to update the global variable to file 
}
Booking* TB_GetBookingDetails(int* length) {
	int lengthOfBooking = 0;
	int lengthOfNotAvailableSeat = 0;
	int currPointedIndex;
	int seatID = 0;
	int numRow = G_GetBinFileNumRow("Booking.bin", sizeof(Booking));
	Booking* booking = NULL;
	TrainSchedule* trainSchedule = TS_GetTrainSchedule(NULL);
	if (trainSchedule == NULL) {
		return NULL;
	}
	int* notAvailableSeat = TB_GetSeatAvailability(&lengthOfNotAvailableSeat, trainSchedule->trainID);


	do
	{
		Booking* tempBooking = (Booking*)realloc(booking, sizeof(Booking) * ++lengthOfBooking);
		int* tempNotAvailableSeat =
			(int*)realloc(notAvailableSeat, sizeof(int) * ((size_t)lengthOfNotAvailableSeat + (size_t)lengthOfBooking));
		if (!tempBooking || !tempNotAvailableSeat) {
			printf("Memory allocation failed.");
			free(booking);
			free(notAvailableSeat);
			exit(ISERROR);
		}
		booking = tempBooking;
		notAvailableSeat = tempNotAvailableSeat;
		currPointedIndex = lengthOfBooking - 1;

		sprintf((booking + currPointedIndex)->bookingID, "B%04d", numRow + lengthOfBooking);
		strncpy((booking + currPointedIndex)->memberID, loggedInMember.memberID, 11);
		(booking + currPointedIndex)->bookingDate = G_GetCurrentDate();
		(booking + currPointedIndex)->ticketDetails
			= TB_GetTicketDetails(*trainSchedule, notAvailableSeat, lengthOfNotAvailableSeat + lengthOfBooking);
		(booking + currPointedIndex)->isCancelled = 0;

		sscanf((booking + currPointedIndex)->ticketDetails.seatID, "S%d", &seatID);
		notAvailableSeat[lengthOfNotAvailableSeat + currPointedIndex] = seatID;

	} while (G_ConfirmationIsValidated("Do you want to continue add booking?[Y/N] >>"));

	*length = lengthOfBooking;
	free(notAvailableSeat);
	return booking;
}
Ticket TB_GetTicketDetails(TrainSchedule trainSchedule, int* notAvailableSeat, int lengthOfNotAvailableSeat)
{
	Ticket ticketDetails = { 0 };

	TB_DisplaySeat(trainSchedule.availableSeat, &notAvailableSeat, lengthOfNotAvailableSeat);
	TB_GetSeatDetails(trainSchedule, &ticketDetails, &notAvailableSeat, lengthOfNotAvailableSeat);

	return ticketDetails;
}
void TB_DisplaySeat(int seatQuantity, int** notAvailableSeat, int lengthOfNotAvailableSeat) {
	// 50% Standard[1] : 20% Premium[2] : 20% Sleeping Berths[3] : 10% Special Needs Seats[4](For Child/Seniors)
	//each coach can only contain 40 seat(col-10, row-4)
	//number of coach = seatQuantity / 40
	const char* currColor = "\033[0;37m";
	const char* color[] =
	{
		"\033[0;37m", // White[0] -- Available seat / Standard seat / Default Color
		"\033[0;32m", // Green[1] -- Seat for Premium
		"\033[0;33m", // Yellow[2] -- Seat for Sleeping Berths
		"\033[0;34m", // Blue[3] -- Seat for Special Needs Seat
		"\033[0;31m", // Red[4] -- Seat not available
		"\033[1;36m" // Bright Cyan[5] -- Display Coach 
	};

	int	displayNumOfSeat[4] =
	{
		(int)ceil(seatQuantity * 0.5),
		(int)floor(seatQuantity * 0.2),
		(int)floor(seatQuantity * 0.2),
		(int)ceil(seatQuantity * 0.1)
	};
	for (int coach = 0; coach < (seatQuantity / 40); coach++)
	{
		printf("%s", color[5]);
		printf("%30c\n", 'A' + coach);
		printf("%s", color[0]);

		for (int col = 0; col < 4; col++)
		{
			printf("-------------------------------------------------------------\n");
			for (int row = 0; row < 10; row++)
			{
				int seat = row + 1 + (col * 10) + (coach * 40);
				printf("| ");
				for (int i = 0; i < sizeof(displayNumOfSeat) / sizeof(displayNumOfSeat[0]); i++)
				{
					if (displayNumOfSeat[i] <= 0) {
						continue;
					}
					currColor = color[i];
					--displayNumOfSeat[i];
					break;
				}
				if (G_InArray(*notAvailableSeat, &seat, sizeof(int),
					lengthOfNotAvailableSeat, G_CompareInt))
				{
					currColor = color[4];
				}
				printf("%s", currColor);
				printf("%03d ", seat);
				printf("%s", color[0]);
			}
			printf("|\n-------------------------------------------------------------\n\n");
		}
	}

	printf("Seat Color Categories:\n");
	for (int i = 0; i < sizeof(color) / sizeof(color[0]) - 1; i++)
	{
		printf("%s000\033[0m - %s\n", color[i], TB_DisplaySeatType(i + 1));
	}
}
void TB_GetSeatDetails(TrainSchedule trainSchedule, Ticket* ticket, int** notAvailableSeat, int lengthOfNotAvailableSeat) {
	int selectedSeatID = 0;
	int	numOfSeat[4] =
	{
		(int)ceil(trainSchedule.availableSeat * 0.5),
		(int)floor(trainSchedule.availableSeat * 0.2),
		(int)floor(trainSchedule.availableSeat * 0.2),
		(int)ceil(trainSchedule.availableSeat * 0.1)
	};

	int inputIsError = 1;
	while (inputIsError)
	{
		if (inputIsError == -1) return;
		inputIsError = G_IntIsValidated("Select your Seat ID", trainSchedule.availableSeat, &selectedSeatID);
		if (G_InArray(*notAvailableSeat, &selectedSeatID, sizeof(int),
			lengthOfNotAvailableSeat, G_CompareInt)) {
			printf("Seat Unavailable.\n");
			inputIsError = 1;
		}
	}

	sprintf(ticket->seatID, "S%04d", selectedSeatID);
	strncpy(ticket->trainID, trainSchedule.trainID, 10);
	ticket->coach = (char)((int)'A' + (int)floor(selectedSeatID / 40.0));
	int seatType = TB_GetSeatType(selectedSeatID, numOfSeat, sizeof(numOfSeat) / sizeof(numOfSeat[0]));
	if (seatType == -1) return;
	ticket->seatType = seatType;

	double price = TB_GetTicketPrice(ticket->seatType, trainSchedule.departureTime, trainSchedule.departureDate);
	if (price == -1) return;
	ticket->price = price;
}
int* TB_GetSeatAvailability(int* length, string trainID) {
	int* notAvailableSeat = NULL;
	Booking booking = { 0 };
	FILE* fptr = fopen("Booking.bin", "rb");
	if (!fptr)
	{
		printf("Error when opening file.");
		exit(ISERROR);
	}


	while (fread(&booking, sizeof(Booking), 1, fptr) != 0) {
		int* tempNotAvailableSeat = (int*)realloc(notAvailableSeat, sizeof(int) * ++(*length));
		if (!tempNotAvailableSeat) {
			printf("Memory allocation Failed.\n");
			free(notAvailableSeat);
			exit(ISERROR);
		}
		notAvailableSeat = tempNotAvailableSeat;
		if (strncmp(booking.ticketDetails.trainID, trainID, 10) == 0) {
			sscanf(booking.ticketDetails.seatID, "S%d", &notAvailableSeat[*length - 1]);
		}
	}
	fclose(fptr);
	return notAvailableSeat;
}
int TB_GetSeatType(int selectedSeatID, int numOfSeat[], int length) {
	for (int i = 1; i <= length; i++)
	{
		if ((selectedSeatID -= numOfSeat[i - 1]) - 1 <= 0)
		{
			return i;
		}
	}
	return ISERROR;
}
double TB_GetTicketPrice(int seatType, Time startTime, Date departureDate) {
	//50% Standard[1] offer 100% : 20% Premium[2] offer 110%: 20% Sleeping Berths[3] offer 120%: 10% Special Needs Seats[4](For Child/Seniors) offer 80%
	//Ticket price based on time[after 1730 inc price 20%]
	//if get closer to departure date, price higher

	double seatTypeRate[4] = { 1.0,1.1,1.2,0.8 };

	Date currDate = { 12,4,2023 }; // assuming this is current date

	int diffDate = G_CalDiffDate(currDate, departureDate);
	if (diffDate == ISERROR) {
		printf("This date can\'t be selected.\n");
		return -1;
	}

	//Ticket Price = Base Price * SeatType[Rate] * DiffDate[Rate] * startTime[Rate]
	return (double)(5.0 *
		seatTypeRate[seatType - 1] *
		((diffDate > 30) ? 2.0 : 1.0) *
		((startTime.hour * 60 + startTime.min) > 1050 ? 1.2 : 1.0));
}
void TB_GetPackageDetails(Booking* booking, int length) {
	Package package[3] =
	{
		{0.9 , 50, "10% offers", 1, 1},
		{0.5 , 250, "50% offers", 2, 2},
		{0.0 , 450, "100% offers", 3, 5}
	};
	int selectedPackage = 1;

	if (!G_ConfirmationIsValidated("Do you want to use Member Point?")) return;

	printf("Current ticket booked : %d\nCurrent Point : %dP\n", length, loggedInMember.memberRewards);
	for (int i = 0; i < 3 && i < length; i++)
	{
		printf("Package %d : %s for all tickets! Only require %dPs\n"
			, i + 1, package[i].desc, package[i].pointRequired);
	}
	int inputIsError = 1;
	while (inputIsError) {
		inputIsError = G_IntIsValidated("Select the package you want to exchange", length, &selectedPackage);
		if (inputIsError == -1) {
			return;
		}
		if (loggedInMember.memberRewards < package[selectedPackage - 1].pointRequired) {
			printf("Your Point is not enough to exchange this package.\n");
			continue;
		}
	}
	for (int i = 0; i < package[selectedPackage - 1].maxLimits && i < length; i++)
	{
		booking[i].ticketDetails.price *= package[selectedPackage - 1].discount;
	}
	loggedInMember.memberRewards -= package[selectedPackage - 1].pointRequired;
	printf("Selected Package : [Package %d : %s, with %d of Points]\n",
		selectedPackage, package[selectedPackage - 1].desc, package[selectedPackage - 1].pointRequired);
}
int TB_CheckAccountBalance(double priceToPaid) {
	while (loggedInMember.memberWallet < priceToPaid) {
		printf("Current Balance : %.2f\n", loggedInMember.memberWallet);
		if (!G_ConfirmationIsValidated(
			"Balance is not enough to purchase the booking, do you want to top up or cancel the booking?"))
			return 0;
		//redirect user to top up balance
	}
	loggedInMember.memberWallet -= priceToPaid;
	return 1;
}


//Train Schedule
TrainSchedule* TS_GetTrainSchedule(string trainID)
{
	int numRow = G_GetTxtFileNumRow("TrainSchedule.txt");
	TrainSchedule* trainSchedule = (TrainSchedule*)malloc(sizeof(TrainSchedule) * numRow);
	TrainSchedule tempTrainSchedule = { 0 };
	if (!trainSchedule) {
		printf("Memory Allocation Failed.");
		exit(ISERROR);
	}
	int selectedTrainID = 1;
	int count = 0;
	FILE* fptr = fopen("Schedule.txt", "r");
	if (!fptr) {
		printf("File unable open.\n");
		exit(ISERROR);
	}
	int inputIsError = 1;
	do {
		if (inputIsError == -1) break;
		printf("%-3s %-20s %-20s %-20s %-20s\n", "NO.", "Departure Station", "Arrival Station", "Departure Date", "Duration Time");
		printf("%-3s %-20s %-20s %-20s %-20s\n", "---", "---------------", "-------------", "--------------", "-------------");
		for (int i = 0;
			fscanf(fptr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d|%d\n",
				&tempTrainSchedule.trainID, &tempTrainSchedule.departureStation, &tempTrainSchedule.arrivalStation,
				&tempTrainSchedule.departureTime.hour, &tempTrainSchedule.departureTime.min,
				&tempTrainSchedule.arrivalTime.hour, &tempTrainSchedule.arrivalTime.min,
				&tempTrainSchedule.availableSeat,
				&tempTrainSchedule.departureDate.day, &tempTrainSchedule.departureDate.month, &tempTrainSchedule.departureDate.year,
				&tempTrainSchedule.isCancelled) != EOF; )

		{
			if (tempTrainSchedule.isCancelled == 1) {
				continue;
			}
			trainSchedule[i] = tempTrainSchedule;

			if (trainID == NULL) {
				printf("%02d. %-20s %-20s %02d/%02d/%04d %d:%d ~ %d:%d\n", ++count,
					trainSchedule[i].departureStation, trainSchedule[i].arrivalStation,
					trainSchedule[i].departureDate.day, trainSchedule[i].departureDate.month, trainSchedule[i].departureDate.year,
					trainSchedule[i].departureTime.hour, trainSchedule[i].departureTime.min,
					trainSchedule[i].arrivalTime.hour, trainSchedule[i].arrivalTime.min);
				i++;
				continue;
			}
			if (strncmp(trainSchedule[i].trainID, trainID, 10)) return (trainSchedule + i);
		}
		fclose(fptr);

		if (trainID != NULL) {
			printf("No Result founded for %s\n", trainID);
			return NULL;
		}
	} while (inputIsError = G_IntIsValidated("Select The train Schedule", count, &selectedTrainID));
	return inputIsError == -1 ? NULL : (trainSchedule + selectedTrainID - 1);
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

	//testing
	printf("\n\n===============================================================================================================================================\n");
	printf("%-10s %-20s %27s %24s %14s %17s %15s\n", "Train ID", "Departure Station", "Arrival Station", "Departure Time", "Arrival Time", "Available Seats", "Departure Date");
	printf("===============================================================================================================================================\n");
	while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d|%d\n",
		train.trainID, train.departureStation, train.arrivalStation,
		&train.departureTime.hour, &train.departureTime.min,
		&train.arrivalTime.hour, &train.arrivalTime.min,
		&train.availableSeat,
		&train.departureDate.day, &train.departureDate.month, &train.departureDate.year,
		&train.isCancelled) != EOF) {
		if (train.isCancelled == 0) {
			printf("%-10s %-30s %-28s %-2s %02d:%02d %10s %02d:%02d %10s %-10d %-2s %02d/%02d/%04d\n",
				train.trainID, train.departureStation, train.arrivalStation,
				"", train.departureTime.hour, train.departureTime.min, "",
				train.arrivalTime.hour, train.arrivalTime.min, "",
				train.availableSeat, "", train.departureDate.day, train.departureDate.month, train.departureDate.year);
			count++;
		}
	}
	printf("Total %d record in train schedule\n", count);
	fclose(fptr);
	system("pause");
}
void TS_addSchedule() {
	int dayOfMonth[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
	Date currDate = G_GetCurrentDate();
	int diffDate = 0;
	int count = 0;

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
	do {
		//Departure Station
		G_DrawTrain();
		printf("Enter the Departure Station (Enter \'0000\' to exit):");
		scanf("%[^\n]", &train.departureStation);
		rewind(stdin);
		if (strncmp(train.departureStation, "0000", 50) == 0) {
			fclose(fptr);
			return;
		}
		printf("\n");
		//Arrival Station
		G_DrawTrain();
		printf("Enter the Arrival Station (Enter \'0000\' to exit):");
		scanf("%[^\n]", &train.arrivalStation);
		rewind(stdin);
		if (strncmp(train.arrivalStation, "0000", 50) == 0) {
			fclose(fptr);
			return;
		}
		printf("\n");


		//Departure Time
		G_DrawTrain();
		train.departureTime = G_GetTime("Departure Time");
		if (train.departureTime.hour == ISERROR) {
			fclose(fptr);
			return;
		}
		printf("\n");

		//Arrival Time
		G_DrawTrain();
		train.arrivalTime = G_GetTime("Arrival Time");
		if (train.arrivalTime.hour == ISERROR) {
			fclose(fptr);
			return;
		}
		printf("\n");

		//Available Seat
		do {
			G_DrawTrain();
			if (inputIsError == ISERROR) {
				fclose(fptr);
				return;
			}
			if (inputIsError == 1)  G_ErrorMessage();
		} while (inputIsError =
			G_IntIsValidated("Enter the number of Coach(each coach contains 40 seat, maximum 5 coach)", 5, &train.availableSeat));
		printf("\n");

		//Departure Date
		do
		{
			G_DrawTrain();
			if (diffDate > 0) printf("Departure Date can\'t be before current date.\n");

			//Departure Date (Year)
			while (inputIsError =
				G_IntIsValidated("Enter the Departure Date(Years)", currDate.year, &train.departureDate.year)) {
				if (inputIsError == -1) {
					fclose(fptr);
					return;
				}
				if (inputIsError == 1)  G_ErrorMessage();
			}

			G_DrawTrain();
			//Departure Date(Month)
			while (inputIsError =
				G_IntIsValidated("Enter the Departure Date(Months)", 12, &train.departureDate.month)) {
				if (inputIsError == -1) {
					fclose(fptr);
					return;
				}
				if (inputIsError == 1)  G_ErrorMessage();
			}

			//Calculate the day of febuary
			dayOfMonth[1] = train.departureDate.year % 4 + 28;
			G_DrawTrain();
			//Departure Date(Day)
			while (inputIsError =
				G_IntIsValidated("Enter the Departure Date(Day)", dayOfMonth[train.departureDate.month - 1], &train.departureDate.day)) {
				if (inputIsError == -1) {
					fclose(fptr);
					return;
				}
				if (inputIsError == 1)  G_ErrorMessage();
			}

			diffDate = G_CalDiffDate(currDate, train.departureDate);
		} while (diffDate > 0);
		diffDate = 0;

		sprintf(train.trainID, "T%04d", numRow + 1 + count++); //newTrainID = numOfRow + 1
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
		system("cls");
	} while (G_ConfirmationIsValidated("Do you want to continue add train Schedule?"));
	fclose(fptr);
	system("pause");
}
void TS_searchSchedule() {
	int count = 0;
	char searchArrivalStation[50] = "";
	int isFound = 0;
	TrainSchedule train = { 0 };

	FILE* fptr = fopen("TrainSchedule.txt", "r");

	if (!fptr) {
		printf("Error: Unable to open file.\n");
		exit(ISERROR);
	}

	G_DrawTrain();
	printf("Welcome to the search mode\n");
	printf("Please enter the arrival station you want (Enter 'quit' to back to main menu) : ");
	scanf("%[^\n]", &searchArrivalStation);
	rewind(stdin);
	if (strncmp(searchArrivalStation, "quit", 50) == 0) {
		fclose(fptr);
		return;
	}

	printf("\n");
	printf("===============================================================================================================================================\n");
	printf("%-10s %-20s %27s %24s %14s %17s %15s\n", "Train ID", "Departure Station", "Arrival Station", "Departure Time", "Arrival Time", "Available Seats", "Departure Date");
	printf("===============================================================================================================================================\n");

	while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%02d:%02d|%02d:%02d|%d|%02d/%02d/%04d|%d\n",
		train.trainID, train.departureStation, train.arrivalStation,
		&train.departureTime.hour, &train.departureTime.min,
		&train.arrivalTime.hour, &train.arrivalTime.min,
		&train.availableSeat,
		&train.departureDate.day, &train.departureDate.month, &train.departureDate.year,
		&train.isCancelled) != EOF) {
		if (strstr(strupr(train.arrivalStation), strupr(searchArrivalStation)) && train.isCancelled == 0) {
			printf("%-10s %-30s %-28s %-2s %02d:%02d %10s %02d:%02d %10s %-10d %-2s %02d/%02d/%04d\n",
				train.trainID, train.departureStation, train.arrivalStation,
				"", train.departureTime.hour, train.departureTime.min, "",
				train.arrivalTime.hour, train.arrivalTime.min, "",
				train.availableSeat, "", train.departureDate.day, train.departureDate.month, train.departureDate.year);
			count++;
			isFound = 1;
		}
	}
	if (isFound) {
		printf("Total %d record in train schedule\n", count);
	}
	else {
		printf("Not result found.\n");
	}
	fclose(fptr);
	system("pause");
	system("cls");
	if (G_ConfirmationIsValidated("Do you want to continue to search?")) {
		TS_searchSchedule();
	}
}
void TS_deleteSchedule() {
	TrainSchedule train[100] = { 0 };
	char TrainID[10] = "";
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
		G_DrawTrain();
		printf("Please enter the train ID you want to delete (Enter \'0000\' to exit): ");
		scanf("%[^\n]", &TrainID);
		if (strncmp(TrainID, "0000", 10) == 0) return;

		rewind(stdin);
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
		G_DrawTrain();
		printf("\n\n\nTrain ID to be modified (Enter\'0000\' to exit):");
		scanf("%[^\n]", &trainID);
		rewind(stdin);
		if (strncmp(trainID, "0000", 10) == 0) {
			return;
		}

		for (int i = 0; i < trainCount; i++) {
			if (strncmp(trainID, train[i].trainID, 10) == 0 && train[i].isCancelled == 0) {
				found = 1;
				//Train ID
				strncpy(tempTrainSchedule.trainID, train[i].trainID, 10);

				//Departure Station
				G_DrawTrain();
				printf("Enter the Departure Station (Enter \'0000\' to exit):");
				scanf("%[^\n]", &tempTrainSchedule.departureStation);
				rewind(stdin);
				if (strncmp(tempTrainSchedule.departureStation, "0000", 50) == 0) {
					return;
				}

				//Arrival Station
				G_DrawTrain();
				printf("Enter the Arrival Station (Enter \'0000\' to exit):");
				scanf("%[^\n]", &tempTrainSchedule.arrivalStation);
				rewind(stdin);
				if (strncmp(tempTrainSchedule.arrivalStation, "0000", 50) == 0) {
					return;
				}

				//Departure Time
				G_DrawTrain();
				tempTrainSchedule.departureTime = G_GetTime("Departure Time");
				if (tempTrainSchedule.departureTime.hour == -1) {
					return;
				}

				//Arrival Time
				G_DrawTrain();
				tempTrainSchedule.arrivalTime = G_GetTime("Arrival Time");
				if (tempTrainSchedule.arrivalTime.hour == -1) {
					return;
				}

				//Available Seat
				do {
					G_DrawTrain();
					if (inputIsError == -1) {
						return;
					}
					if (inputIsError == 1)  G_ErrorMessage();
				} while (inputIsError =
					G_IntIsValidated("Enter the number of Coach(each coach contains 40 seat, maximum 5 coach)", 5, &tempTrainSchedule.availableSeat));

				//Departure Date
				do
				{
					G_DrawTrain();
					if (diffDate < 0) printf("Departure Date can\'t be before current date.\n");

					//Departure Date (Year)
					while (inputIsError =
						G_IntIsValidated("Enter the Departure Date(Years)", currDate.year, &tempTrainSchedule.departureDate.year)) {
						if (inputIsError == -1) {
							return;
						}
						if (inputIsError == 1)  G_ErrorMessage();
					}
					G_DrawTrain();
					//Departure Date(Month)
					while (inputIsError =
						G_IntIsValidated("Enter the Departure Date(Months)", 12, &tempTrainSchedule.departureDate.month)) {
						if (inputIsError == -1) {
							return;
						}
						if (inputIsError == 1)  G_ErrorMessage();
					}

					//Calculate the day of febuary
					dayOfMonth[1] = tempTrainSchedule.departureDate.year % 4 + 28;
					G_DrawTrain();
					//Departure Date(Day)
					while (inputIsError =
						G_IntIsValidated("Enter the Departure Date(Days)",
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
				tempTrainSchedule.availableSeat *= 40;
				
				if (G_ConfirmationIsValidated("\nModify Train Schedule ?"))
				{
					found = 0;
					train[i] = tempTrainSchedule;
				}
				else {
					printf("No changes made\n");
				}
			}
		}

		if (!found) {
			printf("\nThe Train ID cannot be modified. Please enter a valid Train ID.\n");
		}
	} while (found == 1);

	FILE* fptrForWrite = fopen("TrainSchedule.txt", "w");
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

//Staff Informaion[Staff Only]
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
	for (int i = 0; fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%lf\n",
		&staffList[i].staffID, &staffList[i].name, &staffList[i].password, &staffList[i].position,
		&staffList[i].joinedDate.day, &staffList[i].joinedDate.month, &staffList[i].joinedDate.year,
		&staffList[i].salary) != EOF; i++);
	fclose(fptr);
	int inputIsError = 0;

	Staff staff = { 0 };
	G_DrawTrain();
	printf("\n \t< Add Staff >\n");
	G_lineDesign();

	//Staff ID
	do
	{
		inputIsError = 0;
		printf("Staff ID (Enter \'0000\' to exit):");
		scanf("%[^\n]", &staff.staffID);
		rewind(stdin);
		if (strncmp(staff.staffID, "0000", 11) == 0) {
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
	scanf("%19[^\n]", &staff.name);
	rewind(stdin);
	if (strncmp(staff.name, "0000", 11) == 0) {
		return;
	}

	//Password
	printf("Password (Enter \'0000\' to exit):");
	scanf("%29[^\n]", &staff.password);
	rewind(stdin);
	if (strncmp(staff.password, "0000", 11) == 0) {
		return;
	}

	//Position
	printf("Position (Enter \'0000\' to exit):");
	scanf("%19[^\n]", &staff.position);
	rewind(stdin);
	if (strncmp(staff.position, "0000", 11) == 0) {
		return;
	}

	//Joined date
	do
	{
		if (diffDate < 0) printf("Joined Date can\'t be before current date.\n");

		//Joined Date (Year)
		while (inputIsError =
			G_IntIsValidated("Enter the Joined Date(Years)", currDate.year, &staff.joinedDate.year)) {
			if (inputIsError == ISERROR) {
				return;
			}
			if (inputIsError == 1)  G_ErrorMessage();
		}

		//Joined Date(Month)
		while (inputIsError =
			G_IntIsValidated("Enter the Joined Date(Months)", 12, &staff.joinedDate.month)) {
			if (inputIsError == ISERROR) {
				return;
			}
			if (inputIsError == 1)  G_ErrorMessage();
		}

		//Calculate the day of febuary
		dayOfMonth[1] = staff.joinedDate.year % 4 + 28;

		//Joined Date(Day)
		while (inputIsError =
			G_IntIsValidated("Enter the Joined Date(Days)", dayOfMonth[staff.joinedDate.month - 1], &staff.joinedDate.day)) {
			if (inputIsError == ISERROR) {
				return;
			}
			if (inputIsError == 1)  G_ErrorMessage();
		}

		diffDate = G_CalDiffDate(currDate, staff.joinedDate);
	} while (diffDate < 0);
	diffDate = 0;

	//vadition get from member Information
	char tempSalary[11] = "";
	do
	{
		printf("  Monthly salary: ");
		scanf("%10[^\n]", &tempSalary);
		rewind(stdin);
	} while (MI_InputDetailsValidation(tempSalary, "topup") != 0);
	staff.salary = atoi(tempSalary);
	printf("\n");

	fptr = fopen("staffs.txt", "a");
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
	G_DrawTrain();
	printf("\n\t< Search Staff >\n");
	G_lineDesign();
	printf("  Enter staff ID (Enter \'0000\' to exit):");
	scanf("%10[^\n]", &searchStaff);
	rewind(stdin);
	if (strncmp(searchStaff, "0000", 11) == 0) return;

	for (int i = 0; i < r; i++)
	{
		if (strncmp(staff[i].staffID, searchStaff, 11) == 0)
		{
			printf("  Staff ID	 : %s\n", staff[i].staffID);
			printf("  Name		 : %s\n", staff[i].name);
			printf("  Password	 : %s\n", staff[i].password);
			printf("  Position	 : %s\n", staff[i].position);
			printf("  Joined date(NO/ DDMMYY)    : %d/%d/%d\n", staff[i].joinedDate.day, staff[i].joinedDate.month, staff[i].joinedDate.year);
			printf("  Salary	 : RM%.2f\n\n", staff[i].salary);
			system("pause");
			break;
		}
	}
}
void SI_modifyStaff() {
	Staff staff[20] = { 0 };
	char searchStaff[11] = "";
	char tempSalary[11] = "";
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
	for (r = 0; fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%lf\n",
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
	G_DrawTrain();
	printf("\n\t< Modify Staff >\n");
	G_lineDesign();
	printf("  Enter staff ID (Enter \'0000\' to exit):");
	scanf("%10[^\n]", &searchStaff);
	rewind(stdin);
	if (strncmp(searchStaff, "0000", 11) == 0) return;

	for (i = 0; i < r; i++) {
		if (strcmp(searchStaff, staff[i].staffID) == 0) {
			do {
				if (inputIsError == ISERROR) return;
				printf("\n\t< Modification - Staff >\n");
				G_lineDesign();
				printf("  1. Staff ID\n");
				printf("  2. Name\n");
				printf("  3. Password\n");
				printf("  4. Position\n");
				printf("  5. Joined date(NO/ DDMMYY)\n");
				printf("  6. Salary\n");
				G_lineDesign();
			} while (inputIsError =
				G_IntIsValidated("Select the attribute to edit", 6, &decision));
			switch (decision) {
			case 1:
				printf("  Modifying - Staff ID: ");
				scanf("%10[^\n]", &staff[i].staffID);
				break;
			case 2:
				printf("  Modifying - Name: ");
				scanf("%19[^\n]", &staff[i].name);
				break;
			case 3:
				printf("  Modifying - Password: ");
				scanf("%29[^\n]", &staff[i].password);
				break;
			case 4:
				printf("  Modifying - Position: ");
				scanf("%19[^\n]", &staff[i].position);
				break;
			case 5:
				do
				{
					if (diffDate < 0) printf("Joined Date can\'t be before current date.\n");
					//Joined Date (Year)
					while (inputIsError =
						G_IntIsValidated("Enter the Joined Date(Years)", currDate.year, &staff[i].joinedDate.year)) {
						if (inputIsError == 1)  G_ErrorMessage();
					}

					//Joined Date(Month)
					while (inputIsError =
						G_IntIsValidated("Enter the Joined Date(Months)", 12, &staff[i].joinedDate.month)) {
						if (inputIsError == -1) {
							return;
						}
						if (inputIsError == 1)  G_ErrorMessage();
					}

					//Calculate the day of febuary
					dayOfMonth[1] = staff[i].joinedDate.year % 4 + 28;

					//Joined Date(Day)
					while (inputIsError =
						G_IntIsValidated("Enter the Joined Date(Days)", dayOfMonth[staff[i].joinedDate.month - 1], &staff[i].joinedDate.day)) {
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

				do
				{
					printf("  Monthly salary: ");
					scanf("%10[^\n]", &tempSalary);
					rewind(stdin);
				} while (MI_InputDetailsValidation(tempSalary, "topup") != 0);
				staff[i].salary = atoi(tempSalary);
				printf("\n");
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
		fprintf(fptr, "%s|%s|%s|%s|%d/%d/%d|%.2f\n", staff[i].staffID, staff[i].name, staff[i].password, staff[i].position, staff[i].joinedDate.day, staff[i].joinedDate.month, staff[i].joinedDate.year, staff[i].salary);
	}
	fclose(fptr);
}
void SI_displayStaff() {
	Staff staff = { 0 };
	char searchStaff[11] = "";
	int r;
	FILE* fptr = fopen("staffs.txt", "r");
	if (!fptr) {
		printf("  Unable to open file.\n");
		exit(ISERROR);
	}
	G_DrawTrain();
	for (r = 0; fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%lf\n",
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
	system("pause");
}
void SI_deleteStaff() {
	char staffDel[11] = "";
	Staff staff[20] = { 0 };
	int i = 0;

	FILE* fptr = fopen("staffs.txt", "r");
	if (!fptr)
	{
		printf("Cant open the file\n");
		exit(ISERROR);
	}

	for (i = 0;
		fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%d/%d/%d|%lf\n",
			&staff[i].staffID, &staff[i].name, &staff[i].password, &staff[i].position,
			&staff[i].joinedDate.day, &staff[i].joinedDate.month, &staff[i].joinedDate.year,
			&staff[i].salary) != EOF; i++);
	fclose(fptr);

	printf("+ - - - - - - - - - - +\n");
	printf("|%-21s|\n", "Staff Existing ID:");
	for (int j = 0; j < i; j++)
	{
		printf("|%-21s|\n", staff[j].staffID);
	}
	printf("+ - - - - - - - - - - +\n\n");



	printf("Which Data do you wish to delete (Enter \'0000\' to exit) :");
	scanf("%10[^\n]", &staffDel);
	rewind(stdin);
	if (strncmp(staffDel, "0000", 11) == 0) return;
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


//Member Information
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
		return NULL;
	}

	// Allocate memory for member details
	Member* allMembers = (Member*)malloc((rows) * sizeof(Member));
	if (allMembers == NULL) {
		printf("\nMemory allocation failed.");
		fclose(fptr);
		return NULL;
	}

	// Read member details from the binary file
	fread(allMembers, sizeof(Member), rows, fptr);
	return allMembers;
}

//Register or Login [Member]
void MI_memberLogin() {
	char inputID[11];
	int verify;
	do {
		system("cls");
		G_DrawTrain();
		printf("MEMBER LOGIN\n\n");
		printf("Please enter your Member ID (at least 7, maximum 10 characters): ");
		scanf("%[^\n]", &inputID);
		rewind(stdin);
		printf("\n\nPlease enter your password (at least 7, maximum 10 characters): ");
		string inputPass = MI_getPassword();

		verify = MI_verifyLogin(inputID, inputPass);

		if (verify != 0) {
			printf("Wrong ID or password.\n");
		}
	} while (verify != 0);

	if (verify == 0) {
		MI_loggedInMemberDetails(inputID);
		while (MI_mainMenu());
	}
}
int MI_verifyLogin(string inputID, string inputPass) {
	int numOfMembers = MI_getNumberOfMembers();
	Member* allMembers = MI_getMemberDetails(numOfMembers);
	if (allMembers == NULL) {
		printf("\nFailed to read member details.\n");
		exit(ISERROR);
	}

	for (int i = 0; i < numOfMembers; i++) {
		if (strcmp(allMembers[i].memberID, inputID) == 0) {
			if (strcmp(allMembers[i].memberPass, inputPass) == 0) {
				printf("\n\n\nLogin successful, %s\n\n\n", allMembers[i].memberName);
				free(allMembers);
				system("pause");
				return 0;
			}
			else {
				printf("\n\n\nLogin failed, wrong ID or password.\n");
				system("pause");
				free(allMembers);
				return 1;
			}
		}
	}
	free(allMembers);
	return 1;
}
void MI_loggedInMemberDetails(string memberID) {
	//save logged-in member info into global struct
	int numOfMembers = MI_getNumberOfMembers();
	Member* allMembers = MI_getMemberDetails(numOfMembers);
	for (int i = 0; i < numOfMembers; i++) {
		if (strncmp(allMembers[i].memberID, memberID, 11) == 0) {

			loggedInMember = allMembers[i];

		}

	}

};
void MI_registerMember() {
	Member newMember = { 0 };
	int verify;
	int validation = 0;
	char tempPass[11] = "", confirmPass[11] = "";

	do {
		do {
			system("cls");
			G_DrawTrain();
			printf("MEMBER SIGN UP\n\n");
			printf("Please enter a new Member ID (at least 7, maximum 10 characters): ");
			scanf("%[^\n]", &newMember.memberID);
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
	newMember.memberJoinDate = G_GetCurrentDate();
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
	G_systemMenu();
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

//Member Account Management
void MI_topUpWallet(string memberID) {
	char tempWallet[11] = "";
	char* endptr;
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
			tempWallet2 = strtod(tempWallet, &endptr);
			tempWallet2 = allMembers[i].memberWallet + tempWallet2;
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

	Member newDetails = { 0 };
	int verify;
	int validation = 0, errorPassword;
	char tempPass1[11] = "", tempPass2[11] = "", confirmPass[11] = "";
	for (int i = 0; i < numOfMembers; i++) {
		if (strcmp(memberID, allMembers[i].memberID) == 0) {
			strcpy(newDetails.memberID, memberID);
			if (G_ConfirmationIsValidated("\n\nDo you want to edit password?")) {
				do {
					do {
						system("cls");
						G_DrawTrain();
						do {
							errorPassword = 0;
							printf("Please enter the current password for this account: ");
							strncpy(tempPass1, MI_getPassword(), 11);
							if (strcmp(tempPass1, allMembers[i].memberPass) != 0) {
								printf("\n\nWrong password. Please try again. \n\n");
								errorPassword = 1;
							}
						} while (errorPassword == 1);
						
						printf("\n\nPlease enter a new password (at least 7, maximum 10 characters): ");

						strncpy(tempPass2, MI_getPassword(), 11);
						validation = MI_InputDetailsValidation(tempPass2, "idOrPass");

					} while (validation != 0);
					printf("\nPlease re-enter to confirm new password: ");
					strncpy(confirmPass, MI_getPassword(), 11);
					verify = strncmp(tempPass2, confirmPass, 11);
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
			if (G_ConfirmationIsValidated("\n\nDo you want to edit name?")) {
				do {
					printf("New name: ");
					scanf("%s", &newDetails.memberName);
					rewind(stdin);
				} while (MI_InputDetailsValidation(newDetails.memberName, "name") != 0);
			}
			else {
				strcpy(newDetails.memberName, allMembers[i].memberName);
			}
			if (G_ConfirmationIsValidated("\n\nDo you want to edit phone number?")) {
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
void MI_memberDeleteAccount(string memberID) {
	int numOfMembers = MI_getNumberOfMembers();
	Member* allMembers = MI_getMemberDetails(numOfMembers);
	int deleted = 0;

	if (G_ConfirmationIsValidated("\n\nAre you sure you want to delete account?")) {
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
				printf("\n\nAccount Deleted.\n\n");
				deleted = 1;
			}

		}
		fclose(fptrBin);
	}
	free(allMembers);
	printf("\n");
	system("pause");
	if (deleted == 1) {
		G_systemMenu();
	}
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
		else if ((int)c == 124 || (int)c <= 33 || (int)c >= 126) { // Check if printable character
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

				if (!isdigit(value[i])) {

					printf("\n\nYour search year has invalid input (character/symbol). Please re-enter again.\n\n");
					system("pause");
					return 1;

				};

			};
		}
		return 0;

	}
	else if (strcmp(mode, "date-monthOrDay") == 0) {
		if (!(strlen(value) <= 2)) {
			G_ErrorMessage();
			system("pause");
			return 1;
		}
		else {
			for (int i = 0; i < strlen(value); i++) {

				if (!isdigit(value[i])) {

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

			if (!isdigit(value[i])) {

				printf("\n\nYour reward points has invalid input (character/symbol). Please re-enter again.\n\n");
				system("pause");
				return 1;

			};

		};
		return 0;

	}
	else if (strcmp(mode, "topUp") == 0) {
		//check topup amount format
		int dotCount = 0;
		for (int i = 0; value[i]; i++) {
			if (!isdigit(value[i])) {
				if (value[i] == '.') {
					dotCount++;
					// Check only one dot is present
					if (dotCount > 1)
						return 1;
				}
				else {
					return 1;
				}
			}
		}
		return 0;
	}
	return 0;
};

/*for staff use*/ void MI_displayAllMembers() {

	int numOfMembers = MI_getNumberOfMembers();
	Member* allMembers = MI_getMemberDetails(numOfMembers);
	Date date = G_GetCurrentDate();
	Time time = G_GetCurrentTime();
	printf("\n\nList of Members AS OF %02d-%02d-%04d %02d:%02d\n\n", date.day, date.month, date.year, time.hour, time.min);
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
	printf("\n\n");
	system("pause");
	free(allMembers);

};
/*for staff use*/ void MI_displayMembersHeaderOrFooter(char HeaderOrFooter) {

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
/*for staff use*/ void MI_staffSearchMember() {
	int MI_searchYear, MI_searchMonth, MI_searchDay;
	char MI_searchID[11], MI_searchName[50];
	int numOfMembers = MI_getNumberOfMembers();
	Member* allMembers = MI_getMemberDetails(numOfMembers);
	int MI_menuDecision;
	do {
		do {
			system("cls");
			G_DrawTrain();
			printf("< SEARCH MEMBER >\n\n");
			printf("Search member through:\n\n");
			printf("1. Joined year\n");
			printf("2. Joined month\n");
			printf("3. Joined day\n");
			printf("4. Member ID\n");
			printf("5. Member Name\n\n");
		} while (G_IntIsValidated("Mode", 5, &MI_menuDecision));

		char tempDate[10];
		switch (MI_menuDecision) {

		case 1:
			//based on year
			do {
				printf("\n\nSearch member joined on year: ");
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
				printf("\n\nSearch member joined on month: ");
				scanf("%s", &tempDate);
				rewind(stdin);
			} while (MI_InputDetailsValidation(tempDate, "date") != 0);
			printf("\n\n");
			MI_searchMonth = atoi(tempDate);
			MI_displayMembersHeaderOrFooter('H');
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
				printf("\n\nSearch member joined on day: ");
				scanf("%s", &tempDate);
				rewind(stdin);
			} while (MI_InputDetailsValidation(tempDate, "date") != 0);
			printf("\n\n");
			MI_searchDay = atoi(tempDate);
			MI_displayMembersHeaderOrFooter('H');
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
				printf("\n\nSearch member by member ID: ");
				scanf("%s", &MI_searchID);
				rewind(stdin);
			} while (MI_InputDetailsValidation(MI_searchID, "idOrPass") != 0);
			printf("\n\n");
			MI_displayMembersHeaderOrFooter('H');
			for (int i = 0; i < numOfMembers; i++) {

				if (strcmp(MI_searchID, allMembers[i].memberID) == 0) {
					MI_displaySearchedMembers(allMembers[i].memberID);
				}
			}
			break;
		case 5:
			// based on member name
			do {
				printf("\n\nSearch member by member name : ");
				scanf("%[^\n]", &MI_searchName);
				rewind(stdin);
			} while (MI_InputDetailsValidation(MI_searchName, "name") != 0);
			printf("\n\n");
			// Convert search name to lowercase
			for (int i = 0; MI_searchName[i]; i++) {
				MI_searchName[i] = tolower(MI_searchName[i]);
			}
			MI_displayMembersHeaderOrFooter('H');
			for (int i = 0; i < numOfMembers; i++) {
				// Convert member name to lowercase for comparison
				char loweredMemberName[50];
				strcpy(loweredMemberName, allMembers[i].memberName);
				for (int j = 0; loweredMemberName[j]; j++) {
					loweredMemberName[j] = tolower(loweredMemberName[j]);
				}
				if (strstr(loweredMemberName, MI_searchName) != NULL) {
					MI_displaySearchedMembers(allMembers[i].memberID);
				}
			}
			break;
		}
	} while (G_ConfirmationIsValidated("Do you want to continue search for members?") == 1);
	free(allMembers);
};
/*for staff use*/ void MI_displaySearchedMembers(string memberID) {
	int numOfMembers = MI_getNumberOfMembers();
	Member* allMembers = MI_getMemberDetails(numOfMembers);
	int numOfMembersFound = 0;
	for (int i = 0; i < numOfMembers; i++) {
		if (strncmp(memberID, allMembers[i].memberID, 11) == 0) {
			numOfMembersFound++;
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
	printf("\n%d member(s) found. \n\n", numOfMembersFound);
	free(allMembers);


};
/*for staff use*/ void MI_staffAddMember() {

	Member newMember = { 0 };
	int verify;
	char tempPass[11] = "", confirmPass[11] = "";

	do {
		do {
			do {
				system("cls");
				G_DrawTrain();
				printf("< ADD MEMBER >\n\n");
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
				G_DrawTrain();
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
			G_DrawTrain();
			printf("Please enter new member's name: ");
			scanf("%[^\n]", &newMember.memberName);
			rewind(stdin);
		} while (MI_InputDetailsValidation(newMember.memberName, "name") != 0);

		do {
			G_DrawTrain();
			printf("Please enter new member's phone number: ");
			scanf("%s", &newMember.memberPhoneNo);
			rewind(stdin);
		} while (MI_InputDetailsValidation(newMember.memberPhoneNo, "phoneNo") != 0);


		newMember.memberJoinDate = G_GetCurrentDate();
		char tempReward[10];
		do {
			G_DrawTrain();
			printf("Please enter new member's reward points: ");
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
	} while (G_ConfirmationIsValidated("Do you want to add more members?") == 1);
};
/*for staff use*/ void MI_staffDeleteMember() {
	char tempID[11] = "";
	int numOfMembers = MI_getNumberOfMembers();
	Member* allMembers = MI_getMemberDetails(numOfMembers);
	do {
		system("cls");
		G_DrawTrain();
		MI_displayAllMembers();
		printf("< DELETE MEMBER >\n\n");
		printf("Enter member ID to cancel membership: ");
		scanf("%s", tempID);
		rewind(stdin);


		for (int i = 0; i < numOfMembers; i++) {
			if (strcmp(allMembers[i].memberID, tempID) == 0) {
				MI_displayDetails(allMembers[i].memberID);

				if (G_ConfirmationIsValidated("Are you sure you want to delete?")) {
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
							printf("\n\nMember \"%s\" Deleted.\n", allMembers[i].memberName);
						}

					}
					fclose(fptrBin);
				}

			}
		}
	} while (G_ConfirmationIsValidated("Do you want to continue delete more members?") == 1);

	free(allMembers);

}
/*for staff use*/ void MI_staffEditMemberDetails() {
	char tempID[11];
	int numOfMembers = MI_getNumberOfMembers();
	Member* allMembers = MI_getMemberDetails(numOfMembers);
	do {
		system("cls");
		G_DrawTrain();
		MI_displayAllMembers();
		do {
			printf("\n\n< EDIT MEMBER DETAILS >\n\n");
			printf("Enter member ID to edit details: ");
			scanf("%s", &tempID);
			rewind(stdin);
		} while (MI_InputDetailsValidation(tempID, "idOrPass") != 0);
		printf("\n\n");
		MI_displayDetails(tempID);
		printf("\n\n");
		system("pause");

		Member newDetails = { 0 };
		int verify;
		char tempPass[11], confirmPass[11], tempYear[5], tempMonth[3], tempDay[3], tempReward[10];
		for (int i = 0; i < numOfMembers; i++) {
			if (strcmp(tempID, allMembers[i].memberID) == 0) {
				strcpy(newDetails.memberID, tempID);
				G_DrawTrain();
				if (G_ConfirmationIsValidated("Do you want to edit password?")) {
					do {
						do {
							printf("Please enter a new password (at least 7, maximum 10 characters): ");

							strncpy(tempPass, MI_getPassword(), 11);
						} while (MI_InputDetailsValidation(tempPass, "idOrPass") != 0);

						printf("\n\nPlease re-enter to confirm new password: ");
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
				G_DrawTrain();
				if (G_ConfirmationIsValidated("\nDo you want to edit name?")) {
					do {
						printf("\nNew name: ");
						scanf("%s", &newDetails.memberName);
						rewind(stdin);
					} while (MI_InputDetailsValidation(newDetails.memberName, "name") != 0);
				}
				else {
					strncpy(newDetails.memberName, allMembers[i].memberName, 11);
				}

				G_DrawTrain();
				if (G_ConfirmationIsValidated("\nDo you want to edit phone number?")) {
					do {
						printf("\nNew phone number (XXXXXXXXXX or XXXXXXXXXXX): ");
						scanf("%s", &newDetails.memberPhoneNo);
						rewind(stdin);
					} while (MI_InputDetailsValidation(newDetails.memberPhoneNo, "phoneNo") != 0);
				}
				else {
					strncpy(newDetails.memberPhoneNo, allMembers[i].memberPhoneNo, 12);
				}
				G_DrawTrain();
				if (G_ConfirmationIsValidated("\nDo you want to edit Joined Year?")) {
					do {
						printf("\nNew joined year: ");
						scanf("%s", &tempYear);
						rewind(stdin);
					} while (MI_InputDetailsValidation(tempYear, "date-year") != 0);
					newDetails.memberJoinDate.year = atoi(tempYear);

				}
				else {
					newDetails.memberJoinDate.year = allMembers[i].memberJoinDate.year;
				}
				
				G_DrawTrain();
				if (G_ConfirmationIsValidated("\nDo you want to edit Joined Month?")) {
					verify = 0;
					do {

						do {
							printf("\nNew joined month: ");
							scanf("%s", &tempMonth);
							rewind(stdin);
						} while (MI_InputDetailsValidation(tempMonth, "date-monthOrDay") != 0);

						newDetails.memberJoinDate.month = atoi(tempMonth);

						if (newDetails.memberJoinDate.month < 1 || newDetails.memberJoinDate.month > 12) {

							printf("\n\nInvalid input. Month should be between 1 to 12.\n\n");
							verify = 1;
							system("pause");

						}

					} while (verify != 0);
				}
				else {
					newDetails.memberJoinDate.month = allMembers[i].memberJoinDate.month;
				}
				verify = 0;
				int dayOfMonth[12] = { 31,29,31,30,31,30,31,31,30,31,30,31 };
				G_DrawTrain();
				if (G_ConfirmationIsValidated("\nDo you want to edit Joined Day?")) {
					do {
						do {
							printf("\n\nNew joined day: ");
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
				G_DrawTrain();
				if (G_ConfirmationIsValidated("\nDo you want to edit Reward Points?")) {
					do {
						printf("\n\nNew reward points: ");
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
	} while (G_ConfirmationIsValidated("\n\nDo you want to edit more members?") == 1);
	free(allMembers);
};
/*for staff use*/ void MI_staffAddMemberViaTxt() {
	char fileName[100];
	do {
		system("cls");
		G_DrawTrain();
		printf("< ADD MEMBER VIA TXT FILE >\n\n");
		printf("Enter File Name: ");
		scanf("%s", &fileName);
		rewind(stdin);

		MI_AddMemberViaTxtFileProcess(fileName);


	} while (G_ConfirmationIsValidated("Do you want to add more members via txt file?"));
};
/*for admin use*/ int MI_NewMemberIsValidated(const ValidateTxt* member) {
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
	if (!(strlen(member->memberJoinDate) <= 10 && strlen(member->memberJoinDate) >= 8)) {
		return 0;
	}
	else {
		char tempDay[3] = "", tempMonth[3] = "", tempYear[5] = "";
		sscanf(member->memberJoinDate, "%s %s %s", tempDay, tempMonth, tempYear);
		//check length of date
		if (!((strlen(tempYear) == 4) && (strlen(tempMonth) <= 2) && (strlen(tempDay) <= 2))) {
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
	for (int i = 0; member->memberWallet[i]; i++) {
		if (!isdigit(member->memberWallet[i])) {
			if (member->memberWallet[i] == '.') {
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
/*for staff use*/ void MI_AddMemberViaTxtFileProcess(const char* fileName) {
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

	//check file format
	char eachLine[257] = "";
	while (fscanf(fptr, "%[^\n]\n", &eachLine) != EOF) {
		int delimiter = 0;
		for (int i = 0; i < strlen(eachLine); i++) {
			if (eachLine[i] == '|') {
				delimiter++;
				if (delimiter > 6) {
					printf("\n\nFile is in wrong format! Error.\n\n");
					return;
				}
			}
		}
		if (delimiter != 6) {
			printf("\n\nFile is in wrong format! Error.\n\n");
			return;
		}
	}

	//reset pointer if no format problem
	rewind(fptr);
	while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n",
		&validate.memberID, &validate.memberPass, &validate.memberName, &validate.memberPhoneNo,
		&validate.memberJoinDate, &validate.memberRewards, &validate.memberWallet) == 7) {
		// Validation
		if (MI_NewMemberIsValidated(&validate)) {
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
	MI_displayAllMembers();
	fclose(fptr);
	fclose(fptr2);
}
/*for staff use*/ void MI_ConvertBinToTxt() {

	char fileName[100];
	do {
		int numOfMembers = MI_getNumberOfMembers();
		Member* allMembers = MI_getMemberDetails(numOfMembers);
		system("cls");
		G_DrawTrain();
		printf("< CONVERT BIN TO TXT FILE >\n\n");
		printf("Enter new txt file name: ");
		scanf("%s", &fileName);
		rewind(stdin);

		FILE* fptr = fopen(fileName, "w");

		for (int i = 0; i < numOfMembers; i++) {
			fprintf(fptr, "%s|%s|%s|%s|%d %d %d|%d|%.2lf\n", allMembers[i].memberID, allMembers[i].memberPass, allMembers[i].memberName,
				allMembers[i].memberPhoneNo, allMembers[i].memberJoinDate.day, allMembers[i].memberJoinDate.month, allMembers[i].memberJoinDate.year,
				allMembers[i].memberRewards, allMembers[i].memberWallet);
		}

		printf("%d member(s) has been saved into the new txt file \"%s\".\n\n", numOfMembers, fileName);
		free(allMembers);
		fclose(fptr);
	} while (G_ConfirmationIsValidated("Do you want to convert binary file to more txt file?"));

};

//System Menu
int G_systemMenu()
{
	
	int inputIsError = 0;
	int systemMenuDecision;
	do {
		if (inputIsError == ISERROR) exit(ISERROR);
		system("cls");
		G_DrawTrain();
		printf("Welcome to Xpress Railway\n\n");
		printf("Choose your mode: \n");
		printf("1. Member Login\n");
		printf("2. Member Register\n");
		printf("3. Staff Login\n\n");
	} while (inputIsError =
		G_IntIsValidated("Mode", 3, &systemMenuDecision));

	switch (systemMenuDecision)
	{
	case 1:
		MI_memberLogin();
		break;
	case 2:
		MI_registerMember();
		break;
	case 3:
		//create a staff login - get from SI module
		while (SI_mainMenu());
	}
	return 1;
}
int SI_mainMenu() {
	int staffInformationDecision;
	int inputIsError = 0;
	do {
		if (inputIsError == ISERROR) return 0;
		system("cls");
		G_DrawTrain();
		printf("\n\t< Staff Information >\n");
		G_lineDesign();
		printf("1. Staff Account Management\n");
		printf("2. Member Details Management\n");
		printf("3. Train Schedule Management\n");
		G_lineDesign();
	} while (inputIsError =
		G_IntIsValidated("Mode", 3, &staffInformationDecision));
	switch (staffInformationDecision) {
	case 1:
		while (G_staffAccountManagementMenu());
		break;
	case 2:
		while (MI_mainMenuForStaff());
		break;
	case 3:
		while (TS_mainMenu());
		break;
	}
	return 1;
}
int TS_mainMenu() {
	int trainScheduleDecision = 1;
	int inputIsError = 0;
	do
	{
		if (inputIsError == ISERROR) return 0;
		system("cls");
		G_DrawTrain();
		printf("=====================================================\n");
		printf("1.Show train schedule\n");
		printf("2.Add train schedule\n");
		printf("3.Search train schedule\n");
		printf("4.Delete schedule\n");
		printf("5.Modify train schedule\n\n");
	} while (inputIsError =
		G_IntIsValidated("Mode", 5, &trainScheduleDecision));
	switch (trainScheduleDecision) {
	case 1:
		TS_displaySchedule();
		break;
	case 2:
		TS_addSchedule();
		break;
	case 3:
		TS_searchSchedule();
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
int TB_mainMenu() {
	int inputIsError = 1;
	int ticketBookingDecision;
	//function pointer with length of 3
	void (*bookingRouter[3])(TrainSchedule trainSchedule) =
	{ TB_BookingTicket,TB_CancelBooking,TB_EditBooking };

	const char* bookingMenu[] =
	{
		"Display Booking History",
		"Booking Ticket",
		"Cancel Booking",
		"Modified Booking Details"
	};
	//if isAdmin = 1 the range will be 4, else it will be 3; member is restricted to access the modification
	do
	{
		if (inputIsError == ISERROR) return 0;

		system("cls");

		for (int i = 0; i < (3 + isAdmin); i++)
		{
			printf("%d. %s\n", i + 1, bookingMenu[i]);
		}
	} while (inputIsError = G_IntIsValidated("Mode", 3 + isAdmin, &ticketBookingDecision));
	//get member details
	//if (isAdmin)

	//get train details
	TrainSchedule* trainSchedule = TS_GetTrainSchedule(NULL);
	if (trainSchedule == NULL) {
		return 0;
	}
	if (ticketBookingDecision == 1) {
		TB_DisplayBookingRecord(
			G_ConfirmationIsValidated("Do you want to search all result?")
			?
			"ALL"
			:
			trainSchedule->trainID
		);
		return 1;
	}
	//decision - 1 + 1
	bookingRouter[ticketBookingDecision](*trainSchedule);
	return 1;
}
int MI_mainMenu() {
	int staffInformationDecision;
	int inputIsError = 0;
	do {
		if (inputIsError == ISERROR) return 0;

		system("cls");
		G_DrawTrain();
		printf("Current member: %s\n\n", loggedInMember.memberID);
		printf("Choose your mode: \n");
		printf("1. Member Account Management\n");
		printf("2. Booking Management\n");
		printf("3. View Train Schedule\n\n");

	} while (inputIsError =
		G_IntIsValidated("Mode", 3, &staffInformationDecision));

	switch (staffInformationDecision) {
	case 1:
		while (G_memberAccountManagementMenu());
		break;
	case 2:
		while (TB_mainMenu());
		break;
	case 3:
		TS_displaySchedule();
		break;
	}
	return 1;
}
int MI_mainMenuForStaff() {
	int mainMenuForStaffDecision;
	int inputIsError = 0;
	do {
		if (inputIsError == ISERROR) return 0;

		system("cls");
		G_DrawTrain();
		printf("Choose your mode: \n");
		printf("1. View Member Account\n");
		printf("2. Search Member Account\n");
		printf("3. Register New Member Account\n");
		printf("4. Register New Member Account Via txt File\n");
		printf("5. Save Member Accounts Into New txt File\n");
		printf("6. Edit Member Account\n");
		printf("7. Delete Member Account\n\n");
	} while (inputIsError =
		G_IntIsValidated("Mode", 7, &mainMenuForStaffDecision));

	switch (mainMenuForStaffDecision) {
	case 1:
		MI_displayAllMembers();
		break;
	case 2:
		MI_staffSearchMember();
		break;
	case 3:
		MI_staffAddMember();
		break;
	case 4:
		MI_staffAddMemberViaTxt();
		break;
	case 5:
		MI_ConvertBinToTxt();
		break;
	case 6:
		MI_staffEditMemberDetails();
		break;
	case 7:
		MI_staffDeleteMember();
		break;
	}
	return 1;
}
int G_memberDetailsManagementForStaffMenu() {
	//manage member details, booking details
	int memberDetailsManagementDecision;
	int inputIsError = 0;
	do {
		if (inputIsError == ISERROR) return 0;
		system("cls");
		G_DrawTrain();
		printf("1. Member\'s Details Management\n");
		printf("2. Member\'s Booking Management\n");
		G_lineDesign();
	} while (inputIsError =
		G_IntIsValidated("Mode", 2, &memberDetailsManagementDecision));
	switch (memberDetailsManagementDecision) {
	case 1:
		while (MI_mainMenuForStaff());
		break;
	case 2:
		while (TB_mainMenu());
		break;
	}
	return 1;
}
int G_staffAccountManagementMenu() {
	int staffAccountManagementMenuDecision;
	int inputIsError = 0;
	do {
		if (inputIsError == -1) return 0;
		system("cls");
		G_DrawTrain();
		printf("\n\t< Staff Information >\n");
		G_lineDesign();
		printf("  1. Add Staff\n");
		printf("  2. Search Staff\n");
		printf("  3. Modify Staff Details\n");
		printf("  4. Display Staff Details\n");
		printf("  5. Delete Staff\n");
		G_lineDesign();
	} while (inputIsError =
		G_IntIsValidated("Mode", 5, &staffAccountManagementMenuDecision));
	system("cls");
	switch (staffAccountManagementMenuDecision) {
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
int G_memberAccountManagementMenu() {
	int accountManagementMenuDecision;
	int inputIsError = 0;
	do {
		if (inputIsError == -1) {
			return 0;
		}
		system("cls");
		G_DrawTrain();
		printf("Welcome, %s\n\n\n", loggedInMember.memberName);
		MI_displayDetails(loggedInMember.memberID);
		printf("Choose your mode: \n");
		printf("1. Edit Account Details\n");
		printf("2. Top up Ewallet\n");
		printf("3. Delete Account\n\n"); // all related booking will be cancelled also
	} while (inputIsError =
		G_IntIsValidated("Mode", 3, &accountManagementMenuDecision));
	switch (accountManagementMenuDecision)
	{
	case 1:
		MI_MemberEditDetails(loggedInMember.memberID);
		break;
	case 2:
		MI_topUpWallet(loggedInMember.memberID);
		break;
	case 3:
		MI_memberDeleteAccount(loggedInMember.memberID);
		break;
	}
	return 1;
}

//additional function
//when train schedule is deleted, all related booking should be deleted
//when member account is deleted, all related booking should be deleted
//update the member edited details[updated eWallet balance, reward point] - get from MI module
//staff login - get from SI module
