# Parking Management System using DSA in C

A beginner-friendly, menu-driven C mini-project that simulates a 10-slot parking lot. It assigns slots, records vehicle entry/exit times, calculates a demo parking fee, and stores checkout history using a stack.

## Features
- View available and occupied parking slots
- Park a vehicle in the first available slot
- Prevent duplicate vehicle registration numbers
- Check out a vehicle and calculate parking charges
- View currently parked vehicles and entry times
- View the latest checkout using the stack top
- View all checkout records in newest-first order
- Handle invalid menu input and a full parking lot

## DSA concepts used

| Concept | Usage |
|---|---|
| Array | Stores the fixed parking slots |
| Structures (`struct`) | Groups slot, vehicle, and checkout details |
| Stack (LIFO) | Stores checkout records; latest checkout is at the top |
| Linear search | Finds vehicles and available slots |
| String operations | Compares and copies vehicle details |
| Time functions | Records entry/exit time and calculates duration |

## Project files
```text
parking-management-system/
├── main.c
├── README.md
├── .gitignore
└── LICENSE
```

## Requirements
- GCC or another C compiler
- Terminal / Command Prompt

## Compile and run

**Windows (GCC / MinGW):**
```bash
gcc main.c -o parking
.\parking.exe
```

**Linux / macOS:**
```bash
gcc main.c -o parking
./parking
```

## Parking fee rule
The demo charges **₹20 per started hour**, with a minimum charge of ₹20. A 10-minute stay is billed as one hour. Change the rate in `checkoutVehicle()` if you want a different rule.

## Basic algorithm
1. Initialize all parking slots as available and set stack top to `-1`.
2. Display the menu until the user selects Exit.
3. To park a vehicle, check for duplicates, find the first free slot, and save the vehicle details.
4. To check out, search for the vehicle and calculate the parking duration and fee.
5. Push the checkout record onto the stack and mark the parking slot as available.
6. Display checkout records from the stack top to bottom.

## Complexity
Let `n` be the number of slots and `h` the number of checkout records.
- Find a vehicle / free slot: `O(n)`
- Display parking status: `O(n)`
- Push a checkout record: `O(1)`
- Display checkout history: `O(h)`

## Limitations
- Records are stored in memory and reset when the program exits.
- The parking lot has 10 fixed slots.
- The parking rate is only a demonstration.
- This is a terminal-based C project, not a deployed website.

## Future improvements
- Save records to a file for permanent storage
- Add daily revenue reports and vehicle categories
- Make slot count and parking fee configurable
- Add a queue for vehicles waiting when the lot is full
- Add a web interface as a separate front end

## GitHub upload
1. Create a repository named `parking-management-system`.
2. Upload `main.c`, `README.md`, `.gitignore`, and `LICENSE`.
3. Commit with a message like `Add parking management system using C and DSA`.
4. Compile and run the program locally; GitHub does not run a terminal C app as a website.

## Author
Beginner DSA mini-project using C.
