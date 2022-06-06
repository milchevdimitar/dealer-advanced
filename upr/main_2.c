#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node_t{
    int value;
    struct node_t *prev;
    struct node_t *next;
} Node;

typedef struct LinkedList{
    Node *head;
    Node *tail;
} LinkedList;

void clearing_space_for_LinkedList(LinkedList *ll){
    ll->head = NULL;
    ll->tail = NULL;
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
    
    for(int i = 0; i < position; ++i){
        iterator = iterator->prev;
    }

    new_node->next = iterator;
    new_node->prev = iterator->prev;
    iterator->prev->next = new_node;
    iterator->prev = new_node;
}

void print_the_linked_list(LinkedList *ll){
    Node *curr = ll->head;

    while (curr){
        printf("%d ", curr->value);
        curr = curr->next;
    }
    printf("\n");
}

LinkedList * ll;

int main(){
    ll = calloc(1,sizeof(*ll));
    clearing_space_for_LinkedList(ll);
    add_element_at_the_beginning(ll, 10);
    add_element_at_the_end(ll, 11);
    add_element_at_the_end(ll, 12);
    print_the_linked_list(ll);
    eliminate_the_last_element(ll);
    print_the_linked_list(ll);
}