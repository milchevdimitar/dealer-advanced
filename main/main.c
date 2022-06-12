#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define MAX_SIZE_OF_ARRAYS 100
#define MAX_SIZE_OF_PATH_TO_FILES 25
#define MAX_SIZE_OF_STRINGS 15

#define MONEY_AT_BEG_INT 1000
#define MINIMUM_PROFIT_PERC 10 // 0 < MINIMUM_PROFIT_PERC < 15
#define FUCK_PRICE 100
#define DEALER_DISC 20

//Defining file names
#define ODIT         "odit/log_file.txt"
#define SALES_D		 "odit/diary.txt"
#define INVENTORY_N  "base/inv/inventory_names.txt"
#define INVENTORY_A  "base/inv/inventory_amount.txt"
#define INVENTORY_P  "base/inv/inventory_prices.txt"
#define DEALER_L     "base/about_dealer.txt"
#define ADRESS_BOOK  "external/adr/adress_book.txt"
#define CLIENTS_BOOK "external/cli/clients_book.txt"
#define CLIENTS_DISC "external/cli/clients_disc.txt"

//Defining settings
bool auto_save = true;
bool auto_odit = true;
bool secure_insert = true;
bool money_at_beg = true;
bool end_save = false; //dangerous

//Global arrays - cache arrays

static intmax_t INVENTORY_AMOUNT_ARRAY[MAX_SIZE_OF_ARRAYS];
static intmax_t INVENTORY_PRICES_ARRAY[MAX_SIZE_OF_ARRAYS];
static char     INVENTORY_NAMES_ARRAY[MAX_SIZE_OF_ARRAYS][MAX_SIZE_OF_ARRAYS];

static char     ADRESS_BOOK_ARRAY[MAX_SIZE_OF_ARRAYS][MAX_SIZE_OF_ARRAYS];
static intmax_t ADRESS_MEETUP_HOURS[MAX_SIZE_OF_ARRAYS];
static intmax_t ADRESS_MEETUP_MINS[MAX_SIZE_OF_ARRAYS];

static char     CLIENTS_BOOK_ARRAY[MAX_SIZE_OF_ARRAYS][MAX_SIZE_OF_ARRAYS];
static intmax_t CLIENTS_DISCOUNTS[MAX_SIZE_OF_ARRAYS];


//Dealer
struct dealer{
    int money;
    int check_ok;
    char *name;
}my_og;

//Linked List things and functions for "Adress book"
typedef struct node_t{
    int value;
    struct node_t *prev;
    struct node_t *next;
}Node;

typedef struct LinkedList{
    Node *head;
    Node *tail;
}LinkedList;

//Creating double linked list for our adress book and clients book
LinkedList *adress_book;
LinkedList *clients_book;

void clearing_space_for_LinkedList(LinkedList *Linkedlist){
    Linkedlist->head = NULL;
    Linkedlist->tail = NULL;
}

int empty_check(LinkedList *ll){
    if(ll->head == NULL){
        return 1;
    }
    else{
        return 0;
    }
}

void add_element_at_the_beginning(LinkedList *ll, int new_value){
    Node *temporary = calloc(1,sizeof(Node));
    temporary->value = new_value;

    if (empty_check(ll)){
        temporary->next = NULL;
        temporary->prev = NULL;
        ll->head = temporary;
        ll->tail = temporary;
    }
    else{
        temporary->next = ll->head;
        temporary->prev = NULL;
        ll->head->prev = temporary;
        ll->head = temporary;
    }
}

void add_element_at_the_end(LinkedList *ll, int new_value){
    Node *temporary = calloc(1,sizeof(Node));
    temporary->value = new_value;

    if (empty_check(ll)){
        temporary->next = NULL;
        temporary->prev = NULL;
        ll->head = temporary;
        ll->tail = temporary;
    }
    else{
        temporary->next = NULL;
        temporary->prev = ll->tail;
        ll->tail->next = temporary;
        ll->tail = temporary;
    }
}


int eliminate_the_first_element(LinkedList *ll){
    Node *temporary = ll->head;
    ll->head = ll->head->next;
    ll->head->prev = NULL;

    int result = temporary->value;
    free(temporary);
    return result;
}

int eliminate_the_last_element(LinkedList *ll){
    Node *temporary = ll->tail;
    ll->tail = ll->tail->prev;
    ll->tail->next = NULL;

    free(temporary);
    return temporary->value;
}

