#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <Windows.h>
#define ISERROR -1
//#define CANCELED 'O'
#pragma warning(disable:4996)

int isAdmin;
char userID[10];
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
	char bookingID[10]; //auto generate 
	char memberID[10]; //get from system
	Date bookingDate; 
	Ticket ticketDetails;
}Booking;
//char isCancelled
typedef struct {
	char trainID[10];
	char departureStation[50];
	char arrivalStation[50];
	Date departureDate;
	Time departureTime;
	Time arrivalTime;
	int availableSeat;
}TrainSchedule;


// Function Declarations
//Global use
Date G_GetCurrentDate();
Time G_GetCurrentTime();
int G_CalDiffDate(Date currDate, Date dateToCmp);

//Read
void TB_DisplayBooking();
void TB_DisplayBookingHdr(Booking booking, int isChangeTrainID, double* totalPrice);
void TB_DisplayBookingDet(Booking booking, double* totalPrice,int count);
void TB_DisplayBookingFoot(int isChangeTrainID, double totalPrice);
string TB_DisplaySeatType(int seatType);

//Create
void TB_BookingTicket();
Booking* TB_GetBookingDetails(int* length, string memberID);
Ticket TB_GetTicketDetails();
int TB_ContinueOrStopCRUDRecord(char* CRUD);
void TB_GetTrainID(string trainID);
void TB_DisplaySeat(int seatQuantity);
int TB_SeatAvailability(int seatID);
void TB_GetSeatDetails(int seatQuantity, int* selectedSeatID, int* selectedSeatType, string selectedCoach);
int TB_GetSeatType(int selectedSeatID, int numOfSeat[], int length);
double TB_GetTicketPrice(int seatType, Time startTime, Date departureDate);

//Edit
void TB_EditBooking();
int* TB_GetindexOfMemberBookingID(Booking* booking, int numRow, int* countMemberData);
void TB_GetMemberID(string memberID);
int TB_GetBookingID(int numRow);

//Delete
void TB_DeleteBooking(string memberID);

//Function 
int main() {
	//Create data
	TB_BookingTicket();

	//Read file
	TB_DisplayBooking();
	//Modify data
	//Delete data
	return 0;
}

Date G_GetCurrentDate() {
	SYSTEMTIME getDate;
	GetLocalTime(&getDate);
	Date date = { getDate.wDay, getDate.wMonth, getDate.wYear };
	return date;
}
int G_CalDiffDate(Date currDate, Date dateToCmp) {
	return (currDate.year > dateToCmp.year)
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
	SYSTEMTIME localTime;
	GetLocalTime(&localTime);
	Time time = { localTime.wHour,localTime.wMinute };
	return time;
}
int G_GetFileNumRow(string fileName, unsigned int dataSize) {
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
	char* tempArr = (char*)arr;
	for (int i = 0; i < length; i++)
	{
		void* pointedAddress = tempArr + i * elementSize;
		if (compare(pointedAddress, &value)) {
			return 1;
		}
	}
	return 0;
}
G_CompareInt(void* a, void* b) {
	return *(int*)a == *(int*)b;
}

