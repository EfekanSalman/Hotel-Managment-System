  #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_GUESTS 100
#define MAX_ROOMS 50

typedef struct {
	
	int day;
	int month;
	int year;

} Date;


typedef struct {
	
	int id;
	char name[100];
	char phone[15];
	int room_number;
	Date check_in_date;
	Date check_out_date;
	float total_bill;
	
} Guest;


typedef struct {
	
	int room_number;
	int is_booked;
	float price_per_night;
	
} Room;


Guest guests[MAX_GUESTS];
Room rooms[MAX_ROOMS];
int guest_count = 0;


void initialize_rooms() {
	
	float base_price = 100.0;
	for (int i = 0; i < MAX_ROOMS; i++) {
		rooms[i].room_number = i + 1;
		rooms[i].is_booked = 0;
		rooms[i].price_per_night = base_price + (i % 10) * 20.0;
	}
}


int date_compare(Date d1, Date d2) {
	
	if (d1.year != d2.year)
		return d1.year - d2.year;
	if (d1.month != d2.month)
		return d1.month - d2.month;
	return d1.day - d2.day;
}


int is_room_available(int room_number, Date check_in, Date check_out) {
	
	for (int i = 0; i < guest_count; i++) {
		if (guests[i].room_number == room_number) {
			if ((date_compare(check_in, guests[i].check_in_date) >= 0 && date_compare(check_in, guests[i].check_out_date) < 0) ||
				(date_compare(check_out, guests[i].check_in_date) >= 0 && date_compare(check_out, guests[i].check_out_date) <= 0) ||
				(date_compare(check_in, guests[i].check_in_date) <= 0 && date_compare(check_out, guests[i].check_out_date) >= 0)) {
					return 0;
			}
		}
	}
	return 1;
}


int calculate_nights(Date check_in, Date check_out) {
	
	struct tm tm_check_in = {0}, tm_check_out = {0};
	tm_check_in.tm_year = check_in.year - 1900;
	tm_check_in.tm_mon = check_in.month - 1;
	tm_check_in.tm_mday = check_in.day;
	tm_check_out.tm_year = check_out.year - 1900;
	tm_check_out.tm_mon = check_out.month - 1;
	tm_check_out.tm_mday = check_out.day;
	
	time_t time_check_in = mktime(&tm_check_in);
	time_t time_check_out = mktime(&tm_check_out);
	
	double seconds_diff = difftime(time_check_out, time_check_in);
	int nights = seconds_diff / (60 * 60 * 24);
	
	return nights;
}


void add_guest() {
	
	if (guest_count >= MAX_GUESTS) {
		printf("Cannot add more guests.\n");
		return;
	}
	
	Guest new_guest;
	new_guest.id = guest_count + 1;
	printf("Enter guest name: ");
	scanf("%s", new_guest.name);
	printf("Enter guest phone: ");
	scanf("%s", new_guest.phone);

	printf("Enter check-in date (DD MM YYYY): ");
	scanf("%d %d %d", &new_guest.check_in_date.day, &new_guest.check_in_date.month, &new_guest.check_in_date.year);
	printf("Enter check-out date (DD MM YYYY): ");
	scanf("%d %d %d", &new_guest.check_out_date.day, &new_guest.check_out_date.month, &new_guest.check_out_date.year);

	
	int room_number;
	do {
		
		printf("Enter room number (1-%d): ", MAX_ROOMS);
		scanf("%d", &room_number);
		if (room_number < 1 || room_number > MAX_ROOMS) {
			printf("Invalid room number. Please try again.\n");	
		}
		else if (!is_room_available(room_number, new_guest.check_in_date, new_guest.check_out_date)) {
			printf("Room is not available for the given dates. Please choose another room\n");
		}
		else {
			new_guest.room_number = room_number;
		}
	}
	while (room_number < 1 || room_number > MAX_ROOMS || !is_room_available(room_number, new_guest.check_in_date, new_guest.check_out_date));
	

	
	int num_nights = calculate_nights(new_guest.check_in_date, new_guest.check_out_date);
	new_guest.total_bill = num_nights * rooms[new_guest.room_number - 1].price_per_night;
	
	guests[guest_count++] = new_guest;
	rooms[new_guest.room_number - 1].is_booked = 1;
	
	printf("Guest added succesfully!\n");
}


void view_guests() {
    
	printf("Guest List: \n");
    for (int i = 0; i < guest_count; i++) {
        Guest g = guests[i];
        printf("ID: %d, Name: %s, Phone: %s, Room: %d, Check-in: %02d/%02d/%04d, Check-out: %02d/%02d/%04d, Total Bill: $%.2f\n", 
        g.id, g.name, g.phone, g.room_number,
        g.check_in_date.day, g.check_in_date.month, g.check_in_date.year,
        g.check_out_date.day, g.check_out_date.month, g.check_out_date.year,
        g.total_bill);
    }
}


void check_out_guest() {
	
	int guest_id;
	printf("Enter guest ID to check out: ");
	scanf("%d", &guest_id);
	
	if (guest_id <= 0 || guest_id > guest_count) {
		printf("Invalid guest ID.\n");
		return;
	}
	
	Guest *g = &guests[guest_id - 1];
	printf("Guest %s checked out from room %d.\n", g -> name, g -> room_number);
	
	rooms[g -> room_number - 1].is_booked = 0;
	printf("Total bill: $%.2f\n", g -> total_bill);
	
	memmove(&guests[guest_id - 1], &guests[guest_id], (guest_count - guest_id) * sizeof(Guest));
	guest_count--;
}



int main(int argc, char *argv[]) {
	
	initialize_rooms();
	int choice;
	do {
		printf("Hotel Managmet System\n");
		printf("1) Add Guest\n");
		printf("2) View Guests\n");
		printf("3) Check Out Guest\n");
		printf("4) Exit\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		
		switch (choice) {
			case 1:
				add_guest();
				break;
			case 2:
				view_guests();
				break;
			case 3:
				check_out_guest();
				break;
			case 4:
				printf("Exiting...\n");
				break;
			default:
				printf("Invalid choice. Please try again.\n");
		}		
	} 
	while (choice != 4);

	
	return 0;
}