void eliminate_the_single_element(LinkedList *ll){
    Node *temporary = ll->head;
    ll->head = NULL;
    ll->tail = NULL;
    free(temporary);
}

int add_element_on_another_spot(LinkedList *ll, int new_val){
    int position = 0;
    scanf("%d", &position);

    Node *new_node = calloc(1,sizeof(Node));    
    new_node->value = new_val;
    
    Node *iterator = ll->head;
    
    for(int i = 0; i < position; ++i) {
        iterator = iterator->prev;
    }

    new_node->next = iterator;
    new_node->prev = iterator->prev;
    iterator->prev->next = new_node;
    iterator->prev = new_node;
}

void print_the_linked_list(LinkedList *ll, char DATA_ARRAY[MAX_SIZE_OF_ARRAYS][MAX_SIZE_OF_ARRAYS], bool clients_loop, bool adress_loop){
    Node *curr = ll->head;
    while (curr) {
        if(clients_loop == true){
            printf("%s | Discount: +%d%\n", DATA_ARRAY[curr->value], CLIENTS_DISCOUNTS[curr->value]);
        } else if(adress_loop == true) {
            printf("%s\n", DATA_ARRAY[curr->value]);    
        }
        curr = curr->next;
    }
    printf("\n");
}

//File things and functions
void get_inventory_int(){
    int i,n;
    char *token;
    char help[256];
    FILE *InputFile;
    InputFile = fopen(INVENTORY_A, "r");
    fscanf(InputFile, "%255s", help);
    token = strtok(help, ",");
    i = 0;
    while(token != NULL){
        INVENTORY_AMOUNT_ARRAY[i] = atoi(token);
        token = strtok(NULL, ",");
        i++;
    }
    n = i;
    fclose(InputFile);
}

void get_clidisc_int(){
    int i,n;
    char *token;
    char help[256];
    FILE *InputFile;
    InputFile = fopen(CLIENTS_DISC, "r");
    fscanf(InputFile, "%255s", help);
    token = strtok(help, ",");
    i = 0;
    while(token != NULL){
        if(atoi(token) <= MINIMUM_PROFIT_PERC){
            CLIENTS_DISCOUNTS[i] = MINIMUM_PROFIT_PERC;
        } else {
            CLIENTS_DISCOUNTS[i] = atoi(token);
        }
        token = strtok(NULL, ",");
        i++;
    }

    n = i;
    fclose(InputFile);
}

void get_inventory_prices_int(){
    int i,n;
    char *token;
    char help[256];
    FILE *InputFile;
    InputFile = fopen(INVENTORY_P, "r");
    fscanf(InputFile, "%255s", help);
    token = strtok(help, ",");
    i = 0;
    while(token != NULL){
        INVENTORY_PRICES_ARRAY[i] = atoi(token);
        token = strtok(NULL, ",");
        i++;
    }
    n = i;
    fclose(InputFile);
}

void get_inventory_string(){
    char *token;
    char help[256];
    FILE *InputFile;
    InputFile = fopen(INVENTORY_N, "r");
    fscanf(InputFile, "%s", help);
    token = strtok(help, ",");
    for(int i = 0;token != NULL;i++){
        strncpy(INVENTORY_NAMES_ARRAY[i], token, MAX_SIZE_OF_ARRAYS);
        token = strtok(NULL, ",");
    }
    fclose(InputFile);
}

void get_adress_string(){
    int i = 0;
    int n = 0;
    char *token;
    char help[256];
    FILE *InputFile;
    InputFile = fopen(ADRESS_BOOK, "r");
    fscanf(InputFile, "%255s", help);
    token = strtok(help, ",");
    while(token != NULL){
        strncpy(ADRESS_BOOK_ARRAY[i], token, MAX_SIZE_OF_ARRAYS);
        token = strtok(NULL, ",");
        i++;
    }
    n = i;

    fclose(InputFile);
}

void get_clients_string(){
    int i = 0;
    int n = 0;
    char *token;
    char help[256];
    FILE *InputFile;
    InputFile = fopen(CLIENTS_BOOK, "r");
    fscanf(InputFile, "%255s", help);
    token = strtok(help, ",");
    while(token != NULL){
        strncpy(CLIENTS_BOOK_ARRAY[i], token, MAX_SIZE_OF_ARRAYS);
        token = strtok(NULL, ",");
        i++;
    }
    n = i;

    fclose(InputFile);
}

