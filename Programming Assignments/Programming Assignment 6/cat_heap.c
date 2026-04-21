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
            cmd_remove(&s, name);
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

int betterCat(CatHeap *heap, Cat *left, Cat *right) {
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

void percolateUp(CatHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (betterCat(heap, &heap->arr[index], &heap->arr[parent])) {
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

    if (left < heap->size && betterCat(heap, &heap->arr[left], &heap->arr[bestOne])) {
        bestOne = left;
    }
    if (right < heap->size && betterCat(heap, &heap->arr[right], &heap->arr[bestOne])) {
        bestOne = right;
    }

    if (bestOne != index) {
        swapCats(&heap->arr[index], &heap->arr[bestOne]);
        heapify(heap, bestOne);
    }
}

void freeCat(Cat *c) {
  if (c) {
    free(c->name);
    free(c->breed);
  }
}

int find_cat_index(CatHeap *heap, char *name) {
    for (int i = 0; i < heap->size; ++i) {
        if (strcmp(heap->arr[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

double compute_adoption_key(Cat *c, Shelter *s) {
    double base = (1.6 * c->friendliness) + (1.1 * c->health) - (0.7 * c->age);
    double factor = 0.0;

    if (s->featured_breed && strcmp(c->breed, s->featured_breed) == 0) {
        factor = s->alpha;
    } else {
        factor = 1.0;
    }
    
    return (base * factor) - (0.000001 * c->arrival_id);
}

double compute_triage_key(Cat *c) {
    int penalty = 0;
    if (c->age > 12) {
        penalty = c->age - 12;
    } else {
        penalty = 0;
    }

    return (100 - c->health) * (2.0 + penalty * 1.0 - 0.05 * c->friendliness);
}

void recompute_all_keys_and_build(Shelter *s) {
    for (int i = 0; i < s->heap.size; ++i) {
        if (s->mode == MODE_ADOPTION) {
            s->heap.arr[i].key = compute_adoption_key(&s->heap.arr[i], s);
        } else {
            s->heap.arr[i].key = compute_triage_key(&s->heap.arr[i]);
        }
    }

    for (int i = s->heap.size / 2 - 1; i >= 0; --i) {
        heapify(&s->heap, i);
    }
}

void cmd_add(Shelter *s, char *name, char *breed, int age, int friend1, int health) {
    if (find_cat_index(&s->heap, name) != -1) {
        printf("Name %s already exists.\n", name);
        return;
    }

    if (s->heap.size >= s->heap.capacity) {
        s->heap.capacity = s->heap.capacity * 2;
        s->heap.arr = (Cat *)realloc(s->heap.arr, s->heap.capacity * sizeof(Cat));
    }

    Cat *newCat = &s->heap.arr[s->heap.size];
    newCat->name = (char *)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(newCat->name, name);

    newCat->breed = (char *)malloc((strlen(breed) + 1) * sizeof(char));
    strcpy(newCat->breed, breed);

    newCat->age = age;
    newCat->friendliness = friend1;
    newCat->health = health;

    ++s->next_arrival_id;
    newCat->arrival_id = s->next_arrival_id;
    newCat->quarantine = 0;

    if (s->mode == MODE_ADOPTION) {
        newCat->key = compute_adoption_key(newCat, s);
    } else {
        newCat->key = compute_triage_key(newCat);
    }

    ++s->heap.size;
    percolateUp(&s->heap, s->heap.size - 1);
    printf("Added %s.\n", name);
}


void cmd_update(Shelter *s, char *name, char *field, int new_value) {
    int index = find_cat_index(&s->heap, name);
    if (index == -1) {
        printf("Cat %s not found.\n", name);
        return;
    }

    Cat *c = &s->heap.arr[index];
    if (strcmp(field, "QUARANTINE") == 0) {
        c->quarantine = new_value;
        printf("Updated %s: QUARANTINE=%d.\n", name, new_value);
    } else {
        if (strcmp(field, "AGE") == 0) {
            c->age = new_value;
        } else if (strcmp(field, "FRIEND") == 0) {
            c->friendliness = new_value;
        } else if (strcmp(field, "HEALTH") == 0) {
            c->health = new_value;
        }

        if (s->mode == MODE_ADOPTION) {
            c->key = compute_adoption_key(c, s);
        } else {
            c->key = compute_triage_key(c);
        }

        printf("Updated %s: %s = %d. Priority Adjusted.\n", name, field, new_value);
    }
    percolateUp(&s->heap, index);
    heapify(&s->heap, index);
}

void cmd_remove(Shelter *s, char *name) {
    int index = find_cat_index(&s->heap, name);
    if (index == -1) {
        printf("Cat %s not found.\n", name);
        return;
    }

    freeCat(&s->heap.arr[index]);
    printf("Removed %s.\n", name);

    s->heap.arr[index] = s->heap.arr[s->heap.size - 1];
    --s->heap.size;

    if (index < s->heap.size) {
        percolateUp(&s->heap, index);
        heapify(&s->heap, index);
    }
}

void cmd_peek(Shelter *s) {
    if (s->heap.size == 0) {
        printf("No cats available.\n");
        return;
    }

    Cat *c = &s->heap.arr[0];
    char *modeStr;

    if (s->mode == MODE_ADOPTION) {
        modeStr = "ADOPTION";
    } else {
        modeStr = "TRIAGE";
    }

    printf("Top[%s]: [%s] (key=%.2lf, name=%s, breed=%s, age=%d, friend=%d, health=%d)\n", modeStr, modeStr, c->key, c->name, c->breed, c->age, c->friendliness, c->health);
}

void cmd_serve(Shelter *s) {
    if (s->heap.size == 0) {
        printf("No adoptable cats available.\n"); 
        return;
    }

    if (s->mode == MODE_TRIAGE) {
        Cat topCat = s->heap.arr[0];
        printf("Serve now: %s (key=%.2lf, name=%s, breed=%s, age=%d, friend=%d, health=%d)\n", topCat.name, topCat.key, topCat.name, topCat.breed, topCat.age, topCat.friendliness, topCat.health);
        cmd_remove(s, topCat.name);
    } else {
        Cat *tempCatList = (Cat *)malloc(s->heap.size * sizeof(Cat));
        int tempCatListCount = 0;
        Cat servedCat;
        int found = 0;

        while (s->heap.size > 0) {
            Cat topCat = s->heap.arr[0];
            s->heap.arr[0] = s->heap.arr[s->heap.size - 1];
            --s->heap.size;
            if (s->heap.size > 0) {
                heapify(&s->heap, 0);
            }
            
            if (topCat.quarantine == 0) {
                servedCat = topCat;
                found = 1;
                break;
            } else {
                tempCatList[tempCatListCount] = topCat;
                ++tempCatListCount;
            }
        }
        for (int i = 0; i < tempCatListCount; ++i) {
            s->heap.arr[s->heap.size] = tempCatList[i];
            ++s->heap.size;
            percolateUp(&s->heap, s->heap.size - 1);
        }
        free(tempCatList);
        if (found) {
            printf("Serve now: %s (key=%.2lf, name=%s, breed=%s, age=%d, friend=%d, health=%d)\n", servedCat.name, servedCat.key, servedCat.name, servedCat.breed, servedCat.age, servedCat.friendliness, servedCat.health);
            freeCat(&servedCat);
        } else {
            printf("No adoptable cats available.\n");
        }
    }
}

void cmd_mode(Shelter *s, char *mode_str) {
    if (strcmp(mode_str, "ADOPTION") == 0) {
        s->mode = MODE_ADOPTION;
    } else if (strcmp(mode_str, "TRIAGE") == 0) {
        s->mode = MODE_TRIAGE;
    }

    s->heap.mode = s->mode;
    printf("Mode set to %s. Rebuilding priorities...\n", mode_str);
    recompute_all_keys_and_build(s);
}

void cmd_featured(Shelter *s, char *breed, double alpha) {
    free(s->featured_breed);
    if (strcmp(breed, "NONE") == 0) {
        s->featured_breed = NULL;
        s->alpha = 1.0;
        printf("Featured breed cleared. Rebuilding priorities...\n");
    } else {
        s->featured_breed = (char *)malloc((strlen(breed) + 1) * sizeof(char));
        strcpy(s->featured_breed, breed);
        s->alpha = alpha;
        printf("Featured breed set to %s with alpha=%.2lf. Rebuilding priorities...\n", breed, alpha);
    }
    recompute_all_keys_and_build(s);
}

void cmd_print(Shelter *s, int k) {
    if (s->heap.size == 0) {
        printf("No cats available.\n");
        return;
    }

    CatHeap tempHeap;
    tempHeap.capacity = s->heap.size;
    tempHeap.size = s->heap.size;
    tempHeap.mode = s->heap.mode;
    tempHeap.arr = (Cat *)malloc(tempHeap.size * sizeof(Cat));
    for (int i = 0; i < s->heap.size; ++i) {
        tempHeap.arr[i] = s->heap.arr[i];
    }

    int count = 0;

    if (k < tempHeap.size) {
        count = k;
    } else {
        count = tempHeap.size;
    }

    char *modeStr;
    if (s->mode == MODE_ADOPTION) {
        modeStr = "ADOPTION";
    } else {
        modeStr = "TRIAGE";
    }

    for (int i = 0; i < count; ++i) {
        Cat topCat = tempHeap.arr[0];
        printf("[%d] %s (key=%.2lf, %s)\n", i + 1, topCat.name, topCat.key, modeStr);

        tempHeap.arr[0] = tempHeap.arr[tempHeap.size - 1];
        --tempHeap.size;
        heapify(&tempHeap, 0);
    }
    free(tempHeap.arr);
}
