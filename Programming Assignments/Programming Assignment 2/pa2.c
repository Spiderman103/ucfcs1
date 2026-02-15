
/*
 * COP 3502H PA2
 * This program is written by: Sahil Narsinghani
*/

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
void permutation(int *permArr, int *usedArr, int start, int end, int numCats, int teamSize, int numRivals);
float teamScore(int *permArr, int teamStart, int teamSize, int numRivals);

int main() {
    int n;
    int c;
    bestPermScore = 0;

    scanf("%d %d", &n, &c);

    int totalCats = n * c;

    cats = (Cat *)malloc(totalCats * sizeof(Cat));

    char tempName[MAX_STR];
    char tempBreed[MAX_STR];

    for (int i = 0; i < totalCats; ++i) {

        scanf("%s %s", tempName, tempBreed);

        cats[i].name = (char *)malloc((strlen(tempName) + 1) * sizeof(char));
        cats[i].breed = (char *)malloc((strlen(tempBreed) + 1) * sizeof(char));
        strcpy(cats[i].name, tempName);
        strcpy(cats[i].breed, tempBreed);

        int scoreSum = 0;
        for (int j = 0; j < MAX_SCORES; ++j) {
            scanf("%d", &cats[i].scores[j]);
            scoreSum += cats[i].scores[j];
        }
        cats[i].baseScore = scoreSum;
    }

    int numRivals;
    scanf("%d", &numRivals);

    if (numRivals > 0) {
        rivals = (Rivals *)malloc(numRivals * sizeof(Rivals));

        char cat1Name[MAX_STR];
        char cat2Name[MAX_STR];

        for (int i = 0; i < numRivals; ++i) {
            scanf("%s %s", cat1Name, cat2Name);

            int cat1Index = findCatIndex(cat1Name, totalCats);
            int cat2Index = findCatIndex(cat2Name, totalCats);

            if (cat1Index != -1 && cat2Index != -1) {
                rivals[i].cat1 = &cats[cat1Index];
                rivals[i].cat2 = &cats[cat2Index];
            }
        }
    } else {
        rivals = NULL;
    }

    tracker = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        tracker[i] = (int *)malloc(c * sizeof(int));
    }

    int *permArr = (int *)malloc(totalCats * sizeof(int));
    int *usedArr = (int *)malloc(totalCats * sizeof(int));

    for (int i = 0; i < totalCats; ++i) {
        permArr[i] = i;
        usedArr[i] = 0;
    }

    permutation(permArr, usedArr, 0, totalCats - 1, totalCats, c, numRivals);
    printf("Best Teams Grouping score: %.2f\n", bestPermScore);

    float bestScore = -1;
    int bestTeamIndexPos = 0;

    for (int i = 0; i < n; ++i) {
        float score = teamScore(tracker[i], 0, c, numRivals);
        printf("Team %d:", i + 1);

        for (int j = 0; j < c; ++j) {
            printf(" %s", cats[tracker[i][j]].name);
        }
        printf(" %.2f\n", score);

        if (score > bestScore) {
            bestScore = score;
            bestTeamIndexPos = i;
        }
    }

    printf("Best Candidate:");
    for (int j = 0; j < c; ++j) {
        printf(" %s", cats[tracker[bestTeamIndexPos][j]].name);
    }
    printf("\n");

    for (int i = 0; i < totalCats; ++i) {
        free(cats[i].name);
        free(cats[i].breed);
    }
    free(cats);

    if (rivals != NULL) {
        free(rivals);
    } 

    for (int i = 0; i < n; ++i) {
        free(tracker[i]);
    }
    free(tracker);
    free(permArr);
    free(usedArr);

    return 0;
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

    for (int i = 0; i < teamSize - 1; ++i) {
        int leftCatIndex = permArr[teamStart + i];
        int rightCatIndex = permArr[teamStart + i + 1];

        Cat *currentLeft = &cats[leftCatIndex];
        Cat *currentRight = &cats[rightCatIndex];

        Cat *rival1 = rivals[rivalPosition].cat1;
        Cat *rival2 = rivals[rivalPosition].cat2;

        if ((currentLeft == rival1 && currentRight == rival2) || (currentLeft == rival2 && currentRight == rival1)) {
            return rivalPenaltyApplies(permArr, teamStart, teamSize, numRivals, rivalPosition + 1) + 1;
        }
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

void permutation(int *permArr, int *usedArr, int start, int end, int numCats, int teamSize, int numRivals) {
    if (start > end) {
        float total = 0;
        int numTeams = numCats / teamSize;

        for (int i = 0; i < numTeams; ++i) {
            total += teamScore(permArr, i * teamSize, teamSize, numRivals);
        }

        if (total > bestPermScore) {
            bestPermScore = total;

            for (int i = 0; i < numTeams; ++i) {
                for (int j = 0; j < teamSize; ++j) {
                    tracker[i][j] = permArr[i * teamSize + j];
                }
            }
        }
        return;
    }

    for (int i = 0; i < numCats; ++i) {
        if (!usedArr[i]) {
            usedArr[i] = 1;
            permArr[start] = i;

            permutation(permArr, usedArr, start + 1, end, numCats, teamSize, numRivals);
            usedArr[i] = 0;
        }
    }
}

float teamScore(int *permArr, int teamStart, int teamSize, int numRivals) {
    float baseScore = 0;
    
    for (int i = 0; i < teamSize; ++i) {
        int catIndex = permArr[teamStart + i];
        baseScore += cats[catIndex].baseScore;
    }

    baseScore = (float) baseScore / teamSize;
    float score = baseScore;

    int highPerformersCount = countHighPerformersTraits(permArr, teamStart, teamSize, 0, 0);
    score += highPerformersCount * 5;

    if (synergyBonusApplies(permArr, teamStart, teamSize, 0)) {
        score += 30;
    }

    int allDifferentBreeds = 1;
    for (int i = 0; i < teamSize; ++i) {
      for (int j = i + 1; j < teamSize; ++j) {
        if (strcmp(cats[permArr[teamStart + i]].breed, cats[permArr[teamStart + j]].breed) == 0) {
          allDifferentBreeds = 0;
        }
      }
    }

    if (allDifferentBreeds) {
        score += 10;
    } else {
        score -= 15;
    }

    for (int i = 0; i < teamSize; ++i) {
      int catIndex = permArr[teamStart + i];
      score += cats[catIndex].baseScore * (POSITION_BONUS[i] / 100.0);
    }

    if (numRivals > 0) {
        score -= rivalPenaltyApplies(permArr, teamStart, teamSize, numRivals, 0) * 25;
    }

    return score;
}