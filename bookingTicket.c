#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <Windows.h>
#define ISERROR -1

//#define CANCELED 'O'
#pragma warning(disable:4996)

int isAdmin = 1;
char userID[10] = "M0001";
typedef char* string;
typedef int compareFunction(void* a, void* b);
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

typedef struct{
	char bookingID[10];
	char memberID[10]; 
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
}TrainSchedule;

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
} MI_Member;
// Function Declarations
//Global use
Date G_GetCurrentDate();
Time G_GetCurrentTime();
int G_CalDiffDate(Date currDate, Date dateToCmp);
int G_GetFileNumRow(string fileName, unsigned int dataSize);
int G_InArray(void* arr, void* value, int elementSize, int length, compareFunction compare);
int G_CompareInt(void* a, void* b);
int G_ContinueOrStopCRUDRecord(string CRUD);

//Read
void TB_DisplayBooking(string memberID, string searchFilter);
void TB_DisplayBookingHdr(Booking booking, double* totalPrice);
void TB_DisplayBookingDet(Booking booking, double* totalPrice, int count);
void TB_DisplayBookingFoot(double totalPrice);
string TB_DisplaySeatType(int seatType);

//Create
void TB_BookingTicket(string memberID);
Booking* TB_GetBookingDetails(int* length, string memberID);
Ticket TB_GetTicketDetails(int* notAvailableSeat, int lengthOfNotAvailableSeat);
int TB_GetTrainID();
void TB_DisplaySeat(int seatQuantity, int** notAvailableSeat, int lengthOfNotAvailableSeat);
int* TB_GetSeatAvailability(int* length, string trainID);
void TB_GetSeatDetails(int seatQuantity, Ticket* ticket, int** notAvailableSeat, int lengthOfNotAvailableSeat);
int TB_GetSeatType(int selectedSeatID, int numOfSeat[], int length);
double TB_GetTicketPrice(int seatType, Time startTime, Date departureDate);
void TB_GetPackageDetails(Booking* booking, int length, int* currentPoint);
int TB_CheckAccountBalance(double* accountBalance, double priceToPaid);


//Edit
void TB_EditBooking(string memberID);
int* TB_GetindexOfMemberBookingID(Booking* booking, int numRow, int* countMemberData, string memberID);
void TB_GetMemberID(string memberID);
int TB_GetBookingID(int numRow);

//Delete
void TB_DeleteBooking(string memberID);

//Function 
int main() {
	//Create data
	TB_BookingTicket(userID);
	//Read file
	TB_DisplayBooking(userID, "T0001");
	//TB_EditBooking();
	//TB_DeleteBooking(userID);
	//TB_DisplayBooking();
}


