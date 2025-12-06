#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROOM_FILE "rooms.txt"
#define BOOKING_FILE "bookings.txt"

struct Room {
    int roomNumber;
    char roomType[20];
    float price;
};

struct Booking {
    int bookingID;
    int roomNumber;
    char guestName[50];
    char checkIn[20];
    char checkOut[20];
};

void addSampleRooms() {
    FILE *fp = fopen(ROOM_FILE, "a");
    if (!fp) return;

    fprintf(fp, "101 Single 1500\n");
    fprintf(fp, "102 Double 2200\n");
    fprintf(fp, "201 Deluxe 3500\n");
    fprintf(fp, "202 Suite 5000\n");
    fclose(fp);
}

void viewRooms() {
    FILE *fp = fopen(ROOM_FILE, "r");
    struct Room r;

    if (!fp) {
        printf("Room file not found!\n");
        return;
    }

    printf("\n--- Available Rooms ---\n");
    while (fscanf(fp, "%d %s %f", &r.roomNumber, r.roomType, &r.price) != EOF) {
        printf("Room No: %d | Type: %s | Price: %.2f\n",
               r.roomNumber, r.roomType, r.price);
    }
    fclose(fp);
}

int roomExists(int roomNumber) {
    FILE *fp = fopen(ROOM_FILE, "r");
    struct Room r;
    if (!fp) return 0;

    while (fscanf(fp, "%d %s %f", &r.roomNumber, r.roomType, &r.price) != EOF) {
        if (r.roomNumber == roomNumber) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

void createBooking() {
    FILE *fp = fopen(BOOKING_FILE, "a");
    struct Booking b;

    printf("Enter Booking ID: ");
    scanf("%d", &b.bookingID);
    printf("Enter Room Number: ");
    scanf("%d", &b.roomNumber);

    if (!roomExists(b.roomNumber)) {
        printf("? Room does not exist!\n");
        return;
    }

    printf("Guest Name: ");
    scanf("%s", b.guestName);
    printf("Check-in Date: ");
    scanf("%s", b.checkIn);
    printf("Check-out Date: ");
    scanf("%s", b.checkOut);

    fprintf(fp, "%d %d %s %s %s\n", b.bookingID, b.roomNumber, b.guestName, b.checkIn, b.checkOut);
    fclose(fp);

    printf("? Booking Successful!\n");
}

void viewBookings() {
    FILE *fp = fopen(BOOKING_FILE, "r");
    struct Booking b;

    if (!fp) {
        printf("No bookings found!\n");
        return;
    }

    printf("\n--- All Bookings ---\n");
    while (fscanf(fp, "%d %d %s %s %s", &b.bookingID, &b.roomNumber,
                  b.guestName, b.checkIn, b.checkOut) != EOF) {
        printf("ID: %d | Room: %d | Guest: %s | In: %s | Out: %s\n",
               b.bookingID, b.roomNumber, b.guestName, b.checkIn, b.checkOut);
    }
    fclose(fp);
}

void cancelBooking() {
    int id;
    printf("Enter Booking ID to cancel: ");
    scanf("%d", &id);

    FILE *fp = fopen(BOOKING_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Booking b;
    int found = 0;

    while (fscanf(fp, "%d %d %s %s %s", &b.bookingID, &b.roomNumber,
                  b.guestName, b.checkIn, b.checkOut) != EOF) {
        if (b.bookingID != id) {
            fprintf(temp, "%d %d %s %s %s\n", b.bookingID, b.roomNumber,
                    b.guestName, b.checkIn, b.checkOut);
        } else found = 1;
    }

    fclose(fp);
    fclose(temp);

    remove(BOOKING_FILE);
    rename("temp.txt", BOOKING_FILE);

    if (found)
        printf("? Booking Cancelled!\n");
    else
        printf("? Booking ID not found!\n");
}

int main() {
    int choice;

    addSampleRooms(); // Only if file empty

    while(1) {
        printf("\n===== HOTEL RESERVATION SYSTEM =====\n");
        printf("1. View Rooms\n");
        printf("2. Create Booking\n");
        printf("3. View Bookings\n");
        printf("4. Cancel Booking\n");
        printf("0. Exit\n");
        printf("Enter Choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: viewRooms(); break;
            case 2: createBooking(); break;
            case 3: viewBookings(); break;
            case 4: cancelBooking(); break;
            case 0: exit(0);
            default: printf("Invalid option!\n");
        }
    }
}