//Read[Admin/Member]
//user can choose either display all booking or only for certain trainID(search filter)
void TB_DisplayBooking() {
	char currTrainID[10] = "\0";
	int isChangeTrainID = 0;
	double totalPrice = 0;
	Booking booking;

	FILE* fptr = fopen("Booking.dat", "rb");
	if (!fptr) {
		printf("Error when opening file.");
		exit(ISERROR);
	}
	for (int i = 1; fread(&booking, sizeof(Booking), 1, fptr) != 0; i++)
	{
		//schedule details get from train schedule
		//Footer
		TB_DisplayBookingFoot(isChangeTrainID, totalPrice);
		isChangeTrainID = strcmp(currTrainID, booking.ticketDetails.trainID);

		if (isChangeTrainID) strcpy(currTrainID, booking.ticketDetails.trainID);

		//Header
		TB_DisplayBookingHdr(booking, isChangeTrainID, &totalPrice);

		//Details
		TB_DisplayBookingDet(booking, &totalPrice, i);
	}
	fclose(fptr);
}
void TB_DisplayBookingHdr(Booking booking, int isChangeTrainID, double* totalPrice) 
{
	if (!isChangeTrainID) {
		return;
	}
	//get from train schedule
	Date temp_DepartureDate = G_GetCurrentDate();
	Time temp_startTime = G_GetCurrentTime();
	Time temp_endTime = G_GetCurrentTime();
	char departureStation[50] = "";
	char arrivalStation[50] = "";
	//departure|arrival station
	printf("%-15s %-20s\n%-15s %-50s ~ %-50s\n%-15s %-02d/%-02d/%-04d \n%-10s [ %-02d:%-02d ~ %-02d:%-02d ]\n",
		"TRAIN ID :",
		booking.ticketDetails.trainID,
		"STATION :",
		departureStation,
		arrivalStation,
		"DEPARTURE DATE :",
		temp_DepartureDate.day,
		temp_DepartureDate.month,
		temp_DepartureDate.year,
		"TIME",
		temp_startTime.hour,
		temp_startTime.min,
		temp_endTime.hour,
		temp_endTime.min
	);

	printf("%-82s\n", "---------------------------------------------------------------------------------");
	printf("%-3s %-11s %-6s %-10s %-10s %-15s %-6s\n",
		"NO.", "BOOKING ID", "SEAT ID", "COACH", "SEAT TYPE", "BOOKING DATE", "PRICE");
	printf("%-3s %-11s %-10s %-6s %-10s  %-15s %-6s\n",
		"---", "-----------", "----------", "------", "----------", "---------------", "------");
	*totalPrice = 0;
}
void TB_DisplayBookingDet(Booking booking, double* totalPrice,int count) 
{
	*totalPrice += booking.ticketDetails.price;
	printf("%02d. %-10s %-10s %c %-20s %.2f %-02d/%-02d/%-04d\n",
		count,
		booking.bookingID,
		booking.ticketDetails.seatID,
		booking.ticketDetails.coach,
		TB_DisplaySeatType(booking.ticketDetails.seatType),
		booking.ticketDetails.price,
		booking.bookingDate.day,
		booking.bookingDate.month,
		booking.bookingDate.year
	);
}
void TB_DisplayBookingFoot(int isChangeTrainID, double totalPrice)
{
	if (!isChangeTrainID) {
		return;
	}
	printf("%-82s\n", "---------------------------------------------------------------------------------");
	printf("%s RM%.2f", "TOTAL PRICE :", totalPrice);
	system("pause");
}
string TB_DisplaySeatType(int seatType) {
	// 50% Standard[1] : 20% Premium[2] : 20% Sleeping Berths[3] : 10% Special Needs Seats[4](For Child/Seniors)
	switch (seatType)
	{
	case 1: return "Standard"; break;
	case 2: return "Premium"; break;
	case 3: return "Sleeping Berths"; break;
	case 4: return "Special Needs"; break;
	default: return "Standard"; break;
	}
}

