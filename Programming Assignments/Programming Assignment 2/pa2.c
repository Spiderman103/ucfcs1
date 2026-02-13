
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int countHighPerformersTraits(int *permArr, int teamStart, int teamSize, int catPosition, int traitPosition);
int synergyBonusApplies(int *permArr, int teamStart, int teamSize, int catPosition);
int rivalPenaltyApplies(int *permArr, int teamStart, int teamSize, int numRivals, int rivalPosition);

int findCatIndex(char *name, int numCats);
void permutation(int *permArr, int start, int end, int numCats, int teamSize, int numRivals);

int main() {

}

int countHighPerformersTraits(int *permArr, int teamStart, int teamSize, int catPosition, int traitPosition) {
    if (catPosition == teamSize) {
        return 0;
    }

    int count = 0;
    int catIndex = permArr[teamStart + catPosition];

    if (cats[catIndex].scores[traitPosition] >= 90) {
        count = 1;
    }

    if (traitPosition < MAX_SCORES - 1) {
        return count + countHighPerformersTraits(permArr, teamStart, teamSize, catPosition, traitPosition + 1);
    } else {
        return count + countHighPerformersTraits(permArr, teamStart, teamSize, catPosition + 1, 0);
    }
}

int synergyBonusApplies(int *permArr, int teamStart, int teamSize, int catPosition) {
    if (catPosition == teamSize) {
        return 1;
    }

    int catIndex = permArr[teamStart + catPosition];
    int hasHighTrait = 0;

    for (int i = 0; i < MAX_SCORES; ++i) {
        if (cats[catIndex].scores[i] >= 85) {
            hasHighTrait = 1;
            break;
        }
    }

    if (hasHighTrait == 0) {
        return 0;
    }

    return synergyBonusApplies(permArr, teamStart, teamSize, catPosition + 1);
}

int rivalPenaltyApplies(int *permArr, int teamStart, int teamSize, int numRivals, int rivalPosition) {
    if (rivalPosition == numRivals) {
        return 0;
    }

    int cat1Index = findCatIndex(rivals[rivalPosition].cat1->name, teamSize);
    int cat2Index = findCatIndex(rivals[rivalPosition].cat2->name, teamSize);

    if (cat1Index != -1 && cat2Index != -1) {
        return 1;
    }

    return rivalPenaltyApplies(permArr, teamStart, teamSize, numRivals, rivalPosition + 1);
}

int findCatIndex(char *name, int numCats) {
    for (int i = 0; i < numCats; ++i) {
        if (strcmp(cats[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

void permutation(int *permArr, int start, int end, int numCats, int teamSize, int numRivals) {
    if (start == end) {
        return;
    }

    for (int i = start; i <= end; ++i) {
        permutation(permArr, start + 1, end, numCats, teamSize, numRivals);
    }
}