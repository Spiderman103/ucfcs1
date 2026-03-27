/* COP 3502C PA4
    This program is written by: Sahil Narsinghani
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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

int compareTo(Cat *catOne, Cat *catTwo, int traitIndex);
void insertionSort(Cat **list, int low, int high, int traitIndex);
void swapCats(Cat **firstCat, Cat **secondCat);
int partition(Cat **list, int low, int high, int traitIndex);
void quickSortRec(Cat **list, int low, int high, int traitIndex);
void quickSort(Cat **list, int numCats, int traitIndex);

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

int compareTo(Cat *catOne, Cat *catTwo, int traitIndex) {
    if (catOne->scores[traitIndex] != catTwo->scores[traitIndex]) {
        return catTwo->scores[traitIndex] - catOne->scores[traitIndex];
    } else {
        return strcmp(catOne->name, catTwo->name);
    }
}

void insertionSort(Cat **list, int low, int high, int traitIndex) {
  for (int i = low + 1; i <= high; ++i) {
    Cat *tempCat = list[i];
    int j = i - 1;

    while (j >= low && compareTo(list[j], tempCat, traitIndex) > 0) {
      list[j + 1] = list[j];
      j--;
    }
    list[j + 1] = tempCat;
  }
}

void swapCats(Cat **firstCat, Cat **secondCat) {
    Cat *temp = *firstCat;
    *firstCat = *secondCat;
    *secondCat = temp;
}

int partition(Cat **list, int low, int high, int traitIndex) {
    Cat *pivot = list[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (compareTo(list[j], pivot, traitIndex) <= 0) {
            i++;
            swapCats(&list[i], &list[j]);
        }
    }
    swapCats(&list[i + 1], &list[high]);
    return i + 1;
}

void quickSortRec(Cat **list, int low, int high, int traitIndex) {
    if (high - low + 1 <= BASECASESIZE) {
        insertionSort(list, low, high, traitIndex);
    } else {
        int pivotIndex = partition(list, low, high, traitIndex);
        quickSortRec(list, low, pivotIndex - 1, traitIndex);
        quickSortRec(list, pivotIndex + 1, high, traitIndex);
    }
}

void quickSort(Cat **list, int numCats, int traitIndex) {
    quickSortRec(list, 0, numCats - 1, traitIndex);
}