//Update[Admin]
//admin can edit user booking details based on member ID[user input] and trainID/bookingID[selection list](search filter)
//only able to change seatID update the newest price[updated price * 1.1], seat Type, and coach
void TB_EditBooking() 
{
	int selectedBooking = 0;
	
	int seatID;
	int qty = 120;
	double totalPrice = 0;

	int countMemberData = 0;
	int* indexOfMemberBookingID;
	
	int numRow = G_GetFileNumRow("Booking.dat", sizeof(Booking));

	Booking tempBooking = { 0 };
	Booking* booking = (Booking*)malloc(sizeof(Booking) * numRow);
	if (!booking) {
		printf("Memory allocated failed.");
		exit(ISERROR);
	}
	
	//get all the data from the file
	FILE* fptrForRead = fopen("Booking.dat", "rb");
	if (!fptrForRead) {
		printf("Error when opening file.");
		exit(ISERROR);
	}
	fread(booking, sizeof(Booking), numRow, fptrForRead);
	fclose(fptrForRead);

	indexOfMemberBookingID = TB_GetindexOfMemberBookingID(booking, numRow, &countMemberData);
	if (!indexOfMemberBookingID) 
	{
		printf("No User record.");
		return;
	}
	do
	{
		system("cls");
		//Display User Booking details
		//Header
		TB_DisplayBookingHdr(booking[indexOfMemberBookingID[0]], 1, &totalPrice);
		//Details
		for (int i = 1; i <= countMemberData; TB_DisplayBookingDet(booking[indexOfMemberBookingID[i - 1]], &totalPrice, i), i++);
		//Footer
		TB_DisplayBookingFoot(1, totalPrice);

		selectedBooking = TB_GetBookingID(numRow);

		//Edit seat details
		TB_DisplaySeat(qty);
		TB_GetSeatDetails(qty, &seatID,
			booking[indexOfMemberBookingID[selectedBooking - 1]].ticketDetails.seatType,
			booking[indexOfMemberBookingID[selectedBooking - 1]].ticketDetails.coach);
		sprintf(booking[indexOfMemberBookingID[selectedBooking - 1]].ticketDetails.seatID, "S%04d", seatID);
		//Updated Price *= penalty rate[120%]
		booking[indexOfMemberBookingID[selectedBooking - 1]].ticketDetails.price = 1.2 *
			TB_GetTicketPrice(booking[indexOfMemberBookingID[selectedBooking - 1]].ticketDetails.seatType,
				G_GetCurrentTime(), G_GetCurrentDate());
		//get from train schedule
	} while (TB_ContinueOrStopCRUDRecord("edit"));
	
	//Update the newest data
	FILE* fptrForWrite = fopen("Booking.dat", "wb");
	fwrite(booking, sizeof(Booking), numRow, fptrForWrite);
	fclose(fptrForWrite);

	//Free memory
	free(indexOfMemberBookingID);
	free(booking);
}
int* TB_GetindexOfMemberBookingID(Booking* booking, int numRow,int* countMemberData) {
	char selectedMemberID[10] = "\0";
	char selectedTrainID[10] = "\0";
	
	TB_GetMemberID(selectedMemberID);
	TB_GetTrainID(selectedTrainID);
	int* indexOfMemberBookingID = NULL, * tempindexOfMemberBookingID = NULL;
	for (int i = 0; i < numRow; i++)
	{
		if (strncmp(booking[i].ticketDetails.trainID, selectedTrainID, 10) ||
			strncmp(booking[i].memberID, selectedMemberID, 10)) {
			continue;
		}

		tempindexOfMemberBookingID = (int*)realloc(indexOfMemberBookingID, sizeof(int) * ++(*countMemberData));
		if (!tempindexOfMemberBookingID) {
			printf("Memory Allocated failed.");
			free(tempindexOfMemberBookingID);
			free(indexOfMemberBookingID);
			return NULL;
		}
		indexOfMemberBookingID = tempindexOfMemberBookingID;
		indexOfMemberBookingID[*countMemberData - 1] = i;
	}
	if (!indexOfMemberBookingID) {
		printf("User dont have booking record.");
		free(tempindexOfMemberBookingID);
		free(indexOfMemberBookingID);
		return NULL;
	}
	return indexOfMemberBookingID;
}
void TB_GetMemberID(string memberID) {
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
	int selectedBooking;
	do
	{
		printf("Select the booking ID >>");
		scanf("%d", &selectedBooking);
	} while (selectedBooking <= 0 || selectedBooking > numRow);
	return selectedBooking;
}

//Delete[Admin/Member]
//Able to cancel booking based on memberID[manual input if isAdmin] and trainID/bookingID(search filter) - For admin/member
void TB_DeleteBooking(string memberID) 
{
	int selectedBooking = 0;
	int seatID;
	int qty = 120;
	double totalPrice = 0;

	int countMemberData = 0;
	int* indexOfMemberBookingID;
	int* dataToBeDeleted = NULL;
	int* tempDataToBeDeleted = NULL;
	int count = 0;

	int numRow = G_GetFileNumRow("Booking.dat", sizeof(Booking));

	Booking tempBooking;
	Booking* booking = (Booking*)malloc(sizeof(Booking) * numRow);
	if (!booking) {
		printf("Memory allocated failed.");
		exit(ISERROR);
	}

	//get all the data from the file
	FILE* fptrForRead = fopen("Booking.dat", "rb");
	if (!fptrForRead) {
		printf("Error when opening file.");
		exit(ISERROR);
	}
	fread(booking, sizeof(Booking), numRow, fptrForRead);
	fclose(fptrForRead);

	indexOfMemberBookingID = TB_GetindexOfMemberBookingID(booking, numRow, &countMemberData);
	if (!indexOfMemberBookingID) {
		printf("No User record.");
		return;
	}
	do
	{
		tempDataToBeDeleted = (int*)realloc(dataToBeDeleted, sizeof(int) * (count + 1));
		if (!tempDataToBeDeleted) {
			printf("Memory Allocation Failed.");
			free(tempDataToBeDeleted);
			free(dataToBeDeleted);
			exit(ISERROR);
		}
		dataToBeDeleted = tempDataToBeDeleted;

		//Display User Booking details
		//Header
		TB_DisplayBookingHdr(booking[indexOfMemberBookingID[0]], 1, &totalPrice);
		//Details
		for (int i = 1; i <= countMemberData; i++)
		{
			if (!G_InArray(dataToBeDeleted, indexOfMemberBookingID[i - 1],
				sizeof(int), countMemberData, G_CompareInt))
			{
				TB_DisplayBookingDet(booking[indexOfMemberBookingID[i - 1]], &totalPrice, i);
			}
		}
		//Footer
		TB_DisplayBookingFoot(1, totalPrice);

		dataToBeDeleted[count++] = TB_GetBookingID(numRow);

		//get from train schedule
	} while (TB_ContinueOrStopCRUDRecord("delete"));
	
	FILE* fptrForDelete = fopen("Booking.dat", "wb");
	if (!fptrForDelete) {
		printf("File unalbe to open");
		exit(ISERROR);
	}
	for (int i = 1; i <= countMemberData; i++)
	{
		if (!G_InArray(dataToBeDeleted, indexOfMemberBookingID[i - 1],
			sizeof(int), countMemberData, G_CompareInt))
		{
			fwrite(&booking[indexOfMemberBookingID[i - 1]], sizeof(Booking), 1, fptrForDelete);
		}
	}

	free(tempDataToBeDeleted);
	free(dataToBeDeleted);
	fclose(fptrForDelete);
}



