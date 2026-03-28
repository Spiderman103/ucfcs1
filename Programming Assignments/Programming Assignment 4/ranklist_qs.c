/* COP 3502C PA4
    This program is written by: Sahil Narsinghani
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 12
#define NUMTRAITS 7
#define TOTAL 6
#define BASECASESIZE 30

const char TRAITS[NUMTRAITS][MAXSIZE+1] = {
    "Cuteness", "Fluffiness", "Agility",
    "Friendliness", "Intelligence", "Laziness", "Total"
};

typedef struct Cat {
    char *name;
    int scores[NUMTRAITS];
} Cat;

int compareTo(Cat *ptrC1, Cat *ptrC2, int key);
void insertionSort(Cat **list, int low, int high, int key);
void swapCats(Cat **a, Cat **b);
int partition(Cat **list, int low, int high, int key);
void quickSortRec(Cat **list, int low, int high, int key);
void quickSort(Cat **list, int n, int key);

int main() {
    int numCats = 0;
    if (scanf("%d", &numCats) != 1) {
        return 1;
    }

    Cat **catList = (Cat **)malloc(numCats * sizeof(Cat *));
    if (catList == NULL) {
        return 1;
    }

    for (int i = 0; i < numCats; ++i) {
        catList[i] = (Cat *)malloc(sizeof(Cat));
        if (catList[i] == NULL) {
            return 1;
        }

        char tempNameBuffer[MAXSIZE + 1];
        scanf("%s", tempNameBuffer);

        catList[i]->name = (char *)malloc((strlen(tempNameBuffer) + 1) * sizeof(char));
        if (catList[i]->name != NULL) {
            strcpy(catList[i]->name, tempNameBuffer);
        } 

        int runningTotal = 0;
        for (int j = 0; j < TOTAL; ++j) {
            scanf("%d", &catList[i]->scores[j]);
            runningTotal += catList[i]->scores[j];
        }
        catList[i]->scores[TOTAL] = runningTotal;
    }

    int targetTrait = 0;
    if (scanf("%d", &targetTrait) != 1) {
        return 1;
    }

    quickSort(catList, numCats, targetTrait);

    printf("%s Ranklist\n", TRAITS[targetTrait]);
    for (int i = 0; i < numCats; ++i) {
        printf("%d. %-15s %d\n", i + 1, catList[i]->name, catList[i]->scores[targetTrait]);    
    }

    for (int i = 0; i < numCats; ++i) {
        free(catList[i]->name);
        free(catList[i]);
    }
    free(catList);

    return 0;
}

int compareTo(Cat *ptrC1, Cat *ptrC2, int key) {
    if (ptrC1->scores[key] != ptrC2->scores[key]) {
        return ptrC2->scores[key] - ptrC1->scores[key];
    } else {
        return strcmp(ptrC1->name, ptrC2->name);
    }
}

void insertionSort(Cat **list, int low, int high, int key) {
  for (int i = low + 1; i <= high; ++i) {
    Cat *tempCat = list[i];
    int j = i - 1;

    while (j >= low && compareTo(list[j], tempCat, key) > 0) {
      list[j + 1] = list[j];
      --j;
    }
    list[j + 1] = tempCat;
  }
}

void swapCats(Cat **a, Cat **b) {
    Cat *temp = *a;
    *a = *b;
    *b = temp;
}

int partition(Cat **list, int low, int high, int key) {
    int i = low + rand() % (high - low + 1);
    swapCats(&list[low], &list[i]);

    int lowpos = low;
    ++low;

    while (low <= high) {
        while (low <= high && compareTo(list[low], list[lowpos], key) <= 0) {
            ++low;
        }
        while (low <= high && compareTo(list[high], list[lowpos], key) > 0) {
            --high;
        }
        if (low < high) {
            swapCats(&list[low], &list[high]);
        }
    }
    swapCats(&list[lowpos], &list[high]);
    return high;
}

void quickSortRec(Cat **list, int low, int high, int key) {
    if (high - low + 1 <= BASECASESIZE) {
        insertionSort(list, low, high, key);
    } else {
        int split = partition(list, low, high, key);
        quickSortRec(list, low, split - 1, key);
        quickSortRec(list, split + 1, high, key);
    }
}

void quickSort(Cat **list, int numCats, int key) {
    quickSortRec(list, 0, numCats - 1, key);
}
