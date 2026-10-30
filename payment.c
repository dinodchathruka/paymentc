
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int validuserid(char id[]);

int countseats(char text[]);

void getuserid(char search_id[]);

int getshowid(char search_id[]);

void getshowtime(char search_id[], int search_showid,
                 char search_time[],
                 char user_id[], int *movieid, char time[],
                 char box[], char balcony[], char normal[]);

void calculateprices(int box_count, int balcony_count,
                     int normal_count,
                     float *box_price, float *balcony_price,
                     float *normal_price, float *total);

void displaybooking(char user_id[], int movieid, char time[],
                    int box_count, int balcony_count,
                    int normal_count, int tickets,
                    char box[], char balcony[], char normal[],
                    float box_price, float balcony_price,
                    float normal_price, float total);

int payment(void);

void savepayment(char user_id[], int movieid, char time[],
                 int box_count, int balcony_count,
                 int normal_count, int tickets,
                 char box[], char balcony[], char normal[],
                 float box_price, float balcony_price,
                 float normal_price, float total);




int main() {

    int tickets, movieid, search_showid;
    int box_count, balcony_count, normal_count;

    float total, box_price, balcony_price, normal_price;

    char user_id[10], search_id[10];
    char time[10], search_time[10];
    char box[10], balcony[10], normal[10];

    FILE *sa;

    sa = fopen("seat_allocation.txt", "r");

    if (sa == NULL) {
        printf("File not found!\n");
        return 1;
    }

    fclose(sa);

    // User ID

    getuserid(search_id);

    // Show ID

    search_showid = getshowid(search_id);

    // Showtime

    getshowtime(search_id, search_showid, search_time,
                user_id, &movieid, time, box, balcony, normal);

    // seats

    box_count = countseats(box);
    balcony_count = countseats(balcony);
    normal_count = countseats(normal);

    tickets = box_count + balcony_count + normal_count;

    // Calculate prices

    calculateprices(box_count, balcony_count, normal_count,
                    &box_price, &balcony_price,
                    &normal_price, &total);

    // Display booking

    displaybooking(user_id, movieid, time,
                   box_count, balcony_count, normal_count,
                   tickets, box, balcony, normal,box_price, balcony_price, normal_price, total);

    // Payment

   if (payment() == 1) {
   
   	 printf("\n============ Payment Successful! ============\n");

        printf("\n============= PAYMENT DETAILS ================\n");

        displaybooking(user_id, movieid, time,
                       box_count, balcony_count, normal_count,
                       tickets, box, balcony, normal,
                       box_price, balcony_price, normal_price, total);

        // Save payment

        savepayment(user_id, movieid, time,
                    box_count, balcony_count, normal_count,
                    tickets, box, balcony, normal,
                    box_price, balcony_price, normal_price, total);

        printf("\nBooking Confirmed!\n");
    

    return 0;
}
}



int validuserid(char id[]) {

    int i;

    if (strlen(id) != 4) {
        return 0;
    }

    if (id[0] != 'u') {
        return 0;
    }

    for (i = 1; i < 4; i++) {

        if (id[i] < '0' || id[i] > '9') {
            return 0;
        }
    }

    return 1;
}




int countseats(char text[]) {

    int count = 0;
    char copy[10];
    char *p;

    strcpy(copy, text);

    if (strcmp(copy, "-") == 0) {
        return 0;
    }

    p = strtok(copy, ",");

    while (p != NULL) {

        count++;
        p = strtok(NULL, ",");
    }

    return count;
}



void getuserid(char search_id[]) {

    do {

        printf("\nEnter your user ID: ");
        scanf("%s", search_id);

        if (validuserid(search_id) == 0) {

            printf("Invalid User ID! Please enter again.\n");
        }

    } while (validuserid(search_id) == 0);
}




int getshowid(char search_id[]) {

    int search_showid, movieid, found = 0;
    int ch;

    char user_id[10], time[10];
    char box[10], balcony[10], normal[10];

    FILE *sa;

    do {

        printf("Enter Show ID: ");

        if (scanf("%d", &search_showid) != 1) {

            printf("Invalid Show ID!Please enter again. \n");

            while ((ch = getchar()) != '\n' && ch != EOF);

            found = 0;
            continue;
        }

        found = 0;

        sa = fopen("seat_allocation.txt", "r");

        if (sa == NULL) {
            printf("Error opening file!\n");
            exit(1);
        }

        while (fscanf(sa, "%s %d %s %s %s %s",
                      user_id, &movieid, time,
                      box, balcony, normal) == 6) {

            if (strcmp(search_id, user_id) == 0 &&
                search_showid == movieid) {

                found = 1;
                break;
            }
        }

        fclose(sa);

        if (found == 0) {

            printf("Invalid Show ID! Please enter again.\n");
        }

    } while (found == 0);

    return search_showid;
}