//Create[Member] maybe admin can help member to add booking also
//member id get from local variable instead of accessing from the global variable, more flexible
//pass the value as argument when storing it at [TB_BookingTicket]
//member point reward system 
void TB_BookingTicket()
{
	int length = 0;
	
	Booking* booking = TB_GetBookingDetails(&length, userID);
	double totalPrice;
	char confirmAddRecord;
	FILE* fptr = fopen("Booking.dat", "wb");
	if (!fptr) 
	{
		printf("Error when opening file.");
		exit(ISERROR);
	}
	//Display User Booked Ticket
	//Header
	TB_DisplayBookingHdr(booking[0], 1, &totalPrice);
	//Details
	for (int i = 1; i <= length; TB_DisplayBookingDet(booking[i], &totalPrice, i), i++);
	//Footer
	TB_DisplayBookingFoot(1, totalPrice);

	//Booking Confirmation
	do{
		printf("Booking Confirmation[Y/N] >>");
		scanf("%c", &confirmAddRecord);
		rewind(stdin);
		system("cls");
		confirmAddRecord = toupper(confirmAddRecord);
	} while (!(confirmAddRecord == 'Y' || confirmAddRecord == 'N'));

	if (confirmAddRecord == 'Y') {
		fwrite(&booking, sizeof(Booking), length, fptr);
		//update the newest Ewallet amount of the member
	}

	fclose(fptr);
	free(booking);
}
Booking* TB_GetBookingDetails(int* length,string memberID) {
	int count = 0;
	int currPointedIndex;
	int numRow = G_GetFileNumRow("Booking.dat", sizeof(Booking));
	Booking* booking = NULL;
	Booking* tempBooking = NULL;
	do
	{
		tempBooking = (Booking*)realloc(booking, sizeof(Booking) * ++count);
		if (!tempBooking) {
			printf("Memory allocation failed.");
			free(booking);
			free(tempBooking);
			exit(ISERROR);
		}
		booking = tempBooking;

		currPointedIndex = --count;

		(booking + currPointedIndex)->bookingDate = G_GetCurrentDate();
		(booking + currPointedIndex)->ticketDetails = TB_GetTicketDetails();

		strcpy((booking + currPointedIndex)->memberID, memberID);
		sprintf((booking + currPointedIndex)->bookingID, "B%04d", numRow + count);
	} while (TB_ContinueOrStopCRUDRecord("add"));
	
	*length = count;
	free(tempBooking);
	return booking;
}
int TB_ContinueOrStopCRUDRecord(char * CRUD) {
	char continueCRUDRecord;
	do
	{
		printf("Do you want to continue %s record?[Y/N]", CRUD);
		scanf("%c", &continueCRUDRecord);
		rewind(stdin);
		system("cls");
		continueCRUDRecord = toupper(continueCRUDRecord);
	} while (!(continueCRUDRecord == 'Y' || continueCRUDRecord == 'N'));
	return continueCRUDRecord == 'Y';
}
Ticket TB_GetTicketDetails()
{
	int qty = 120; // get from train schedule
	int seatType = 1;
	char selectedCoach;
	int selectedSeatType;
	int seatID;
	char selectedTrainID[10] = "\0";
	char selectedSeatID[10] = "\0";
	
	TB_GetTrainID(&selectedTrainID);
	TB_DisplaySeat(qty);
	TB_GetSeatDetails(qty, &seatID, &selectedSeatType, &selectedCoach);
	sprintf(selectedSeatID, "S%04d", seatID);
	Ticket ticketDetails =
	{ selectedTrainID, selectedSeatID, selectedSeatType, selectedCoach,
		TB_GetTicketPrice(selectedSeatType,G_GetCurrentTime(),G_GetCurrentDate()) };
	return ticketDetails;
} 
void TB_GetTrainID(string trainID)
{
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
	
	sprintf(trainID, "T%04d", selectedTrainID);
	fclose(fptr);
}
void TB_DisplaySeat(int seatQuantity) {
	// 50% Standard[1] : 20% Premium[2] : 20% Sleeping Berths[3] : 10% Special Needs Seats[4](For Child/Seniors)

	//each coach can only contain 40 seat(col-10, row-4)
	//number of coach = seatQuantity / 40
	int seat;
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
				if (TB_SeatAvailability(seat)) {
					currColor = color[4];
				}

				printf("%s", currColor);
				printf("%03d ", seat);
				printf("%s", color[0]);
			}
			printf("|\n-------------------------------------------------------------\n\n");
		}
		printf("|\n\n");
	}
}
void TB_GetSeatDetails(int seatQuantity,int* selectedSeatID, int* selectedSeatType, string selectedCoach) {
	int seatID;
	
	int	numOfSeat[4] =
	{
		(int)ceil(seatQuantity * 0.5),
		(int)floor(seatQuantity * 0.2),
		(int)floor(seatQuantity * 0.2),
		(int)ceil(seatQuantity * 0.1)
	};

	do
	{
		scanf("%d", &seatID);
		rewind(stdin);
		system("cls");
		if (TB_SeatAvailability(seatID)) {
			continue;
		}
	} while (seatQuantity < seatID && seatID < 0);
	
	*selectedSeatID = seatID;
	*selectedCoach = 'A' + (int)floor(seatID / 40.0);
	*selectedSeatType = TB_GetSeatType(seatID, numOfSeat, sizeof(numOfSeat) / sizeof(numOfSeat[0]));
}
int TB_SeatAvailability(int seatID) {
	int* notAvailableSeat = NULL; //get from booking ticket
	int* tempNotAvailableSeat = NULL;
	int count = 0;
	int seatNo = 0;
	int seatAvailability = 0;
	Booking booking;
	FILE* fptr = fopen("Booking.dat", "rb");

	if (!fptr)
	{
		printf("Error when opening file.");
		exit(ISERROR);
	}
	while (fread(&booking, sizeof(Booking), 1, fptr) != 0) {
		tempNotAvailableSeat = (int*)realloc(notAvailableSeat, sizeof(int) * ++count);
		if (!tempNotAvailableSeat) {
			printf("Memory allocation Failed.\n");
			notAvailableSeat = NULL;
			break;
		}
		notAvailableSeat = tempNotAvailableSeat;
		sprintf(booking.ticketDetails.seatID, "S%04d", notAvailableSeat[count - 1]);
	}
	if (notAvailableSeat != NULL) {
		for (int i = 0; i < count; i++)
		{
			if (seatID == notAvailableSeat[i]) {
				seatAvailability = 1;
				break;
			}
		}
	}

	free(notAvailableSeat);
	free(tempNotAvailableSeat);
	fclose(fptr);
	return seatAvailability;
}
int TB_GetSeatType(int selectedSeatID, int numOfSeat[], int length) {
	for (int i = 0; i < length; i++)
	{
		if ((selectedSeatID -= numOfSeat[i]) < 0)
		{
			return i;
		}
	}
	return ISERROR;
}
double TB_GetTicketPrice(int seatType, Time startTime,Date departureDate) {

	double seatTypeRate[4] = { 1.0,1.1,1.2,0.8 };

	Date currDate = G_GetCurrentDate();

	int diffDate = G_CalDiffDate(currDate, departureDate);
	if (diffDate == ISERROR) {
		printf("This date can\'t be selected.\n");
		return -1;
	}

	//50% Standard[1] offer 100% : 20% Premium[2] offer 110%: 20% Sleeping Berths[3] offer 120%: 10% Special Needs Seats[4](For Child/Seniors) offer 80%
	//Ticket price based on time[after 1730 inc price 20%]
	//if get closer to departure date, price higher
	
	//Ticket Price = Base Price * SeatType[Rate] * DiffDate[Rate] * startTime[Rate]
	return 5.0 *
		seatTypeRate[seatType] * 
		((diffDate < 30) ? 2.0 : 1.0) * 
		((startTime.hour * 60 + startTime.min) > 1050 ? 1.2 : 1.0); 
}


	
