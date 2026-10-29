#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SLOTS 10
#define PLATE_LEN 20
#define NAME_LEN 50
#define MAX_RECORDS 1000

typedef struct {
    int slotNumber;
    int occupied;
    char plate[PLATE_LEN];
    char owner[NAME_LEN];
    time_t entryTime;
} ParkingSlot;

typedef struct {
    int slotNumber;
    char plate[PLATE_LEN];
    char owner[NAME_LEN];
    time_t entryTime, exitTime;
    double fee;
} ParkingRecord;

typedef struct {
    ParkingRecord items[MAX_RECORDS];
    int top;
} Stack;

ParkingSlot slots[MAX_SLOTS];
Stack checkoutHistory;

void initialize(void) {
    for (int i = 0; i < MAX_SLOTS; i++) {
        slots[i].slotNumber = i + 1;
        slots[i].occupied = 0;
        slots[i].plate[0] = '\0';
        slots[i].owner[0] = '\0';
        slots[i].entryTime = 0;
    }
    checkoutHistory.top = -1;
}

void clearInput(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}
}

void readLine(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin))
        buffer[strcspn(buffer, "\n")] = '\0';
    else
        buffer[0] = '\0';
}

int findVehicle(const char *plate) {
    for (int i = 0; i < MAX_SLOTS; i++)
        if (slots[i].occupied && strcmp(slots[i].plate, plate) == 0)
            return i;
    return -1;
}

int findFreeSlot(void) {
    for (int i = 0; i < MAX_SLOTS; i++)
        if (!slots[i].occupied) return i;
    return -1;
}

void viewSlots(void) {
    int available = 0;
    printf("\n========== PARKING SLOT STATUS ==========\n");
    printf("%-8s %-14s %-18s\n", "Slot", "Status", "Vehicle Number");
    for (int i = 0; i < MAX_SLOTS; i++) {
        printf("%-8d %-14s %-18s\n", slots[i].slotNumber,
               slots[i].occupied ? "Occupied" : "Available",
               slots[i].occupied ? slots[i].plate : "-");
        if (!slots[i].occupied) available++;
    }
    printf("Available slots: %d/%d\n", available, MAX_SLOTS);
}

void parkVehicle(void) {
    char plate[PLATE_LEN], owner[NAME_LEN];
    readLine("\nEnter vehicle number (e.g. TN01AB1234): ", plate, sizeof plate);
    if (plate[0] == '\0') {
        puts("Vehicle number cannot be empty.");
        return;
    }
    if (findVehicle(plate) != -1) {
        puts("This vehicle is already parked.");
        return;
    }
    int index = findFreeSlot();
    if (index == -1) {
        puts("Parking is full. No slot is available.");
        return;
    }
    readLine("Enter owner name: ", owner, sizeof owner);
    if (owner[0] == '\0') strcpy(owner, "Not provided");

    slots[index].occupied = 1;
    strcpy(slots[index].plate, plate);
    strcpy(slots[index].owner, owner);
    slots[index].entryTime = time(NULL);

    printf("\nVehicle parked successfully! Assigned slot: %d\n", slots[index].slotNumber);
    printf("Entry time: %s", ctime(&slots[index].entryTime));
}

void viewCurrentVehicles(void) {
    int found = 0;
    printf("\n================ CURRENT VEHICLES ================\n");
    printf("%-8s %-16s %-20s %-24s\n", "Slot", "Vehicle No.", "Owner", "Entry Time");
    for (int i = 0; i < MAX_SLOTS; i++) {
        if (slots[i].occupied) {
            char entry[32];
            struct tm *info = localtime(&slots[i].entryTime);
            if (info) strftime(entry, sizeof entry, "%d-%m-%Y %H:%M:%S", info);
            else strcpy(entry, "Unknown");
            printf("%-8d %-16s %-20s %-24s\n", slots[i].slotNumber,
                   slots[i].plate, slots[i].owner, entry);
            found = 1;
        }
    }
    if (!found) puts("No vehicles are currently parked.");
}