void print_inventory(){
    int n = MAX_SIZE_OF_ARRAYS;
    for (int i = 0; i<n; i++){
        if((strlen(INVENTORY_NAMES_ARRAY[i])) == 0){
            printf("Element %d is NULL. Breaking ...", i+1);
            break;
        }
        else{
            printf("%s: %d | price: %d$\n",INVENTORY_NAMES_ARRAY[i], INVENTORY_AMOUNT_ARRAY[i], INVENTORY_PRICES_ARRAY[i]);
        }
    }
}

void print_avail_opt_main_menu(){
    printf("\n");
    printf("1) Work with your inventory\n");
    printf("2) Work with your adress book\n");
    printf("3) Work with your clients book\n");
    printf("4) Work with your dealer\n");
    printf("5) Main save\n");
    printf("6) Exit\n");
}

void print_avail_adress(){
    for (int i = 0; i<MAX_SIZE_OF_ARRAYS; i++){
        if(ADRESS_BOOK_ARRAY[i][i] != '\0'){
            printf("Adress %d: %s\n", i+1, ADRESS_BOOK_ARRAY[i]); 
        }
        else{
            break;
        }
    }
}

void f_print_inventory(FILE * log_file){
    int n = MAX_SIZE_OF_ARRAYS;
    for (int i = 0; i<n; i++){
        if(INVENTORY_AMOUNT_ARRAY[i] == '\0'){
            break;
        }
        else{
           fprintf(log_file,"Item: %s\nAmount: %d\n",INVENTORY_NAMES_ARRAY[i], INVENTORY_AMOUNT_ARRAY[i]);
        }
    }
    fclose(log_file);
}

void read_from_log_file(FILE * log_file){
    char filename[100], temp;
    log_file = fopen(ODIT, "r");
    if (log_file == NULL){
        printf("Cannot open file \n");
        exit(0);
    }
    temp = fgetc(log_file);
    for(int i = 1; temp != EOF;){
        if(temp == '\n'){
            printf ("%c", temp);
            printf ("|Row: %d|", i);
            i++;
        }else{
            printf ("%c", temp);    
        }
        temp = fgetc(log_file);
    }
    fclose(log_file);
}

void write_in_the_log_file(int tm_year, int tm_mon, int tm_mday, int tm_hour, int tm_min, int tm_sec){
    FILE *log_file = fopen(ODIT, "a");
    if (log_file == NULL) {
        fprintf(stderr, "error opening log file %s: %s\n",
                ODIT, strerror(errno));
    } else {
        fprintf(log_file, "Inventory at %02d.%02d.%02d - %02d:%02d:%02d\n", tm_mday, tm_mon, tm_year, tm_hour, tm_min, tm_sec);
        f_print_inventory(log_file);
    }
}

void save_inventory(FILE * inventory){
    inventory = fopen(INVENTORY_A, "w");
    for(int i = 0; INVENTORY_AMOUNT_ARRAY[i] != '\0'; i++){
        fprintf(inventory, "%d,", INVENTORY_AMOUNT_ARRAY[i]);
    }
    fclose(inventory);
    printf("Infile info about amounts correctly overwritten\n");
}   


void save_inventory_prices(FILE * inventory){
    inventory = fopen(INVENTORY_P, "w");
    for(int i = 0; INVENTORY_PRICES_ARRAY[i] != '\0'; i++){
        fprintf(inventory, "%d,", INVENTORY_PRICES_ARRAY[i]);
    }
    fclose(inventory);
    printf("Infile info about prices correctly overwritten\n");

}

void save_inventory_names(){
    FILE * inventory = fopen(INVENTORY_N, "w");
    for(int i = 0; strlen(INVENTORY_NAMES_ARRAY[i]) != 0; i++){
        fprintf(inventory, "%s,", INVENTORY_NAMES_ARRAY[i]);
    }
    fclose(inventory);
    printf("Infile info about names correctly overwritten\n");

}

//Random amounts for testing "saving" feature
void test(){
    for(int i = 0; INVENTORY_AMOUNT_ARRAY[i] != '\0'; i++){
        INVENTORY_AMOUNT_ARRAY[i] = rand();
        printf("Number (%d) is randomised\n", i);
    }
}

