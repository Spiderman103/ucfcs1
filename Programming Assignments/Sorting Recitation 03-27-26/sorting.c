/*
    COP3502H | Spring 2026 | Section 0206
    Name: Sahil Narsinghani
    UCF ID: 5753629
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int quickComparisons = 0;
int quickSwaps = 0;
int mergeComparisons = 0;
int mergeMoves = 0;


// Used functions from mergeSort.c and quickSort.c files
void printArray(int arr[], int n);
void swap(int *a, int *b);
int partition(int arr[], int low, int high);
void quickSort(int arr[], int low, int high);
void merge(int arr[], int left, int mid, int right);
void mergeSort(int arr[], int left, int right);

int main() {
    int n = 0;

    if (scanf("%d", &n) != 1) {
        return 1;
    }

    int *originalArr = (int *)malloc(n * sizeof(int));
    int *arr1 = (int *)malloc(n * sizeof(int));
    int *arr2 = (int *)malloc(n * sizeof(int));

    if (originalArr == NULL || arr1 == NULL || arr2 == NULL) {
        return 1;
    }

    // Getting the numbers from the user
    for (int i = 0; i < n; ++i) {
        scanf("%d", &originalArr[i]);
    }

    // Copying over those inputs to arr1 & arr2
    for (int i = 0; i < n; ++i) {
        arr1[i] = originalArr[i];
        arr2[i] = originalArr[i];
    }

    // Using clock() to measure sorting time in ms.
    clock_t qStart = clock();
    quickSort(arr1, 0, n - 1); // arr1 is used for quickSort
    clock_t qEnd = clock();

    // Using clock() to find quick Sort time in ms.
    clock_t qTicks = qEnd - qStart;
    double qSeconds = (double)qTicks / CLOCKS_PER_SEC;
    double qTime = qSeconds * 1000;


    // Using clock() to find merge Sort time in ms. 
    clock_t mStart = clock();
    mergeSort(arr2, 0, n - 1); // arr2 is used for mergeSort
    clock_t mEnd = clock();

    clock_t mTicks = mEnd - mStart;
    double mSeconds = (double)mTicks / CLOCKS_PER_SEC;
    double mTime = mSeconds * 1000;

    printf("Quick Sort: \n");
    printf("Sorted array: ");
    printArray(arr1, n);
    
    printf("Comparisons: %d\n", quickComparisons);
    printf("Swaps: %d\n", quickSwaps);
    printf("Runtime: %.2lf ms\n", qTime);
    printf("\n");

    printf("Merge Sort: \n");
    printf("Sorted array: ");
    printArray(arr2, n);

    printf("Comparisons: %d\n", mergeComparisons);
    printf("Moves: %d\n", mergeMoves);
    printf("Runtime: %.2lf ms\n", mTime);

    free(originalArr);
    free(arr1);
    free(arr2);

    return 0;
 }

// Took this from quickSort.c file
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Took this from quickSort.c file
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
    ++quickSwaps; // Same function from quickSort, but added swap counter.
}

// Took this partition function from quickSort.c file
int partition(int arr[], int low, int high) {
    int i = low + rand() % (high-low+1);
    swap(&arr[low], &arr[i]);

    int lowpos = low;
    low++;

    while (low <= high) {
        while (low <=high && arr[low] <= arr[lowpos]) {
            ++quickComparisons;
            low++;
        }
        while (low <= high && arr[high] > arr[lowpos]) {
            ++quickComparisons;
            high--;
        }

        if (low < high) {
            swap(&arr[low], &arr[high]);
        }
    }
    swap(&arr[lowpos], &arr[high]);
    
    return high;
}


// Took this from quickSort.c
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int split = partition(arr, low, high);
        quickSort(arr, low, split-1);
        quickSort(arr, split+1, high);
    }
}

// Took this from mergeSort.c
void merge(int arr[], int left, int mid, int right){
    int left_ar_sz = mid - left + 1;
    int right_ar_sz = right - mid;
    int *L = (int *)malloc(left_ar_sz * sizeof(int));
    int *R = (int *)malloc(right_ar_sz * sizeof(int));

    for (int i = 0;i < left_ar_sz; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0;j < right_ar_sz; j++) {
        R[j] = arr[mid + 1 + j];
    }
    int i = 0;  //index for left array
    int j = 0;  //index for right array
    int k = left;  //index for original array
    while (i < left_ar_sz && j < right_ar_sz) {
        ++mergeComparisons;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            k++;
            i++;
        } else {
            arr[k] = R[j];
            k++;
            j++;
        }
        ++mergeMoves;
    }
    while (i < left_ar_sz) {
        arr[k] = L[i];
        k++;
        i++;
        ++mergeMoves;
    }
    while (j < right_ar_sz) {
        arr[k] = R[j];
        k++;
        j++;
        ++mergeMoves;
    }
    free(L);
    free(R);
}


// Took this from mergeSort.c
void mergeSort(int arr[], int left, int right) {
    if (left == right) {
        return;  
    } 
    
    int mid = (left + right) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid+1, right);
    merge(arr, left, mid, right);
}

