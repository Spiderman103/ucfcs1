/*
 * COP 3502H | Spring 2026 | Section 0206
 * Name: Sahil Narsinghani
 * UCF ID: 5753629
*/

#include <stdio.h>

int searchRecord(int numbers[], int low, int high, int value);

int main() {

    //Test Cases
    int arr[] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19};
    int size = sizeof(arr) / sizeof(arr[0]);
    int value = searchRecord(arr, 0, size - 1, 9);

    printf("Index of 9: %d\n", value);
    
    return 0;
}

int searchRecord(int numbers[], int low, int high, int value) {
    if (low > high) {
        return -1;
    }

    int middleIndex = low + (high - low) / 2;
    if (numbers[middleIndex] == value) {
        return middleIndex;
    }

    if (numbers[middleIndex] > value) {
        return searchRecord(numbers, low, middleIndex - 1, value);
    } else {
        return searchRecord(numbers, middleIndex + 1, high, value);
    }
}