//Working with the adress book
void print_avail_opt_adress_book(){
    printf("1) Add the adresses from file(Can be done only once)\n");
    printf("2) Print your adress book\n");
    printf("3) Save current adress\n");
    printf("4) Add new adress\n");
    printf("5) Exit\n");
}

void save_ll(FILE * book_file, char DATA_ARRAY[MAX_SIZE_OF_ARRAYS][MAX_SIZE_OF_ARRAYS], char file_path[MAX_SIZE_OF_PATH_TO_FILES]){
    book_file = fopen(file_path, "w");
    for(int i = 0; (strlen(DATA_ARRAY[i])) != 0; i++){
        fprintf(book_file, "%s,", DATA_ARRAY[i]);
    }
    fclose(book_file);
}

void add_something_new(LinkedList *ll, char DATA_ARRAY[MAX_SIZE_OF_ARRAYS][MAX_SIZE_OF_ARRAYS]){
    int i = 0;
    for(;(strlen(DATA_ARRAY[i])) != 0; i++){}
    add_element_at_the_end(ll, i);
    printf("Enter the new adress or client:");
    scanf("%s", &DATA_ARRAY[i]);
}

void add_from_file(LinkedList *ll, char DATA_ARRAY[MAX_SIZE_OF_ARRAYS][MAX_SIZE_OF_ARRAYS]){
    int curr_count = 0;
    int count = 0;
    int old_count = 0;
    int added_count = 0;

    for(int i = 0; (strlen(DATA_ARRAY[i])) != 0; i++){
        old_count++;
    }
    printf("Old count %d\n", old_count);
    for(int i = 0; (strlen(DATA_ARRAY[i])) != 0; i++){
        add_element_at_the_end(ll, i);
        curr_count++;
    }
    for(int i = 0; (strlen(DATA_ARRAY[i])) != 0; i++){
        count += 2;
    }
    added_count = count - old_count;
    printf("%d elements were added to the list successfuly\n", added_count);
    count /= 2;
    printf("Now you have %d\n", count);
}

void print_avail_opt_clients_book(){
    printf("1) Add the clients from file(Can be done only once)\n");
    printf("2) Print your clients book\n");
    printf("3) Save current clients\n");
    printf("4) Add new client\n");
    printf("5) Exit\n");
}

void save_cli_disc(){
    FILE * cli = fopen(CLIENTS_DISC, "w");
    for(int i = 0; CLIENTS_DISCOUNTS[i] != '\0'; i++){
        fprintf(cli, "%d,", CLIENTS_DISCOUNTS[i]);
        printf("%d,", CLIENTS_DISCOUNTS[i]);
    }
    
    fclose(cli);
    printf("Infile info about clients discounts correctly overwritten\n");
}

void work_with_clients_book(LinkedList *ll){
    clearing_space_for_LinkedList(ll);
    get_clidisc_int();
    int op = 0;

    FILE * clients_book_file;

    int firstcaseexec = 0;

    bool looping = true;
    while(looping == true){
        print_avail_opt_clients_book();
        printf("Enter op:");
        scanf("%d", &op);
            
        switch(op){
            case 1: if(firstcaseexec == 0){
                        get_clients_string();
                        add_from_file(ll, CLIENTS_BOOK_ARRAY);
                        firstcaseexec++;
                    }else if(secure_insert == false){
                        get_clients_string();
                        add_from_file(ll, CLIENTS_BOOK_ARRAY);
                    }else{ 
                        printf("You already done it once\n");
                        printf("Secure insert is set to %d\n", secure_insert);
                    }
                break;
            case 2: print_the_linked_list(ll, CLIENTS_BOOK_ARRAY, true, false);
                break;
            case 3: save_ll(clients_book_file, CLIENTS_BOOK_ARRAY, CLIENTS_BOOK);
                    save_cli_disc();
                break;
            case 4: add_something_new(ll, CLIENTS_BOOK_ARRAY);
                    int i;
                    for(i = 0; strlen(CLIENTS_BOOK_ARRAY[i]) != 0; i++){}
                    i--;
                    printf("How much discount do u give to your client:");
                    scanf("%d", &CLIENTS_DISCOUNTS[i]);
                    printf("Auto save is set to %d\n", auto_save);
                    if(auto_save == true){
                        save_ll(clients_book_file, CLIENTS_BOOK_ARRAY, CLIENTS_BOOK);
                        save_cli_disc();
                    }
                break;
            case 5: printf("Auto save is set to %d\n", auto_save);
                    if(auto_save == true){
                        save_ll(clients_book_file, CLIENTS_BOOK_ARRAY, CLIENTS_BOOK);
                    }
                    looping = false;
                break;
            }
    }
}

