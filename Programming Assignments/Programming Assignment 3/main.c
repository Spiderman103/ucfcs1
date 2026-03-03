/* COP 3502C PA3
 This program is written by: Sahil Narsinghani
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cat_s {
    int arrival; // Minutes since clinic opened (Unique)
    char *name; // Unique cat name
    int duration; // Required treatment time in minutes
} Cat;

typedef struct SLLNode_s {
    Cat *cat; // Pointer to a cat
    struct SLLNode_s *next;
} SLLNode;

typedef struct Queue_s {
    SLLNode *front;
    SLLNode *back;
} Queue;

typedef struct Stack_s {
    SLLNode *top;
} Stack;


// Queue helper functions
void init(Queue *q);
int isEmptyQueue(Queue *q);
Cat *peek(Queue *q);
void enqueue(Queue *q, Cat *c);
Cat *dequeue(Queue *q);

//Stack helper functions
void initStack(Stack *s);
int isEmptyStack(Stack *s);
Cat *peekStack(Stack *s);
void push(Stack *s, Cat *c);
Cat *pop(Stack *s);

int main() {

    Queue waitQueue;
    init(&waitQueue);

    Stack stack;
    initStack(&stack);

    int input;
    int unoIsBusy = -1;
    int dosIsBusy = -1;

    int unoIsFree = 0;
    int dosIsFree = 0;

    while (scanf("%d", &input) != -1) {
        if (input == -1) {
            break;
        }
        
        char arr[26];
        int inputDuration;
        scanf("%s %d", arr, &inputDuration);

        if (inputDuration == 0) {
            continue;
        }

        Cat *newCat = (Cat *)malloc(sizeof(Cat));
        newCat->arrival = input;
        newCat->duration = inputDuration;

        newCat->name = (char *)malloc((strlen(arr) + 1) * sizeof(char));
        strcpy(newCat->name, arr);
        enqueue(&waitQueue, newCat);
    }



    for (int i = 0; i < 480; ++i) {
      if (isEmptyQueue(&waitQueue)) {
        break;
      }

      if (unoIsBusy == -1 || i >= unoIsBusy) {
        unoIsFree = 1;
      } else {
        unoIsFree = 0;
      }

      if (dosIsBusy == -1 || i >= dosIsBusy) {
        dosIsFree = 1;
      } else {
        dosIsFree = 0;
      }

      if (unoIsFree) {
        Cat *frontCat = peek(&waitQueue);

        if (frontCat != NULL && frontCat->arrival <= i) {
          Cat *dequeuedCat = dequeue(&waitQueue);

          if ((i + frontCat->duration) > 480) {
            printf("Cannot accommodate %s\n", dequeuedCat->name);
            free(dequeuedCat->name);
            free(dequeuedCat);
            
          } else {
            printf("Doctor Uno treated %s at %d\n", dequeuedCat->name, i);
            unoIsBusy = i + dequeuedCat->duration;

            free(dequeuedCat->name);
            free(dequeuedCat);
          }
        }
      }

      if (isEmptyQueue(&waitQueue)) {
        break;
      }

      if (dosIsFree) {
        Cat *frontCat = peek(&waitQueue);

        if (frontCat != NULL && frontCat->arrival <= i) {
          Cat *dequeuedCat = dequeue(&waitQueue);

          if ((i + dequeuedCat->duration) > 480) {
            printf("Cannot accommodate %s\n", dequeuedCat->name);
            free(dequeuedCat->name);
            free(dequeuedCat);
            
          } else {
            printf("Doctor Dos treated %s at %d\n", dequeuedCat->name, i);
            dosIsBusy = i + dequeuedCat->duration;
            push(&stack, dequeuedCat);
          }
        }
      }
    }

    while (!isEmptyQueue(&waitQueue)) {
        Cat *remainingCat = dequeue(&waitQueue);
        printf("Cannot accommodate %s\n", remainingCat->name);
        free(remainingCat->name);
        free(remainingCat);
    }

    if (isEmptyStack(&stack)) {
        printf("No Exposed Cats\n");
    } else {
        printf("Exposed Cats\n");
        
        while (!isEmptyStack(&stack)) {
            Cat *cat = pop(&stack);
            printf("%s\n", cat->name);
            free(cat->name);
            free(cat);
        }
    }
    
    return 0;
} 

void init(Queue *q) {
    q->front = NULL;
    q->back = NULL;
}

int isEmptyQueue(Queue *q) {
    if (q->front == NULL) {
        return 1;
    }
    return 0;
}

Cat *peek(Queue *q) {
    if (isEmptyQueue(q)) {
        return NULL;
    }
    return q->front->cat;
}

void enqueue(Queue *q, Cat *c) {
    SLLNode *newNode = (SLLNode *)malloc(sizeof(SLLNode));
    newNode->cat = c;
    newNode->next = NULL;

    if (q->front == NULL) {
        q->front = newNode;
        q->back = newNode;
        return;
    }

    if (c->arrival < q->front->cat->arrival) {
        newNode->next = q->front;
        q->front = newNode;
        return;
    }

    SLLNode *curr = q->front;
    while (curr->next != NULL && curr->next->cat->arrival < c->arrival) {
        curr = curr->next;
    }

    newNode->next = curr->next;
    curr->next = newNode;

    if (newNode->next == NULL) {
        q->back = newNode;
    }
}

Cat *dequeue(Queue *q) {
    if (q->front == NULL) {
        return NULL;
    }

    SLLNode *temp = q->front;
    Cat *removedCat = temp->cat;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->back = NULL;
    }  

    free(temp);
    return removedCat;
}

void initStack(Stack *s) {
    s->top = NULL;
}

int isEmptyStack(Stack *s) {
    if (s->top == NULL) {
        return 1;
    }
    return 0;
}

Cat *peekStack(Stack *s) {
    if (s->top == NULL) {
        return NULL;
    }
    return s->top->cat;
}

void push(Stack *s, Cat *c) {
    SLLNode *newNode = (SLLNode *)malloc(sizeof(SLLNode));
    newNode->cat = c;
    newNode->next = s->top;
    s->top = newNode;
}

Cat *pop(Stack *s) {
    if (s->top == NULL) {
        return NULL;
    }

    SLLNode *temp = s->top;
    Cat *removedCat = temp->cat;
    s->top = s->top->next;

    free(temp);
    return removedCat;
}
