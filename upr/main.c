#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node_t {
    int value;
    struct node_t *next;
    struct node_t *prev;
} Node;

typedef struct {
    Node *head;
    Node *tail;
} LinkedList;

void linkedList_init(LinkedList *linkedList) {
    linkedList = (LinkedList*)calloc(1, sizeof(LinkedList));
    linkedList->head = (Node*)calloc(1, sizeof(Node));
    linkedList->tail = (Node*)calloc(1, sizeof(Node));
    linkedList->head = NULL;
    linkedList->tail = NULL;
}

bool is_empty(LinkedList *q) {
    return q->head == NULL;
}

void push_front(LinkedList *q, int new_val) {
    Node *temp = calloc(1, sizeof(Node));
    temp->value = new_val;
    if(is_empty(q)) {
        temp->next = NULL;
        temp->prev = NULL;
        q->head = temp;
        q->tail = temp;
    } else {
        temp->next = q->head;
        temp->prev = NULL;
        q->head->prev = temp;
        q->head = temp;
    }
}

void push_back(LinkedList *q, int new_val) {
    Node *temp = calloc(1, sizeof(Node));
    temp->value = new_val;
    if(is_empty(q)) {
        temp->next = NULL;
        temp->prev = NULL;
        q->head = temp;
        q->tail = temp;
    } else {
        temp->next = NULL;
        temp->prev = q->tail;
        q->tail->next = temp;
        q->tail = temp;
    }
}

Node* peek_back(LinkedList *q) {
    return q->tail;
}

Node* peek_front(LinkedList *q) {
    return q->head;
}

Node* pop_back(LinkedList *q) {
    Node *temp = q->tail;
    q->tail = q->tail->prev;
    q->tail->next = NULL;

    return temp;
}

int pop_front(LinkedList *q) {
    Node *temp = q->head;
    q->head = q->head->next;
    q->head->prev = NULL;

    int result = temp->value;
    free(temp);

    return result;
}

void push(int position, int new_val, LinkedList *list) {
    Node *new_node = calloc(1, sizeof(Node));
    new_node->value = new_val;

    Node *iterator = list->head;
    for(int i = 0; i < position; ++i) {
        iterator = iterator->next;
    }

    new_node->next = iterator;
    new_node->prev = iterator->prev;
    iterator->prev->next = new_node;
    iterator->prev = new_node;
}

Node* pop(int position, LinkedList *list) {
    Node *iterator = list->head;
    for(int i = 0; i < position; ++i) {
        iterator = iterator->next;
    }

    iterator->prev->next = iterator->next;
    iterator->next->prev = iterator->prev;

    return iterator;
}

void remove_seconds(LinkedList *list) {
    Node *iterator = list->head;
    int i = 0;

    //while (iterator->next->value != ring->first->value) {
    while (iterator->next != NULL) {
        iterator = iterator->next;
        i++;
        if (i%2 == 0) {
            iterator->prev->next = iterator->next;
            iterator->next->prev = iterator->prev;
        }
    }
}

int main() {

    LinkedList *q = calloc(1, sizeof(LinkedList));
    linkedList_init(q);

    printf("Is empty: %s\n", is_empty(q) ? "true" : "false");

    push(q, 10);
    printf("Is empty: %s\n", is_empty(q) ? "true" : "false");
    printf("Peek: %d\n", peek(q)->value);
    push(q, 20);
    printf("Popped: %d\n", pop(q)->value);
    printf("Popped: %d\n", peek(q)->value);

    return 0;
}