void work_with_adress_book(LinkedList *ll){
    clearing_space_for_LinkedList(ll);
    int op = 0;

    FILE * adress_book_file;

    int firstcaseexec = 0;

    bool looping = true;
    while(looping == true){
        print_avail_opt_adress_book();

        printf("Enter op:");
        scanf("%d", &op);
            
        switch(op){
            case 1: if(firstcaseexec == 0){
                        get_adress_string();
                        add_from_file(ll, ADRESS_BOOK_ARRAY);
                        firstcaseexec++;
                    }else if(secure_insert == false){
                        get_adress_string();
                        add_from_file(ll, ADRESS_BOOK_ARRAY);
                    }else{ 
                        printf("You already done it once\n");
                        printf("Secure insert is set to %d\n", secure_insert);
                    }
                break;
            case 2: print_the_linked_list(ll, ADRESS_BOOK_ARRAY, false, true);
                break;
            case 3: save_ll(adress_book_file, ADRESS_BOOK_ARRAY, ADRESS_BOOK);
                break;
            case 4: add_something_new(ll, ADRESS_BOOK_ARRAY);
                    printf("Auto save is set to %d\n", auto_save);
                    if(auto_save == true){
                        save_ll(adress_book_file, ADRESS_BOOK_ARRAY, ADRESS_BOOK);
                    }
                break;
            case 5: printf("Auto save is set to %d\n", auto_save);
                    if(auto_save == true){
                        save_ll(adress_book_file, ADRESS_BOOK_ARRAY, ADRESS_BOOK);
                    }
                    looping = false;
                break;
            }
    }
}

void print_avail_opt_inventory(){
    printf("\n");
    printf("1) Read the inventory log file\n");
    printf("2) Save the current inventory in the log file\n");
    printf("3) Print the current inventory on the screen\n");
    printf("4) Save\n");
    printf("5) Free restock\n");
    printf("6) Real restock\n");
    printf("7) Sale\n");
	printf("8) Add new product\n");
    printf("9) Remove product\n");
    printf("10) Test - for debug\n");
    printf("11) Exit\n");
}

void free_restock(){
    print_inventory();

    printf("\nWhat do u want to restock: ");
    char temp_string_for_name[MAX_SIZE_OF_STRINGS];
    scanf("%s", &temp_string_for_name);
    
    printf("How much do u want to restock: ");
    int temp_val_for_restock = 0;
    scanf("%d", &temp_val_for_restock);

    for(int i = 0; (strlen(INVENTORY_NAMES_ARRAY[i])) != 0; i++){
        if(strcmp(temp_string_for_name, INVENTORY_NAMES_ARRAY[i]) == 0){
            INVENTORY_AMOUNT_ARRAY[i] += temp_val_for_restock;
            printf("Now u have %d from %s\n", INVENTORY_AMOUNT_ARRAY[i], INVENTORY_NAMES_ARRAY[i]);
        break;
        }
    }
}

void restock(){
    print_inventory();

    printf("\nWhat do u want to restock: ");
    char temp_string_for_name[MAX_SIZE_OF_STRINGS];
    scanf("%s", &temp_string_for_name);
    
    printf("How much do u want to restock: ");
    int temp_val_for_restock = 0;
    scanf("%d", &temp_val_for_restock);

    for(int i = 0; (strlen(INVENTORY_NAMES_ARRAY[i])) != 0; i++){
        if(strcmp(temp_string_for_name, INVENTORY_NAMES_ARRAY[i]) == 0){
            if(temp_val_for_restock*INVENTORY_PRICES_ARRAY[i] < my_og.money){
                INVENTORY_AMOUNT_ARRAY[i] += temp_val_for_restock;
				my_og.money -= (temp_val_for_restock * INVENTORY_PRICES_ARRAY[i]) -
                (DEALER_DISC*(temp_val_for_restock * INVENTORY_PRICES_ARRAY[i]))/100;
				printf("Now u have %d from %s and %d in cash\n", INVENTORY_AMOUNT_ARRAY[i], INVENTORY_NAMES_ARRAY[i], my_og.money);
            break;
            } else {
                printf("U dont have so much money silly\n");
            break;
            }
        }
    }
}

