/* 
    COP 3502C PA6
    This program is written by: Sahil Narsinghani
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10 
#define MAX_NAME 25
#define EPSILON 1e-9

typedef enum {
    MODE_ADOPTION = 0,
    MODE_TRIAGE = 1
} Mode;

typedef struct Cat {
    char *name;
    char *breed;
    int age;
    int friendliness;
    int health;
    int arrival_id;
    int quarantine;
    double key;
} Cat;

typedef struct {
    Cat *arr;
    int size;
    int capacity;
    Mode mode;
} CatHeap;

typedef struct {
    Mode mode;
    char *featured_breed;
    double alpha;
    int next_arrival_id;
    CatHeap heap;
} Shelter;

int find_cat_index(CatHeap *heap, char *name);
double compute_adoption_key(Cat *c, Shelter *s);
double compute_triage_key(Cat *c);
void recompute_all_keys_and_build(Shelter *s);
void cmd_add(Shelter *s, char *name, char *breed, int age, int friend1, int health);
void cmd_update(Shelter *s, char *name, char *field, int new_value);
void cmd_remove(Shelter *s, char *name);
void cmd_peek(Shelter *s);
void cmd_serve(Shelter *s);
void cmd_mode(Shelter *s, char *mode_str);
void cmd_featured(Shelter *s, char *breed, double alpha);
void cmd_print(Shelter *s, int k);

// Helper Functions
void swapCats(Cat *a, Cat *b);
int betterCat(CatHeap *heap, Cat *left, Cat *right);
void percolateUp(CatHeap *heap, int index);
void heapify(CatHeap *heap, int index);
void freeCat(Cat *c);

int main() {
    Shelter s;
    int n;

    s.mode = MODE_ADOPTION;
    s.featured_breed = NULL;
    s.alpha = 1.0;
    s.next_arrival_id = 0;

    s.heap.size = 0;
    s.heap.capacity = INITIAL_CAPACITY;
    s.heap.mode = MODE_ADOPTION;
    s.heap.arr = (Cat *)malloc(s.heap.capacity * sizeof(Cat));

    if (scanf("%d", &n) != 1) {
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        char command[MAX_NAME];
        if (scanf("%s", command) != 1) {
            break;
        }

        if (strcmp(command, "ADD") == 0) {
            char name[MAX_NAME];
            char breed[MAX_NAME];

            int age;
            int friend;
            int health;

            scanf("%s %s %d %d %d", name, breed, &age, &friend, &health);
            cmd_add(&s, name, breed, age, friend, health);
        } else if (strcmp(command, "UPDATE") == 0) {
            char name[MAX_NAME];
            char field[MAX_NAME];
            int val;

            scanf("%s %s %d", name, field, &val);
            cmd_update(&s, name, field, val);
        } else if (strcmp(command, "REMOVE") == 0) {
            char name[MAX_NAME];
            scanf("%s", name);
        } else if (strcmp(command, "PEEK") == 0) {
            cmd_peek(&s);
        } else if (strcmp(command, "SERVE") == 0) {
            cmd_serve(&s);
        } else if (strcmp(command, "MODE") == 0) {
            char mode[MAX_NAME];
            scanf("%s", mode);
            cmd_mode(&s, mode);
        } else if (strcmp(command, "FEATURED") == 0) {
            char breed[MAX_NAME];
            double alpha;
            scanf("%s %lf", breed, &alpha);
            cmd_featured(&s, breed, alpha);
        } else if (strcmp(command, "PRINT") == 0) {
            int k;
            scanf("%d", &k);
            cmd_print(&s, k);
        } else if (strcmp(command, "QUIT") == 0) {
            break;
        }
    }
    for (int i = 0; i < s.heap.size; ++i) {
        freeCat(&s.heap.arr[i]);
    }
    free(s.heap.arr);
    free(s.featured_breed);

    return 0;
}

void swapCats(Cat *a, Cat *b) {
    Cat temp = *a;
    *a = *b;
    *b = temp;
}

int better_cat(CatHeap *heap, Cat *left, Cat *right) {
    if (heap->mode == MODE_ADOPTION) {
        if (left->key > right->key + EPSILON) {
            return 1;
        }
        if (right->key > left->key + EPSILON) {
            return 0;
        }
    } else {
        if (left->key < right->key - EPSILON) {
            return 1;
        }
        if (right->key < left->key - EPSILON) {
            return 0;
        }
    }
    int cmpName = strcmp(left->name, right->name);
    if (cmpName != 0) {
        if (cmpName < 0) {
            return 1;
        } else {
            return 0;
        }
    }

    if (left->arrival_id < right->arrival_id) {
        return 1;
    } else {
        return 0;
    }
}

void percolate_up(CatHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (better_cat(heap, &heap->arr[index], &heap->arr[parent])) {
            swapCats(&heap->arr[index], &heap->arr[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

void heapify(CatHeap *heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int bestOne = index;

    if (left < heap->size && better_cat(heap, &heap->arr[left], &heap->arr[bestOne])) {
        bestOne = left;
    }
    if (right < heap->size && better_cat(heap, &heap->arr[right], &heap->arr[bestOne])) {
        bestOne = right;
    }

    if (bestOne != index) {
        swapCats(&heap->arr[index], &heap->arr[bestOne]);
        heapify(heap, bestOne);
    }
}

void cmd_add(Shelter *s, char *name, char *breed, int age, int friend1, int health) {
    
}