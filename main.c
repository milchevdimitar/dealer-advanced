#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <math.h>
#include <time.h>
#include <assert.h>

#define MAX_SIZE 100

#define ODIT "log_file.txt"
#define INVENTORY_N "inventory_names.txt"
#define INVENTORY_A "inventory_amount.txt"

static int inventory_amount_array[MAX_SIZE];
static char inventory_names_array[MAX_SIZE][MAX_SIZE];

void print_avail_opt(){
    printf("1) Read from the log file\n");
    printf("2) Save the current inventory in the log file\n");
    printf("3) Print current inventory and item amounts\n");
    printf("4) Save\n");
    printf("5) Test\n");
    printf("6) Exit\n");
}

void get_inventory_int(){
    int i,n;
    char *token;
    char help[256];
    FILE *InputFile;
    InputFile = fopen(INVENTORY_A, "r");
    fscanf(InputFile, "%s", help);
    token = strtok(help, ",");
    i = 0;
    while(token != NULL){
        inventory_amount_array[i] = atoi(token);
        token = strtok(NULL, ",");
        i++;
    }
    n = i;
}

void get_inventory_string(){
    int i,n;
    char *token;
    char help[256];
    FILE *InputFile;
    InputFile = fopen(INVENTORY_N, "r");
    fscanf(InputFile, "%s", help);
    token = strtok(help, ",");
    while(token != NULL){
        strncpy(inventory_names_array[i], token, MAX_SIZE);
        token = strtok(NULL, ",");
        i++;
    }
    n = i;
}

void print_inventory(){
    int n = 10;
    for (int i = 0; i<n; i++){
        if(inventory_names_array[i][i] == '\0'){
            break;
        }
        else{
            printf("Available stock from %s: %d\n",inventory_names_array[i], inventory_amount_array[i]);        }
        }
}

void f_print_inventory(FILE * log_file){
    int n = 10;
    for (int i = 0; i<n; i++){
        if(inventory_amount_array[i] == '\0'){
            break;
        }
        else{
            fprintf(log_file,"Item: %s\nAmount: %d\n",inventory_names_array[i], inventory_amount_array[i]);
        }
    }
}

void read_from_log_file(FILE * log_file){
    char filename[100], temp;
    log_file = fopen(ODIT, "r");
    if (log_file == NULL){
        printf("Cannot open file \n");
        exit(0);
    }
    temp = fgetc(log_file);
    while (temp != EOF){
        printf ("%c", temp);
        temp = fgetc(log_file);
    }
    fclose(log_file);
}

void write_in_file(FILE * log_file, int tm_year, int tm_mon, int tm_mday,int tm_hour, int tm_min, int tm_sec){
    log_file = fopen(ODIT, "a");

    fprintf(log_file,"Inventory at %02d.%02d.%02d - %02d:%02d:%02d\n", tm_mday, tm_mon, tm_year, tm_hour, tm_min,tm_sec);
    f_print_inventory(log_file);
    fprintf(log_file,"--------------------------------------------------------------\n");

    fclose(log_file);
}

void save(FILE * inventory){
    inventory = fopen(INVENTORY_A, "w");
    for(int i = 0; inventory_amount_array[i] != '\0'; i++){
        fprintf(inventory, "%d,", inventory_amount_array[i]);
        printf("%d", i);
    }
    fclose(inventory);
}   

void test(){
    for(int i = 0; inventory_amount_array[i] != '\0'; i++){
        inventory_amount_array[i] = rand();
        printf("Number (%d) is randomised\n", i);
    }
}

int main(void){
    get_inventory_string();
    get_inventory_int();

    time_t T = time(NULL);
    struct tm tm = *localtime(&T);

    FILE * log_file;
    FILE * inventory_a;
    FILE * inventory_n;

    bool loop = true;
    while (loop == true){

        print_avail_opt();

        printf("Enter op:");
        int op = 0;scanf("%d", &op);

        switch (op){
        case 1: read_from_log_file(log_file);
            break;
        case 2: write_in_file(log_file, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
            break;
        case 3: print_inventory();
            break;
        case 4: save(inventory_a);
            break;
        case 5: test();
            break; 
        case 6: loop = false;
            break;
        default: printf("Not correct option\n");
            break;
        }
    }
}