Date G_GetCurrentDate() {
	//printf("Audit Log : Date G_GetCurrentDate()\n");
	SYSTEMTIME getDate;
	GetLocalTime(&getDate);
	Date date = { getDate.wDay, getDate.wMonth, getDate.wYear };
	return date;
}
int G_CalDiffDate(Date currDate, Date dateToCmp) {
	//printf("Audit Log : int G_CalDiffDate(Date currDate, Date dateToCmp)\n");
	return (currDate.year <= dateToCmp.year)
		?
		abs(
			(currDate.year * 365 + currDate.month * 30 + currDate.day)
			-
			(dateToCmp.year * 365 + dateToCmp.month * 30 + dateToCmp.day)
		)
		:
		ISERROR;
}
Time G_GetCurrentTime() {
	//printf("Audit Log : Time G_GetCurrentTime()\n");
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);
	Time time = { localTime.wHour,localTime.wMinute };
	return time;
}
int G_GetFileNumRow(string fileName, unsigned int dataSize) {
	//printf("Audit Log : int G_GetFileNumRow(string fileName, unsigned int dataSize)\n");
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
int G_InArray(void* arr, void* value, int elementSize, int length, compareFunction compare) {
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
int G_ContinueOrStopCRUDRecord(string CRUD) {
	//printf("AuditLog : int G_ContinueOrStopCRUDRecord(char * CRUD)\n");
	char continueCRUDRecord;
	do
	{
		printf("Do you want to continue %s record?[Y/N] >>", CRUD);
		scanf("%c", &continueCRUDRecord);
		rewind(stdin);
		continueCRUDRecord = toupper(continueCRUDRecord);
	} while (!(continueCRUDRecord == 'Y' || continueCRUDRecord == 'N'));
	return continueCRUDRecord == 'Y';
}



//Read[Admin/Member]
void TB_DisplayBooking(string memberID, string searchFilter)
{
	//printf("Audit Log : void TB_DisplayBooking()\n");
	int currTrainID = 0;
	int trainID;
	int isChangeTrainID = 0;
	double totalPrice = 0;
	int count = 0;
	int length = G_GetFileNumRow("Booking.bin", sizeof(Booking));
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
		if (G_InArray(displayedTrainID, &trainID, sizeof(int), count, G_CompareInt))
		{
			i++;
			continue;
		}
		else
		{
			int* tempDisplayedTrainID = (int*)realloc(displayedTrainID, sizeof(int) * ++count);
			if (!tempDisplayedTrainID) {
				printf("Memory Allocation failed.");
				free(displayedTrainID);
				exit(ISERROR);
			}
			displayedTrainID = tempDisplayedTrainID;
			displayedTrainID[count - 1] = trainID;
		}
		//Header
		if (strncmp(searchFilter, booking[i].ticketDetails.trainID, 10) == 0) 
		{
			TB_DisplayBookingHdr(booking[i], &totalPrice);
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
				TB_DisplayBookingDet(booking[j], &totalPrice, num++);
			}
		}
		//schedule details get from train schedule
		currTrainID++;
	}
	system("pause");
	free(booking);
	free(displayedTrainID);
}
void TB_DisplayBookingHdr(Booking booking, double* totalPrice) 
{
	//printf("Audit Log : void TB_DisplayBookingHdr(Booking booking, int isChangeTrainID, double* totalPrice)\n");

	//get from train schedule
	Date temp_DepartureDate = G_GetCurrentDate();
	Time temp_startTime = G_GetCurrentTime();
	Time temp_endTime = G_GetCurrentTime();
	char departureStation[50] = "";
	char arrivalStation[50] = "";
	//departure|arrival station
	printf("%-15s %-20s\n%-15s %-50s ~ %-50s\n%-15s %02d/%02d/%04d \n%-10s [%02d:%02d~%02d:%02d]\n%-10s %02d/%02d/%04d\n",
		"TRAIN ID :",
		booking.ticketDetails.trainID,
		"STATION :",
		departureStation,
		arrivalStation,
		"DEPARTURE DATE :",
		temp_DepartureDate.day,
		temp_DepartureDate.month,
		temp_DepartureDate.year,
		"TIME : ",
		temp_startTime.hour,
		temp_startTime.min,
		temp_endTime.hour,
		temp_endTime.min,
		"BOOKING DATE :",
		temp_DepartureDate.day,
		temp_DepartureDate.month,
		temp_DepartureDate.year
	);

	printf("%-82s\n", "---------------------------------------------------------------------------------");
	printf("%-3s %-15s %-10s %-10s %-20s %-10s\n",
		"NO.", "BOOKING ID", "SEAT ID", "COACH", "SEAT TYPE", "PRICE");
	printf("%-3s %-15s %-10s %-10s %-20s %-10s\n",
		"---", "-----------", "----------", "------", "--------------", "------");
	*totalPrice = 0;
}
void TB_DisplayBookingDet(Booking booking, double* totalPrice,int count) 
{
	//printf("Audit Log : void TB_DisplayBookingDet(Booking booking, double* totalPrice,int count)\n");
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
void TB_DisplayBookingFoot(double totalPrice)
{
	//printf("Audit Log : void TB_DisplayBookingFoot(int isChangeTrainID, double totalPrice)\n");

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
void TB_EditBooking(string memberID) 
{
	printf("Audit Log : void TB_EditBooking()\n");
	Ticket newTicketDetails = { 0 };

	double priceDifference = 0;
	double accountBalance = 780.69;
	int selectedBooking = 0;
	
	int qty = 120;
	double totalPrice = 0;

	int countMemberData = 0;
	int* indexOfMemberBookingID = NULL;
	
	int lengthOfNotAvailableSeat = 0;
	int* notAvailableSeat = TB_GetSeatAvailability(&lengthOfNotAvailableSeat, "T0001");
	int preSeatID = 0, currSeatID = 0;

	int numRow = G_GetFileNumRow("Booking.bin", sizeof(Booking));
	
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
	indexOfMemberBookingID = TB_GetindexOfMemberBookingID(booking, numRow, &countMemberData, memberID);
	if (!indexOfMemberBookingID) 
	{
		printf("No User record.");
		return;
	}
	do
	{
		//Display User Booking details
		//Header
		TB_DisplayBookingHdr(booking[indexOfMemberBookingID[0]], &totalPrice);
		//Details
		for (int i = 0; i < countMemberData; TB_DisplayBookingDet(booking[indexOfMemberBookingID[i]], &totalPrice, i + 1), i++);
		//Footer
		TB_DisplayBookingFoot(totalPrice);

		selectedBooking = TB_GetBookingID(numRow);
		
		//Edit seat details
		TB_DisplaySeat(qty, &notAvailableSeat, lengthOfNotAvailableSeat);
		TB_GetSeatDetails(qty, &newTicketDetails, &notAvailableSeat, lengthOfNotAvailableSeat);
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
		//get from train schedule
	} while (G_ContinueOrStopCRUDRecord("edit"));
	free(indexOfMemberBookingID);
	if (TB_CheckAccountBalance(&accountBalance, priceDifference)) 
	{
		free(booking);
		return;
	}
	// tempdata get from member information

	//Update the newest data
	FILE* fptrForWrite = fopen("Booking.bin", "wb");
	if (!fptrForWrite) {
		printf("File unable to open.");
		exit(ISERROR);
	}

	fwrite(booking, sizeof(Booking), numRow, fptrForWrite);

	fclose(fptrForWrite);
	free(booking);

}
int* TB_GetindexOfMemberBookingID(Booking* booking, int numRow,int* countMemberData,string memberID) {
	printf("Audit Log : int* TB_GetindexOfMemberBookingID(Booking* booking, int numRow,int* countMemberData)\n");

	char selectedTrainID[10] = "\0";
	
	//TB_GetTrainID(selectedTrainID);
	int* indexOfMemberBookingID = NULL, * tempindexOfMemberBookingID = NULL;
	for (int i = 0; i < numRow; i++)
	{
		if (strncmp(booking[i].ticketDetails.trainID, "T0001", 10) ||
			strncmp(booking[i].memberID, memberID, 10) ||
			booking[i].isCancelled) {
			continue;
		}

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
void TB_GetMemberID(string memberID) {
	printf("Audit Log : void TB_GetMemberID(string memberID)\n");
	//check from member information if the data exists
	char tempMemberID[10] = "\0";
	do
	{
		printf("Enter the member ID :");
		scanf("%[^\n]", &tempMemberID);
		rewind(stdin);
	} while (strcmp(tempMemberID, "\0") == 0);
	strncpy(memberID, tempMemberID, 10);
}
int TB_GetBookingID(int numRow) {
	printf("Audit Log : int TB_GetBookingID(int numRow)\n");
	int selectedBooking;
	do
	{
		printf("Select the booking ID >>");
		scanf("%d", &selectedBooking);
		rewind(stdin);
	} while (selectedBooking <= 0 || selectedBooking > numRow);
	return selectedBooking;
}



//Delete[Admin/Member]
void TB_DeleteBooking(string memberID) 
{
	printf("Audit Log : void TB_DeleteBooking(string memberID)\n");
	double totalPrice = 0;

	int countMemberData = 0;
	int* indexOfMemberBookingID = NULL;
	int* dataToBeDeleted = NULL;
	int* tempDataToBeDeleted = NULL;
	int countSelectedData = 0;

	int numRow = G_GetFileNumRow("Booking.bin", sizeof(Booking));

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
	indexOfMemberBookingID = TB_GetindexOfMemberBookingID(booking, numRow, &countMemberData, memberID);
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
		//Header
		TB_DisplayBookingHdr(booking[indexOfMemberBookingID[0]], &totalPrice);
		//Details
		for (int i = 0,count = 0; i < countMemberData; i++)
		{
			if (!G_InArray(dataToBeDeleted, &indexOfMemberBookingID[i],
				sizeof(int), countMemberData, G_CompareInt) && !booking[indexOfMemberBookingID[i]].isCancelled)
			{
				TB_DisplayBookingDet(booking[indexOfMemberBookingID[i]], &totalPrice, ++count);
			}
		}
		//Footer
		TB_DisplayBookingFoot(totalPrice);

		dataToBeDeleted[countSelectedData++] = indexOfMemberBookingID[TB_GetBookingID(numRow) - 1];

		//get from train schedule
	} while (G_ContinueOrStopCRUDRecord("delete"));
	
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
			printf("%d ", booking[indexOfMemberBookingID[i]].isCancelled);
		}
		printf("%d. %d ", i, dataToBeDeleted[i]);
	}
	for (int i = 0; i < countMemberData; i++)
	{
		printf("%d. %d ", i, indexOfMemberBookingID[i]);
	}
	fwrite(booking, sizeof(Booking), numRow, fptrForDelete);
	free(dataToBeDeleted);
	fclose(fptrForDelete);
}



//Create[Member]
void TB_BookingTicket(string memberID)
{
	//printf("AuditLog : void TB_BookingTicket()\n");
	int length = 0;
	Booking* booking = TB_GetBookingDetails(&length, memberID);
	double totalPrice;
	char confirmAddRecord;
	int currentPoint = 500;
	double accountBalance = 700.69;
	FILE* fptr = fopen("Booking.bin", "ab");
	if (!fptr)
	{
		printf("Error when opening file.");
		exit(ISERROR);
	}
	//Display package
	TB_GetPackageDetails(booking, length, &currentPoint); //temp data get from member informaiton

	//Display User Booked Ticket
	//Header
	TB_DisplayBookingHdr(booking[0], &totalPrice);
	//Details
	for (int i = 1; i <= length; i++) TB_DisplayBookingDet(booking[i - 1], &totalPrice, i);
	//Footer
	TB_DisplayBookingFoot(totalPrice);

	//Booking Confirmation check with member's eWallet's balance 
	if (!TB_CheckAccountBalance(&accountBalance, totalPrice)) //temp data get from member informaiton
	{
		fclose(fptr);
		free(booking);
		return;
	}
	do {
		printf("Booking Confirmation[Y/N] >>");
		scanf("%c", &confirmAddRecord);
		rewind(stdin);
		
		confirmAddRecord = toupper(confirmAddRecord);
	} while (!(confirmAddRecord == 'Y' || confirmAddRecord == 'N'));

	if (confirmAddRecord == 'Y') {
		//check if the amount is enough
		fwrite(booking, sizeof(Booking), length, fptr);
		//update the newest Ewallet amount of the member
	}

	fclose(fptr);
	free(booking);
}
Booking* TB_GetBookingDetails(int* length,string memberID) {
	//printf("AuditLog : Booking* TB_GetBookingDetails(int* length,string memberID)\n");
	int lengthOfBooking = 0;
	int lengthOfNotAvailableSeat = 0;
	int currPointedIndex;
	int seatID = 0;
	int numRow = G_GetFileNumRow("Booking.bin", sizeof(Booking));
	int* notAvailableSeat = TB_GetSeatAvailability(&lengthOfNotAvailableSeat, "T0001");
	Booking* booking = NULL;

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
		strcpy((booking + currPointedIndex)->memberID, memberID);
		(booking + currPointedIndex)->bookingDate = G_GetCurrentDate();
		(booking + currPointedIndex)->ticketDetails
			= TB_GetTicketDetails(notAvailableSeat, lengthOfNotAvailableSeat + lengthOfBooking);
		(booking + currPointedIndex)->isCancelled = 0;

		sscanf((booking + currPointedIndex)->ticketDetails.seatID, "S%d", &seatID);
		notAvailableSeat[lengthOfNotAvailableSeat + currPointedIndex] = seatID;

	} while (G_ContinueOrStopCRUDRecord("add"));
	
	*length = lengthOfBooking;
	free(notAvailableSeat);
	return booking;
}
Ticket TB_GetTicketDetails(int* notAvailableSeat, int lengthOfNotAvailableSeat)
{
	//printf("AuditLog : Ticket TB_GetTicketDetails()\n");
	// get details from train schedule
	Ticket ticketDetails = { 0 };
	
	int qty = 120;

	TB_DisplaySeat(qty, &notAvailableSeat, lengthOfNotAvailableSeat);
	TB_GetSeatDetails(qty, &ticketDetails, &notAvailableSeat, lengthOfNotAvailableSeat);
	
	return ticketDetails;
}
int TB_GetTrainID()
{
	//printf("AuditLog : int TB_GetTrainID()\n");

	TrainSchedule trainSchedule = { 0 };
	int selectedTrainID;
	int count = 0;
	FILE* fptr = fopen("Schedule.txt","r");
	if (!fptr) {
		printf("File unable open.\n");
		exit(ISERROR);
	}
	printf("%-3s %-20s %-20s %-20s %-20s\n","NO.", "Departure Station", "Arrival Station", "Departure Date", "Duration Time");
	printf("%-3s %-20s %-20s %-20s %-20s\n","---","---------------","-------------","--------------","-------------");
	while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%d/%d/%d|%d:%d|%d:%d|%d\n",
		&trainSchedule.trainID, &trainSchedule.departureStation, &trainSchedule.arrivalStation,
		&trainSchedule.departureDate.day, &trainSchedule.departureDate.month, &trainSchedule.departureDate.year,
		&trainSchedule.departureTime.hour, &trainSchedule.departureTime.min,
		&trainSchedule.arrivalTime.hour, &trainSchedule.arrivalTime.min,
		&trainSchedule.availableSeat) != EOF) {
		printf("%02d. %-20s %-20s %02d/%02d/%04d %d:%d ~ %d:%d\n", ++count,
			trainSchedule.departureStation, trainSchedule.arrivalStation,
			trainSchedule.departureDate.day, trainSchedule.departureDate.month, trainSchedule.departureDate.year,
			trainSchedule.departureTime.hour, trainSchedule.departureTime.min,
			trainSchedule.arrivalTime.hour, trainSchedule.arrivalTime.min);
	}
	//might similiar as trainSchedule module, rmb to make the change
	
	do
	{
		printf("Select The train Schedule [ENTER '0' to escape] >>");
		scanf("%d", &selectedTrainID);
		rewind(stdin);
	} while (selectedTrainID < 0 || selectedTrainID > count);
	fclose(fptr);
	return selectedTrainID;
}
void TB_DisplaySeat(int seatQuantity, int** notAvailableSeat, int lengthOfNotAvailableSeat) {
	// 50% Standard[1] : 20% Premium[2] : 20% Sleeping Berths[3] : 10% Special Needs Seats[4](For Child/Seniors)

	//each coach can only contain 40 seat(col-10, row-4)
	//number of coach = seatQuantity / 40
	int seat = 0;
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
				seat = row + 1 + (col * 10) + (coach * 40);
				printf("| ");
				for (int i = 0; i < sizeof(displayNumOfSeat)/sizeof(displayNumOfSeat[0]); i++)
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
	for (int i = 0; i < sizeof(color)/sizeof(color[0]) - 1; i++)
	{
		printf("%s000\033[0m - %s\n", color[i], TB_DisplaySeatType(i + 1));
	}	
}
void TB_GetSeatDetails(int seatQuantity, Ticket* ticket, int** notAvailableSeat, int lengthOfNotAvailableSeat) {
	int selectedSeatID = 0;
	char seatID[10] = "";
	int inputNoError = 0;
	int	numOfSeat[4] =
	{
		(int)ceil(seatQuantity * 0.5),
		(int)floor(seatQuantity * 0.2),
		(int)floor(seatQuantity * 0.2),
		(int)ceil(seatQuantity * 0.1)
	};

	while (!inputNoError)
	{
		inputNoError = 1;
		printf("Select your Seat ID >>");
		scanf("%d", &selectedSeatID);
		rewind(stdin);
		if (G_InArray(*notAvailableSeat, &selectedSeatID, sizeof(int), lengthOfNotAvailableSeat, G_CompareInt)
			||
			(0 <= selectedSeatID && selectedSeatID > seatQuantity)
			) {
			printf("Seat Unavailable.\n");
			inputNoError = 0;
		}
	}
	
	sprintf(seatID, "S%04d", selectedSeatID);
	strcpy(ticket->seatID, seatID);
	strncpy(ticket->trainID, "T0001", 10);
	ticket->coach = (char)((int)'A' + (int)floor(selectedSeatID / 40.0));
	ticket->seatType = TB_GetSeatType(selectedSeatID, numOfSeat, sizeof(numOfSeat) / sizeof(numOfSeat[0]));
	ticket->price = TB_GetTicketPrice(ticket->seatType, G_GetCurrentTime(), G_GetCurrentDate());
}
int* TB_GetSeatAvailability(int *length,string trainID) {
	//printf("AuditLog : int TB_GetSeatAvailability(int seatID)\n");
	int* notAvailableSeat = NULL; // Initialize to NULL	
	int seatNo = 0;
	int seatAvailability = 0;
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
	//printf("AuditLog : int TB_GetSeatType(int selectedSeatID, int numOfSeat[], int length)\n");
	for (int i = 1; i <= length; i++)
	{
		if ((selectedSeatID -= numOfSeat[i - 1]) - 1 <= 0)
		{
			return i;
		}
	}
	return ISERROR;
}
double TB_GetTicketPrice(int seatType, Time startTime,Date departureDate) {
	//printf("AuditLog : double TB_GetTicketPrice(int seatType, Time startTime,Date departureDate)\n");
	double seatTypeRate[4] = { 1.0,1.1,1.2,0.8 };

	Date currDate = { 12,4,2024 };

	int diffDate = G_CalDiffDate(currDate, departureDate);
	if (diffDate == ISERROR) {
		printf("This date can\'t be selected.\n");
		return -1;
	}

	//50% Standard[1] offer 100% : 20% Premium[2] offer 110%: 20% Sleeping Berths[3] offer 120%: 10% Special Needs Seats[4](For Child/Seniors) offer 80%
	//Ticket price based on time[after 1730 inc price 20%]
	//if get closer to departure date, price higher
	
	//Ticket Price = Base Price * SeatType[Rate] * DiffDate[Rate] * startTime[Rate]
	return (double)(5.0 *
		seatTypeRate[seatType - 1] * 
		((diffDate > 30) ? 2.0 : 1.0) * 
		((startTime.hour * 60 + startTime.min) > 1050 ? 1.2 : 1.0)); 
}
void TB_GetPackageDetails(Booking* booking, int length, int* currentPoint) {
	char confirmUsePoint = 'N';
	int selectedPackage = 1;
	double highestPrice = 0;
	Package package[3] =
	{
		{0.9 , 50, "10% offers", 1, 1},
		{0.5 , 250, "50% offers", 2, 2},
		{0.0 , 450, "100% offers", 3, 5}
	};

	do
	{
		printf("Do you want to use Member Point[Y/N] >>");
		scanf("%c", &confirmUsePoint);
		rewind(stdin);
		confirmUsePoint = toupper(confirmUsePoint);
	} while (!(confirmUsePoint == 'Y' || confirmUsePoint == 'N'));

	if (confirmUsePoint != 'Y') {
		return;
	}
	printf("Current ticket booked : %d\nCurrent Point : %dP\n", length, *currentPoint);
	for (int i = 0; i < 3 && i < length; i++)
	{
		printf("Package %d : %s for all tickets! Only require %dPs\n"
			, i + 1, package[i].desc, package[i].pointRequired);
	}

	do
	{
		printf("Select the package you want to exchange >>");
		scanf("%d", &selectedPackage);
		rewind(stdin);
		if (length < package[selectedPackage - 1].minRequired) {
			printf("This Package required at least %d of ticket in the booking.\n",
				package[selectedPackage - 1].minRequired);
			continue;
		}
		if (*currentPoint < package[selectedPackage - 1].pointRequired) {
			printf("Your Point is not enough to exchange this package.\n");
			continue;
		}
	} while (selectedPackage < 0 && selectedPackage > 3);

	for (int i = 0; i < package[selectedPackage - 1].maxLimits && i < length; i++)
	{
		booking[i].ticketDetails.price *= package[selectedPackage - 1].discount;
	}
	*currentPoint -= package[selectedPackage - 1].pointRequired;
	printf("Selected Package : [Package %d : %s, with %d of Points]\n",
		selectedPackage, package[selectedPackage - 1].desc, package[selectedPackage - 1].pointRequired);
}
int TB_CheckAccountBalance(double* accountBalance, double priceToPaid) {
	while (*accountBalance < priceToPaid) {
		char confirmTopUpBalance = 'N';
		printf("Current Balance : %.2f", *accountBalance);
		do
		{
			printf("Balance is not enough to purchase the booking, do you want to top up or cancel the booking ? [Y/N]");
			scanf("%c", &confirmTopUpBalance);
			rewind(stdin);
		} while (!(confirmTopUpBalance == 'Y' || confirmTopUpBalance == 'N'));
		if (confirmTopUpBalance != 'Y') {
			return 0;
		}
		//redirect user to top up balance
	}
	*accountBalance -= priceToPaid;
	return 1;
}
//get full member information and attract current balance and point based on passed member ID