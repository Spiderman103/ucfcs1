/*
 * Author: Sahil Narsinghani
 * Course: COP3502H - Spring 2026
 * Assignment: PA0 - Cat Beauty Competition
 * Date: January 21, 2026
*/

#include <stdio.h>
#include "Cat.h"

/*
 * readData function reads cat data from user's input
 * Parameters are an array of cat structs and a pointer of type
 * int to store the number of cats. 
*/

void readData(struct cat *cats, int *c) {
    int numberOfCats;
    int scoreSum;

    scanf("%d", &numberOfCats);
    *c = numberOfCats;

    // Reading in all inputted data for each cat. 
    for (int i = 0; i < numberOfCats; ++i) {
        scanf("%d", &cats[i].cat_ID);
        scanf("%s", cats[i].name);
        scanf("%d", &cats[i].score1);
        scanf("%d", &cats[i].score2);
        scanf("%d", &cats[i].score3);

        // Calculating average score for each cat. 
        scoreSum = cats[i].score1 + cats[i].score2 + cats[i].score3;
        cats[i].average = scoreSum / 3.0;
    }
}

/*
 * getCutestCat function finds cutest cat based on highest average. 
 * Parameters are an array of cat structs and
 * an integer to store the number of cats. 
*/

int getCutestCat(struct cat *s, int n) {
    int cutestCatIndex = 0;
    float maxAverage = s[0].average;

    // Finding the cat with the highest average.
    for (int i = 1; i < n; ++i) {
        if (s[i].average > maxAverage) {
            maxAverage = s[i].average;
            cutestCatIndex = i;
        }
    }

    return cutestCatIndex;
}

int main() {
    struct cat cats[MAXCATS];
    int catCount = 0;
    int letters[26] = {};
    int cutestIndex;
    float percentage = 0.0;
    
    // Call readData function to read data of each cat. 
    readData(cats, &catCount);

    // Printing out data for each cat.
    for (int i = 0; i < catCount; ++i) {
        printf("%d", cats[i].cat_ID);
        printf(" %s", cats[i].name);
        printf(" %d", cats[i].score1);
        printf(" %d", cats[i].score2);
        printf(" %d", cats[i].score3);
        printf(" %.2lf\n", cats[i].average);
    }
    
    // Finding and printing out cutest cat.
    cutestIndex = getCutestCat(cats, catCount);
    printf("Cutest cat is %s\n", cats[cutestIndex].name);

    // Counting first letters of each cat's name.
    for (int i = 0; i < catCount; ++i) {
        char firstLetter = cats[i].name[0];
        int index = firstLetter - 'A';
        letters[index] = letters[index] + 1;
    }

    // Printing out the letter percentages.
    for (int i = 0; i < 26; ++i) {
        char letter = 'A' + i;

        if (catCount > 0)
            percentage = (letters[i] * 100.0) / catCount;
        else
            percentage = 0.0;

        printf("%c: %d %.2lf%%\n", letter, letters[i], percentage);
    }

    return 0;
}