void dealer__save(){
    FILE * dealer = fopen(DEALER_L, "w");
    fprintf(dealer,"%d  %d  %s", my_og.money, my_og.check_ok, my_og.name);
    fclose(dealer);
    printf("Infile info about dealer correctly overwritten\n");
}

void dealer__check(){
    printf("Name: %s | Money: %d$\n", my_og.name, my_og.money);
    printf("Startup money set to %d\n", money_at_beg);
    dealer__save();
}

void dealer__init(){
    my_og.name = calloc(MAX_SIZE_OF_STRINGS, sizeof(const char));
    FILE * dealer = fopen(DEALER_L, "r");
    fscanf(dealer, "%d  %d  %255s", &my_og.money, &my_og.check_ok, my_og.name);
    dealer__check();

    if(money_at_beg == true && my_og.check_ok == 0){
        my_og.money = MONEY_AT_BEG_INT;
        my_og.check_ok = 1;
        dealer__save();
    }
    fclose(dealer);
}

void dealer__moneypowerup(){
    printf("How much do u want to take from them: ");
    int credit = 0;
    scanf("%d", &credit);
    my_og.money += credit;
    credit /= FUCK_PRICE;
    printf("U have to be fucked %d times\n", credit);
}
void print_dealer_menu(){
    printf("1) Print info about dealer\n");
    printf("2) Get money from fucking\n");
    printf("3) Exit\n");
}

void dealer__operate(){
    bool looping = true;
    while(looping){
        int op = 0;
        print_dealer_menu();
        scanf("%d", &op);
        switch (op){
        case 1: dealer__check();
            break;
        case 2: dealer__moneypowerup();
                dealer__check();
        default: looping = false;
            break;
        }
    }
}

void main__save(){
    FILE * inventory_a;
    FILE * inventory_p;
    save_inventory(inventory_a);
    save_inventory_prices(inventory_p);
    dealer__save();
}

int sale(){
    print_inventory();
    time_t T = time(NULL);
    struct tm tm = *localtime(&T);

    printf("\nWhat do u want to sale: ");
    char temp_string_for_prname[MAX_SIZE_OF_STRINGS];
    scanf("%s", &temp_string_for_prname);
    
    printf("How much do u want to sale: ");
    int temp_val_for_sale = 0;
    scanf("%d", &temp_val_for_sale);

    printf("Who do u want to sale %d from %s to: ", temp_val_for_sale,temp_string_for_prname);
    char temp_string_for_cliname[MAX_SIZE_OF_STRINGS];
    scanf("%s", &temp_string_for_cliname);
    getchar();

    int ovr_price = 0;
    int profit = 0;
    
    for(int i = 0; (strlen(INVENTORY_NAMES_ARRAY[i])) != 0; i++){
        if((strcmp(temp_string_for_prname, INVENTORY_NAMES_ARRAY[i])) == 0){
       		if(INVENTORY_AMOUNT_ARRAY[i] >= temp_val_for_sale){
                int j;
                for(j = 0; strlen(CLIENTS_BOOK_ARRAY[j]) != 0; j++){
                    if(strcmp(CLIENTS_BOOK_ARRAY[j], temp_string_for_cliname) == 0){
                        ovr_price = INVENTORY_PRICES_ARRAY[i];
                        ovr_price -= (INVENTORY_PRICES_ARRAY[i] * CLIENTS_DISCOUNTS[j]) / 100;
                        profit = INVENTORY_PRICES_ARRAY[i] - ovr_price;
                    }         
                }
    			
                INVENTORY_AMOUNT_ARRAY[i] -= temp_val_for_sale;
				my_og.money += ovr_price;
				
                printf("You have succesfully sold %d from %s to %s\n",
                temp_val_for_sale, INVENTORY_NAMES_ARRAY[i], temp_string_for_cliname);
                
                if(auto_odit == true){
                	FILE * sales_f = fopen(SALES_D, "a");
				    fprintf(sales_f, "You have succesfully sold %d from %s to %s at %02d.%02d.%02d - %02d:%02d:%02d | Profit:%d$ \n",
                    temp_val_for_sale, INVENTORY_NAMES_ARRAY[i], temp_string_for_cliname,
                    1900+tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
                    profit);
                    fclose(sales_f);
                
                write_in_the_log_file(
                        tm.tm_year + 1900, 
                        tm.tm_mon + 1, 
                        tm.tm_mday, 
                        tm.tm_hour, 
                        tm.tm_min, 
                        tm.tm_sec
                );
                }
            break;
			} else printf("Im not a bank bro\n");
		} 
	}
}