void getshowtime(char search_id[], int search_showid,
                 char search_time[],
                 char user_id[], int *movieid, char time[],
                 char box[], char balcony[], char normal[]) {

    int found;

    FILE *sa;

    do {

        printf("Enter show time (10:00AM, 2:00PM, 6:00PM): ");
        scanf("%s", search_time);

        found = 0;

        sa = fopen("seat_allocation.txt", "r");

        if (sa == NULL) {
            printf("Error opening file!\n");
            exit(1);
        }

        while (fscanf(sa, "%s %d %s %s %s %s",
                      user_id, movieid, time,
                      box, balcony, normal) == 6) {

            if (strcmp(search_id, user_id) == 0 &&
                search_showid == *movieid &&
                strcmp(search_time, time) == 0) {

                found = 1;
                break;
            }
        }

        fclose(sa);

        if (found == 0) {

            printf("Invalid Showtime! Please enter again.\n");
        }

    } while (found == 0);
}




void calculateprices(int box_count, int balcony_count,
                     int normal_count,
                     float *box_price, float *balcony_price,
                     float *normal_price, float *total) {

    *box_price = box_count * 1200.00;
    *balcony_price = balcony_count * 600.00;
    *normal_price = normal_count * 400.00;

    *total = *box_price + *balcony_price + *normal_price;
}




void displaybooking(char user_id[], int movieid, char time[],
                    int box_count, int balcony_count,
                    int normal_count, int tickets,
                    char box[], char balcony[], char normal[],
                    float box_price, float balcony_price,
                    float normal_price, float total) {

    printf("\n========== BOOKING DETAILS ==========\n");

    printf("\nUser ID: %s\n", user_id);
    printf("Show ID  : %d\n", movieid);
    printf("Show Time: %s\n", time);

    printf("\n========== Ticket Details ==========\n");

    printf("Box tickets     : %d\n", box_count);
    printf("Balcony tickets : %d\n", balcony_count);
    printf("Normal tickets  : %d\n", normal_count);
    printf("Total tickets   : %d\n", tickets);

    printf("\n======== Selected Seat Numbers ======\n");

    printf("Box seat numbers     : %s\n", box);
    printf("Balcony seat numbers : %s\n", balcony);
    printf("Normal seat numbers  : %s\n", normal);
    
    printf( "\n========== Ticket Prices ==========\n");

    printf( "Box tickets price    : Rs. %.2f\n", box_price);
    printf( "Balcony tickets price: Rs. %.2f\n", balcony_price);
    printf("Normal tickets price  : Rs. %.2f\n", normal_price);
    
    printf("		     -----------------\n");

    printf("\nTotal is            : Rs. %.2f\n", total);
    printf("		     -----------------\n");
    printf("		     -----------------\n");
}




int payment() {

    int method;

    char cardnum[20], ccc[10];

    printf("\n===== CINEMA TICKET PAYMENT =====\n");

    printf("\n1. Card\n");
    printf("2. Cancel\n\n");

    printf("Select payment method: ");

    if (scanf("%d", &method) != 1) {

        printf("Invalid payment method!\n");
        return 0;
    }

    if (method == 2) {

        printf("Cancel the payment\n");
        return 0;
    }

    else if (method != 1) {

        printf("Invalid payment method!\n");
        return 0;
    }

  

    printf("Enter visa card number: ");
    scanf("%s", cardnum);

    while (strlen(cardnum) != 16) {

        printf("Invalid card number!\n");

        printf("Enter visa card number: ");
        scanf("%s", cardnum);
    }

   

    printf("Enter ccc number: ");
    scanf("%s", ccc);

    while (strlen(ccc) != 3) {

        printf("Invalid ccc number!\n");

        printf("Enter ccc number: ");
        scanf("%s", ccc);
    }

    return 1;
}




void savepayment(char user_id[], int movieid, char time[],
                 int box_count, int balcony_count,
                 int normal_count, int tickets,
                 char box[], char balcony[], char normal[],
                 float box_price, float balcony_price,
                 float normal_price, float total) {

    FILE *fp;

    fp = fopen("payments.txt", "a");

    if (fp == NULL) {

        printf("Error opening file!\n");
        return;
    }

    

    fprintf(fp,  "%s\t", user_id);
    fprintf(fp, "%d\t", movieid);
    fprintf(fp, " %s\t", time);

 

    fprintf(fp, "%d\t", box_count);
    fprintf(fp, "%d\t", balcony_count);
    fprintf(fp, "%d\t", normal_count);

    fprintf(fp, "Rs. %.2f\t", box_price);
    fprintf(fp, "Rs. %.2f\t", balcony_price);
    fprintf(fp, "Rs. %.2f\t", normal_price);

    fprintf(fp, "Rs. %.2f\n", total);

    fclose(fp);
}

