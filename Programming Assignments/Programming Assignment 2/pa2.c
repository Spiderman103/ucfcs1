
#include <stdio.h>
#include <stdlib.h>

#define MAX_SCORES 5 // Size of a cat scores array representing 5 traits
#define MAX_STR 25 // maximum name length of a cat

typedef struct Cat {
    char *name; // dynamically allocated cat name
    char *breed; // dynamically allocated breed
    int scores[MAX_SCORES]; // array of size 5 representing trait scores
    int baseScore; // sum of scores array
} Cat;

typedef struct Rivals {
    Cat *cat1; // pointer to cat1;
    Cat *cat2; // pointer to cat2;
} Rivals;

const float POSITION_BONUS[10] = {3.0, 5.0, 4.0, 6.0, 7.0, 2.0, 8.0, 1.0, 9.0, 1.5};
// bonus percentages that apply to each position

Cat *cats;
Rivals *rivals;
float bestPermScore;
int **tracker;

