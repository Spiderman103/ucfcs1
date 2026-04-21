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
void insertionSort(Cat **list, int low, int high, int key); // Helper function
void merge(Cat **list, int low, int mid, int high, int key); // Helper function
void mergeSortRec(Cat **list, int low, int high, int key);
void mergeSort(Cat **list, int n, int key);

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

    mergeSort(catList, numCats, targetTrait);

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

void merge(Cat **list, int low, int mid, int high, int key) {
    int leftSize = mid - low + 1;
    int rightSize = high - mid;

    Cat **leftArray = (Cat **)malloc(leftSize * sizeof(Cat *));
    Cat **rightArray = (Cat **)malloc(rightSize * sizeof(Cat *));
    if (leftArray == NULL || rightArray == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    for (int i = 0; i < leftSize; ++i) {
        leftArray[i] = list[low + i];
    }
    for (int i = 0; i < rightSize; ++i) {
        rightArray[i] = list[mid + 1 + i];
    }

    int i = 0;
    int j = 0;
    int k = low;

    while (i < leftSize && j < rightSize) {
      if (compareTo(leftArray[i], rightArray[j], key) <= 0) {
        list[k] = leftArray[i];
        ++i;
      } else {
        list[k] = rightArray[j];
        ++j;
      }
        ++k;
    }

    while (i < leftSize) {
        list[k] = leftArray[i];
        ++i;
        ++k;
    }
    while (j < rightSize) {
        list[k] = rightArray[j];
        ++j;
        ++k;
    }

    free(leftArray);
    free(rightArray);
}

void mergeSortRec(Cat **list, int low, int high, int key) {
    if (high - low + 1 <= BASECASESIZE) {
      insertionSort(list, low, high, key);
      return;
    }
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSortRec(list, low, mid, key);
        mergeSortRec(list, mid + 1, high, key);
        merge(list, low, mid, high, key);
    }
}

void mergeSort(Cat **list, int n, int key) {
  mergeSortRec(list, 0, n - 1, key);
}
