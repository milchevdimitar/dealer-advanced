#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define MAX_SIZE_OF_ARRAYS 100
#define MAX_SIZE_OF_PATH_TO_FILES 25

//Defining file names
#define ODIT         "odit/log_file.txt"
#define INVENTORY_N  "base/inventory_names.txt"
#define INVENTORY_A  "base/inventory_amount.txt"
#define ADRESS_BOOK  "external/adress_book.txt"
#define CLIENTS_BOOK "external/clients_book.txt"

//Defining settings
#define AUTO_SAVE 1
bool auto_save = AUTO_SAVE;
#define AUTO_ODIT 0
bool auto_odit = AUTO_ODIT;

//Global arrays
static int INVENTORY_AMOUNT_ARRAY[MAX_SIZE_OF_ARRAYS];
static char INVENTORY_NAMES_ARRAY[MAX_SIZE_OF_ARRAYS][MAX_SIZE_OF_ARRAYS];
static char ADRESS_BOOK_ARRAY[MAX_SIZE_OF_ARRAYS][MAX_SIZE_OF_ARRAYS];
static char CLIENTS_BOOK_ARRAY[MAX_SIZE_OF_ARRAYS][MAX_SIZE_OF_ARRAYS];

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

void print_the_linked_list(LinkedList *ll, char DATA_ARRAY[MAX_SIZE_OF_ARRAYS][MAX_SIZE_OF_ARRAYS]){
    Node *curr = ll->head;

    while (curr) {
        printf("%s ", DATA_ARRAY[curr->value]);
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

void get_inventory_string(){
    int i,n;
    char *token;
    char help[256];
    FILE *InputFile;
    InputFile = fopen(INVENTORY_N, "r");
    fscanf(InputFile, "%255s", help);
    token = strtok(help, ",");
    while(token != NULL){
        strncpy(INVENTORY_NAMES_ARRAY[i], token, MAX_SIZE_OF_ARRAYS);
        token = strtok(NULL, ",");
        i++;
    }
    n = i;
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
    for (int i = 0; i<MAX_SIZE_OF_ARRAYS; i++){
        if(INVENTORY_NAMES_ARRAY[i][i] == '\0'){
            break;
        }
        else{
            printf("Available stock from %s: %d\n",INVENTORY_NAMES_ARRAY[i], INVENTORY_AMOUNT_ARRAY[i]);        
        }
    }
}

void print_avail_opt_main_menu(){
    printf("\n");
    printf("1) Read from the log file\n");
    printf("2) Save the current inventory in the log file\n");
    printf("3) Print current inventory and item amounts\n");
    printf("4) Save\n");
    printf("5) Test\n");
    printf("6) Work with your adress book\n");
    printf("7) Work with your clients book\n");
    printf("8) Exit\n");
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
    FILE *log_file = calloc(6,sizeof(FILE));
    log_file = fopen(ODIT, "a");
    if (log_file == NULL) {
        fprintf(stderr, "error opening log file %s: %s\n",
                ODIT, strerror(errno));
    } else {
        fprintf(log_file, "Inventory at %02d.%02d.%02d - %02d:%02d:%02d\n", tm_mday, tm_mon, tm_year, tm_hour, tm_min, tm_sec);
        f_print_inventory(log_file);
        fprintf(log_file, "--------------------------------------------------------------\n");
    }
}

void save_inventory(FILE * inventory){
    inventory = fopen(INVENTORY_A, "w");
    for(int i = 0; INVENTORY_AMOUNT_ARRAY[i] != '\0'; i++){
        fprintf(inventory, "%d,", INVENTORY_AMOUNT_ARRAY[i]);
    }
    fclose(inventory);
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
    printf("1) Add the adresses from file\n");
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

// void remaining_time(int tm_mday, int tm_hour, int tm_min, int tm_sec){
//     int g_mday = 0;
//     int g_mhour = 0;
//     int g_min = 0;
//     int g_msec = 0;
//     scanf("%d:%d:%d:%d" , &g_mday,&g_mhour,&g_min,&g_msec);    
//     for(;g_mhour >= 24;g_mday++){
//         for(;)
//     }
// }

void add_from_file(LinkedList *ll, char DATA_ARRAY[MAX_SIZE_OF_ARRAYS][MAX_SIZE_OF_ARRAYS]){
    int curr_count = 0;
    int count = 0;
    int old_count = 0;
    int added_count = 0;curr_count = 0;

    for(int i = 0; (strlen(DATA_ARRAY[i])) != 0; i++){
        old_count++;
    }
    printf("Old count %d\n", old_count);
    get_clients_string();
    for(int i = 0; (strlen(DATA_ARRAY[i])) != 0; i++){
        add_element_at_the_end(ll, i);
        curr_count++;
    }
    for(int i = 0; (strlen(DATA_ARRAY[i])) != 0; i++){
        count++;
    }
    added_count = count - old_count;
    printf("%d elements were added to the list successfuly\n", added_count);
    printf("Now you have %d\n", count);
}

void print_avail_opt_clients_book(){
    printf("1) Add the clients from file\n");
    printf("2) Print your clients book\n");
    printf("3) Save current clients\n");
    printf("4) Add new client\n");
    printf("5) Exit\n");
}

void work_with_clients_book(LinkedList *ll){
    clearing_space_for_LinkedList(ll);
    int op = 0;

    FILE * clients_book_file;

    bool looping = true;
    while(looping == true){
        print_avail_opt_clients_book();
        printf("Enter op:");
        scanf("%d", &op);
            
        switch(op){
            case 1: add_from_file(ll, CLIENTS_BOOK_ARRAY);
                break;
            case 2: print_the_linked_list(ll, CLIENTS_BOOK_ARRAY);
                break;
            case 3: save_ll(clients_book_file, CLIENTS_BOOK_ARRAY, CLIENTS_BOOK);
                break;
            case 4: add_something_new(ll, CLIENTS_BOOK_ARRAY);
                    printf("Auto save is set to %d\n", auto_save);
                    if(auto_save == true){
                        save_ll(clients_book_file, CLIENTS_BOOK_ARRAY, CLIENTS_BOOK);
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

    bool looping = true;
    while(looping == true){
        print_avail_opt_adress_book();

        printf("Enter op:");
        scanf("%d", &op);
            
        switch(op){
            case 1: add_from_file(ll, ADRESS_BOOK_ARRAY);
                break;
            case 2: print_the_linked_list(ll, ADRESS_BOOK_ARRAY);
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

//Main
void main(){
    get_inventory_int();
    get_inventory_string();
    time_t T = time(NULL);
    struct tm tm = *localtime(&T);

    adress_book = calloc(2,sizeof(adress_book));
    clients_book = calloc(2,sizeof(clients_book));

    FILE * inventory_a = calloc(2,sizeof(FILE));
    FILE * inventory_n = calloc(2,sizeof(FILE));
    FILE * log_file = calloc(2,sizeof(FILE));
    
    bool looping = true;
    while (looping == true){

        print_avail_opt_main_menu();
        printf("Enter op:");
        int op = 0;scanf("%d", &op);
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
                    FILE * inventory_temp = fopen(INVENTORY_A, "r");
                    fclose(inventory_temp);
                break;
            case 3: print_inventory();
                break;
            case 4: save_inventory(inventory_a);
                break;
            case 5: test();
                break;
            case 6: work_with_adress_book(adress_book);
                break;
            case 7: work_with_clients_book(clients_book);
                break;
            case 8: printf("Auto save is set to %d\n", auto_save);
                    if(auto_save == true){
                        save_inventory(inventory_a);
                    }
                    looping = false;
                break;
            default: printf("Not correct option\n");
                break;
        }
    }
}