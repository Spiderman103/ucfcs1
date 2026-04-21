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

/* Function Prototypes */
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

/* Internal Helper Functions */
void swap_cats(Cat *a, Cat *b);
int better_cat(CatHeap *heap, Cat *left, Cat *right);
void percolate_up(CatHeap *heap, int index);
void heapify(CatHeap *heap, int index);
void free_cat_contents(Cat *c);

/* --- Main Logic --- */

int main() {
    Shelter s;
    s.mode = MODE_ADOPTION;
    s.featured_breed = NULL;
    s.alpha = 1.0;
    s.next_arrival_id = 0;
    
    s.heap.size = 0;
    s.heap.capacity = INITIAL_CAPACITY;
    s.heap.mode = MODE_ADOPTION;
    s.heap.arr = (Cat *)malloc(sizeof(Cat) * s.heap.capacity);

    int n;
    if (scanf("%d", &n) != 1) return 0;

    for (int i = 0; i < n; i++) {
        char command[MAX_NAME];
        if (scanf("%s", command) != 1) break;

        if (strcmp(command, "ADD") == 0) {
            char name[MAX_NAME], breed[MAX_NAME];
            int age, friend, health;
            scanf("%s %s %d %d %d", name, breed, &age, &friend, &health);
            cmd_add(&s, name, breed, age, friend, health);
        } else if (strcmp(command, "UPDATE") == 0) {
            char name[MAX_NAME], field[MAX_NAME];
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
            char mode_str[MAX_NAME];
            scanf("%s", mode_str);
            cmd_mode(&s, mode_str);
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

    /* Cleanup */
    for (int i = 0; i < s.heap.size; i++) {
        free_cat_contents(&s.heap.arr[i]);
    }
    free(s.heap.arr);
    free(s.featured_breed);

    return 0;
}

/* --- Heap Implementation --- */

void swap_cats(Cat *a, Cat *b) {
    Cat temp = *a;
    *a = *b;
    *b = temp;
}

int better_cat(CatHeap *heap, Cat *left, Cat *right) {
    if (heap->mode == MODE_ADOPTION) {
        if (left->key > right->key + EPSILON) return 1;
        if (right->key > left->key + EPSILON) return 0;
    } else {
        if (left->key < right->key - EPSILON) return 1;
        if (right->key < left->key - EPSILON) return 0;
    }
    
    /* Tie-breakers */
    int name_cmp = strcmp(left->name, right->name);
    if (name_cmp != 0) return name_cmp < 0;
    return left->arrival_id < right->arrival_id;
}

void percolate_up(CatHeap *heap, int index) {
    while (index > 0) {
        int p = (index - 1) / 2;
        if (better_cat(heap, &heap->arr[index], &heap->arr[p])) {
            swap_cats(&heap->arr[index], &heap->arr[p]);
            index = p;
        } else break;
    }
}

void heapify(CatHeap *heap, int index) {
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    int best = index;

    if (left < heap->size && better_cat(heap, &heap->arr[left], &heap->arr[best]))
        best = left;
    if (right < heap->size && better_cat(heap, &heap->arr[right], &heap->arr[best]))
        best = right;

    if (best != index) {
        swap_cats(&heap->arr[index], &heap->arr[best]);
        heapify(heap, best);
    }
}

/* --- Command Handlers --- */

void cmd_add(Shelter *s, char *name, char *breed, int age, int friend1, int health) {
    if (find_cat_index(&s->heap, name) != -1) {
        printf("Name %s already exists.\n", name);
        return;
    }

    if (s->heap.size >= s->heap.capacity) {
        s->heap.capacity *= 2;
        s->heap.arr = (Cat *)realloc(s->heap.arr, sizeof(Cat) * s->heap.capacity);
    }

    Cat *c = &s->heap.arr[s->heap.size];
    c->name = (char *)malloc(strlen(name) + 1);
    strcpy(c->name, name);
    c->breed = (char *)malloc(strlen(breed) + 1);
    strcpy(c->breed, breed);
    c->age = age;
    c->friendliness = friend1;
    c->health = health;
    c->arrival_id = s->next_arrival_id++;
    c->quarantine = 0;
    
    c->key = (s->mode == MODE_ADOPTION) ? compute_adoption_key(c, s) : compute_triage_key(c);
    
    s->heap.size++;
    percolate_up(&s->heap, s->heap.size - 1);
    printf("Added %s.\n", name);
}

void cmd_update(Shelter *s, char *name, char *field, int new_value) {
    int idx = find_cat_index(&s->heap, name);
    if (idx == -1) {
        printf("Cat %s not found.\n", name);
        return;
    }

    Cat *c = &s->heap.arr[idx];
    if (strcmp(field, "QUARANTINE") == 0) {
        c->quarantine = new_value;
        printf("Updated %s: QUARANTINE=%d.\n", name, new_value);
    } else {
        if (strcmp(field, "AGE") == 0) c->age = new_value;
        else if (strcmp(field, "FRIEND") == 0) c->friendliness = new_value;
        else if (strcmp(field, "HEALTH") == 0) c->health = new_value;
        
        c->key = (s->mode == MODE_ADOPTION) ? compute_adoption_key(c, s) : compute_triage_key(c);
        printf("Updated %s: %s=%d. Priority adjusted.\n", name, field, new_value);
    }
    
    percolate_up(&s->heap, idx);
    heapify(&s->heap, idx);
}

void cmd_remove(Shelter *s, char *name) {
    int idx = find_cat_index(&s->heap, name);
    if (idx == -1) {
        printf("Cat %s not found.\n", name);
        return;
    }

    free_cat_contents(&s->heap.arr[idx]);
    printf("Removed %s.\n", name);

    s->heap.arr[idx] = s->heap.arr[s->heap.size - 1];
    s->heap.size--;

    if (idx < s->heap.size) {
        percolate_up(&s->heap, idx);
        heapify(&s->heap, idx);
    }
}

void cmd_peek(Shelter *s) {
    if (s->heap.size == 0) {
        printf("No cats available.\n");
        return;
    }
    Cat *c = &s->heap.arr[0];
    const char *mode = (s->mode == MODE_ADOPTION) ? "ADOPTION" : "TRIAGE";
    printf("Top[%s]: [%s] (key=%.2f, name=%s, breed=%s, age=%d, friend=%d, health=%d)\n",
           mode, mode, c->key, c->name, c->breed, c->age, c->friendliness, c->health);
}

void cmd_serve(Shelter *s) {
    if (s->heap.size == 0) {
        printf("No cats available.\n");
        return;
    }

    if (s->mode == MODE_TRIAGE) {
        Cat top = s->heap.arr[0];
        printf("Serve now: %s (key=%.2f, name=%s, breed=%s, age=%d, friend=%d, health=%d)\n",
               top.name, top.key, top.name, top.breed, top.age, top.friendliness, top.health);
        cmd_remove(s, top.name);
    } else {
        /* Search for the best non-quarantined cat */
        Cat *temp_list = (Cat *)malloc(sizeof(Cat) * s->heap.size);
        int temp_count = 0;
        Cat *served_cat = NULL;

        while (s->heap.size > 0) {
            Cat top = s->heap.arr[0];
            /* Remove from heap temporarily */
            s->heap.arr[0] = s->heap.arr[s->heap.size - 1];
            s->heap.size--;
            heapify(&s->heap, 0);

            if (top.quarantine == 0) {
                served_cat = (Cat *)malloc(sizeof(Cat));
                *served_cat = top;
                break;
            } else {
                temp_list[temp_count++] = top;
            }
        }

        /* Put quarantined cats back */
        for (int i = 0; i < temp_count; i++) {
            s->heap.arr[s->heap.size] = temp_list[i];
            s->heap.size++;
            percolate_up(&s->heap, s->heap.size - 1);
        }
        free(temp_list);

        if (served_cat) {
            printf("Serve now: %s (key=%.2f, name=%s, breed=%s, age=%d, friend=%d, health=%d)\n",
                   served_cat->name, served_cat->key, served_cat->name, served_cat->breed, served_cat->age, served_cat->friendliness, served_cat->health);
            free_cat_contents(served_cat);
            free(served_cat);
        } else {
            printf("No adoptable cats available.\n");
        }
    }
}

void cmd_mode(Shelter *s, char *mode_str) {
    if (strcmp(mode_str, "ADOPTION") == 0) s->mode = MODE_ADOPTION;
    else if (strcmp(mode_str, "TRIAGE") == 0) s->mode = MODE_TRIAGE;
    
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
        s->featured_breed = (char *)malloc(strlen(breed) + 1);
        strcpy(s->featured_breed, breed);
        s->alpha = alpha;
        printf("Featured breed set to %s with alpha=%.2f. Rebuilding priorities...\n", breed, alpha);
    }
    recompute_all_keys_and_build(s);
}

void cmd_print(Shelter *s, int k) {
    if (s->heap.size == 0) {
        printf("No cats available.\n");
        return;
    }

    /* Temporary heap for printing in priority order */
    CatHeap temp;
    temp.capacity = s->heap.size;
    temp.size = s->heap.size;
    temp.mode = s->heap.mode;
    temp.arr = (Cat *)malloc(sizeof(Cat) * temp.size);
    memcpy(temp.arr, s->heap.arr, sizeof(Cat) * temp.size);

    int count = (k < temp.size) ? k : temp.size;
    const char *mode_str = (s->mode == MODE_ADOPTION) ? "ADOPTION" : "TRIAGE";

    for (int i = 0; i < count; i++) {
        Cat top = temp.arr[0];
        printf("[%d] %s (key=%.2f, %s)\n", i + 1, top.name, top.key, mode_str);
        
        temp.arr[0] = temp.arr[temp.size - 1];
        temp.size--;
        heapify(&temp, 0);
    }
    free(temp.arr);
}

/* --- Utilities --- */

int find_cat_index(CatHeap *heap, char *name) {
    for (int i = 0; i < heap->size; i++) {
        if (strcmp(heap->arr[i].name, name) == 0) return i;
    }
    return -1;
}

double compute_adoption_key(Cat *c, Shelter *s) {
    double base = 1.6 * c->friendliness + 1.1 * c->health - 0.7 * c->age;
    double factor = (s->featured_breed && strcmp(c->breed, s->featured_breed) == 0) ? s->alpha : 1.0;
    return (base * factor) - (0.000001 * c->arrival_id);
}

double compute_triage_key(Cat *c) {
    int penalty = (c->age > 12) ? (c->age - 12) : 0;
    return (100 - c->health) * 2.0 + penalty * 1.0 - 0.05 * c->friendliness;
}

void recompute_all_keys_and_build(Shelter *s) {
    for (int i = 0; i < s->heap.size; i++) {
        if (s->mode == MODE_ADOPTION)
            s->heap.arr[i].key = compute_adoption_key(&s->heap.arr[i], s);
        else
            s->heap.arr[i].key = compute_triage_key(&s->heap.arr[i]);
    }
    for (int i = s->heap.size / 2 - 1; i >= 0; i--) {
        heapify(&s->heap, i);
    }
}

void free_cat_contents(Cat *c) {
    if (c) {
        free(c->name);
        free(c->breed);
    }
}