void cache_add_product(){
    int i = 0;
    for(;(strlen(INVENTORY_NAMES_ARRAY[i])) != 0; i++){}
    printf("Enter the product's name:");
    scanf("%s", &INVENTORY_NAMES_ARRAY[i]);
    printf("Enter the product's price:");
    scanf("%d", &INVENTORY_PRICES_ARRAY[i]);
    printf("How much is available in stock:");
    scanf("%d", &INVENTORY_AMOUNT_ARRAY[i]);
}

void cache_remove_product(void) {
    int i, j;
    getchar();
    printf("Enter the product's name u want to remove:");
    char temp_string_for_prname[MAX_SIZE_OF_STRINGS + 1];
    if (!fgets(temp_string_for_prname, sizeof temp_string_for_prname, stdin)){
        return;
    }

    size_t len = strlen(temp_string_for_prname);
    
    if (len > 0 && temp_string_for_prname[len - 1] == '\n'){
        temp_string_for_prname[--len] = '\0';
    }
    if (len == 0){
        return;
    }
    for (i = 0; INVENTORY_NAMES_ARRAY[i][0] != '\0';) {
        if (strcmp(temp_string_for_prname, INVENTORY_NAMES_ARRAY[i]) == 0) {
            for (j = i; j < MAX_SIZE_OF_ARRAYS - 1; j++) {
                strcpy(INVENTORY_NAMES_ARRAY[j], INVENTORY_NAMES_ARRAY[j + 1]);
                INVENTORY_PRICES_ARRAY[j] = INVENTORY_PRICES_ARRAY[j + 1];
                INVENTORY_AMOUNT_ARRAY[j] = INVENTORY_AMOUNT_ARRAY[j + 1];
                if (INVENTORY_NAMES_ARRAY[j][0] == '\0'){
                    break;
                }
            }
            INVENTORY_NAMES_ARRAY[j][0] = '\0';
        } else {
            i++;
        }
    }
}

void work_with_inventory(){
    get_inventory_int();
    get_inventory_prices_int();
    get_inventory_string();     
    get_clidisc_int();

    FILE * inventory_a = calloc(2,sizeof(FILE));
    FILE * inventory_n = calloc(2,sizeof(FILE));
    FILE * inventory_p = calloc(2,sizeof(FILE));
    FILE * log_file = calloc(2,sizeof(FILE));

    time_t T = time(NULL);
    struct tm tm = *localtime(&T);

    bool looping = true;
    while (looping == true){

        print_avail_opt_inventory();
        printf("Enter op:");
        int op = 0;
        scanf("%d", &op);
        switch (op){
            case 1: read_from_log_file(log_file);
                break;
            case 2: write_in_the_log_file(
                        tm.tm_year + 1900, 
                        tm.tm_mon + 1, 
                        tm.tm_mday, 
                        tm.tm_hour, 
                        tm.tm_min, 
                        tm.tm_sec
                    );
                break;    
            case 3: print_inventory();
                break;
            case 4: save_inventory(inventory_a);
                    save_inventory_prices(inventory_p);
                    save_inventory_names();
                    dealer__save();
                break;
            case 5: free_restock();
                break;
            case 6: restock();
                break;
			case 7: sale();
				break;
            case 8: cache_add_product();
                break;
            case 9: cache_remove_product();
                break;
            case 10: test();
                break;
            case 11: looping = false;
                break;
        }
    }
}

//Main
int main(void){
    adress_book = calloc(2,sizeof(adress_book));
    clients_book = calloc(2,sizeof(clients_book));

    dealer__init();

    bool looping = true;
    while (looping == true){

        print_avail_opt_main_menu();
        printf("Enter op:");
        int op = 0;
        scanf("%d", &op);
        switch (op){
            case 1: work_with_inventory();
                break;
            case 2: work_with_adress_book(adress_book);
                break;
            case 3: work_with_clients_book(clients_book);
                break;
            case 4: dealer__operate();
                break;
            case 5: main__save();
                break;
            case 6: looping = false;
                if(end_save == true){
                    main__save();
                }
                break;
            default: printf("Not correct option\n");
                break;
        }
    }
}