void checkoutVehicle(void) {
    char plate[PLATE_LEN];
    readLine("\nEnter vehicle number to check out: ", plate, sizeof plate);
    int index = findVehicle(plate);
    if (index == -1) {
        puts("Vehicle not found.");
        return;
    }

    time_t exitTime = time(NULL);
    double elapsedSeconds = difftime(exitTime, slots[index].entryTime);
    double elapsedHours = elapsedSeconds / 3600.0;
    int billableHours = (int)elapsedHours;
    if (elapsedHours > billableHours) billableHours++;
    if (billableHours < 1) billableHours = 1;
    double fee = billableHours * 20.0; /* Demo rate: Rs. 20 per started hour */

    if (checkoutHistory.top < MAX_RECORDS - 1) {
        ParkingRecord *r = &checkoutHistory.items[++checkoutHistory.top];
        r->slotNumber = slots[index].slotNumber;
        strcpy(r->plate, slots[index].plate);
        strcpy(r->owner, slots[index].owner);
        r->entryTime = slots[index].entryTime;
        r->exitTime = exitTime;
        r->fee = fee;
    }

    printf("\nCheckout successful for %s (Slot %d).\n", slots[index].plate, slots[index].slotNumber);
    printf("Duration: %.0f minute(s) | Billable hours: %d\n", elapsedSeconds / 60.0, billableHours);
    printf("Parking fee: Rs. %.2f\n", fee);

    slots[index].occupied = 0;
    slots[index].plate[0] = '\0';
    slots[index].owner[0] = '\0';
    slots[index].entryTime = 0;
}

void viewLatestCheckout(void) {
    if (checkoutHistory.top == -1) {
        puts("\nNo checkout records yet.");
        return;
    }
    ParkingRecord *r = &checkoutHistory.items[checkoutHistory.top];
    char entry[32], exit[32];
    struct tm *info = localtime(&r->entryTime);
    if (info) strftime(entry, sizeof entry, "%d-%m-%Y %H:%M:%S", info);
    else strcpy(entry, "Unknown");
    info = localtime(&r->exitTime);
    if (info) strftime(exit, sizeof exit, "%d-%m-%Y %H:%M:%S", info);
    else strcpy(exit, "Unknown");

    printf("\n======= MOST RECENT CHECKOUT (STACK TOP) =======\n");
    printf("Vehicle number : %s\nOwner          : %s\nSlot           : %d\n", r->plate, r->owner, r->slotNumber);
    printf("Entry time     : %s\nExit time      : %s\nFee            : Rs. %.2f\n", entry, exit, r->fee);
}

void viewCheckoutHistory(void) {
    if (checkoutHistory.top == -1) {
        puts("\nCheckout history is empty.");
        return;
    }
    printf("\n========== CHECKOUT HISTORY (NEWEST FIRST) ==========\n");
    printf("%-16s %-8s %-12s\n", "Vehicle No.", "Slot", "Fee (Rs.)");
    for (int i = checkoutHistory.top; i >= 0; i--)
        printf("%-16s %-8d %-12.2f\n", checkoutHistory.items[i].plate,
               checkoutHistory.items[i].slotNumber, checkoutHistory.items[i].fee);
}

int main(void) {
    int choice;
    initialize();
    puts("========================================");
    puts("       PARKING MANAGEMENT SYSTEM");
    puts("          DSA Concepts in C");
    puts("========================================");

    do {
        puts("\n--------------- MAIN MENU ---------------");
        puts("1. View parking slots");
        puts("2. Park a vehicle");
        puts("3. Check out a vehicle");
        puts("4. View currently parked vehicles");
        puts("5. View most recent checkout (Stack)");
        puts("6. View all checkout history");
        puts("0. Exit");
        printf("Choose an option: ");

        if (scanf("%d", &choice) != 1) {
            puts("Please enter a valid number.");
            clearInput();
            choice = -1;
            continue;
        }
        clearInput();

        switch (choice) {
            case 1: viewSlots(); break;
            case 2: parkVehicle(); break;
            case 3: checkoutVehicle(); break;
            case 4: viewCurrentVehicles(); break;
            case 5: viewLatestCheckout(); break;
            case 6: viewCheckoutHistory(); break;
            case 0: puts("Thank you for using the Parking Management System!"); break;
            default: puts("Invalid option. Choose from 0 to 6.");
        }
    } while (choice != 0);

    return 0;
}
