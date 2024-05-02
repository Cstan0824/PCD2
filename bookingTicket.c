#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <Windows.h>
#define ISERROR -1
#pragma warning(disable:4996)

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

typedef struct{
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

extern int isAdmin = 0;
extern Member loggedInMember = { 0 };

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

int TB_MainMenu();

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
int G_GetTxtFileNumRow(string fileName);

//Function 
int main() {
	while (TB_MainMenu());
	return 0;
}

int TB_MainMenu() {
	int inputIsError = 1;
	int decision;
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
		if (inputIsError == -1)	return 0;
		
		system("cls");
		
		for (int i = 0; i < (3 + isAdmin); i++)
		{
			printf("%d. %s\n", i + 1, bookingMenu[i]);
		}
	} while (inputIsError = G_IntIsValidated("Select The train Schedule", 3 + isAdmin, &decision));
	//get member details
	//if (isAdmin)
	
	//get train details
	TrainSchedule* trainSchedule = TS_GetTrainSchedule(NULL);
	if (trainSchedule == NULL) {
		return 0;
	}
	if (decision == 1) {
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
	bookingRouter[decision](*trainSchedule);
	return 1;
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
Time G_GetCurrentTime() {
	//printf("Audit Log : Time G_GetCurrentTime()\n");
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);
	Time time = { localTime.wHour,localTime.wMinute };
	return time;
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
void TB_DisplayBookingRecordDet(Booking booking, double* totalPrice,int count) 
{
	//printf("Audit Log : void TB_DisplayBookingRecordDet(Booking booking, double* totalPrice,int count)\n");
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
		}while (inputIsError = G_IntIsValidated("Select The Booking ID", numRow, &selectedBooking));
		
		
		
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
int* TB_GetindexOfMemberBookingID(Booking* booking, int numRow,int* countMemberData, string trainID) {
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
int* TB_GetSeatAvailability(int *length,string trainID) {
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
double TB_GetTicketPrice(int seatType, Time startTime,Date departureDate) {
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
	TrainSchedule *trainSchedule = (TrainSchedule *)malloc(sizeof(TrainSchedule) * numRow);
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
			&tempTrainSchedule.isCancelled) != EOF;  )
			
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