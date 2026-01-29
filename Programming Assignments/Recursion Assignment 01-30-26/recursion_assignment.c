/*
 * COP 3502H | Spring 2026 | Section 0206
 * Name: Sahil Narsinghani
 * UCF ID: 5753629
*/

int zeros(int n);
int isPalindrome(char *str, int left, int right);
int findMax(int arr[], int start, int end);
int findMin(int arr[], int start, int end);
int searchRecord(int numbers[], int low, int high, int value);

int main () {
    return 0;
}

// My Recursion Code
int zeros(int n) {
    if (n == 0)
        return 0;
    else 
        return n/5 + zeros(n/5);
}

//My Recursion Code
int isPalindrome(char *str, int left, int right) {
    if(left >= right)
        return 1;
    
    if(str[left] != str[right])
        return 0;

    return isPalindrome(str, left + 1, right - 1);
}

//The recursive function for findMax
int findMax(int arr[], int start, int end) {
    int max = 0;
    if(start == end) {
        return arr[start];
    } 

    int maxOfRest = findMax(arr, start + 1, end);

    if(arr[start] > maxOfRest) {
        return arr[start];
    } else {
        return maxOfRest;
    }
}

int findMin(int arr[], int start, int end) {
    if (start == end) {
        return arr[start];
    }

    int minOfRest = findMin(arr, start + 1, end);

    if(arr[start] < minOfRest) {
        return arr[start];
    } else {
        return minOfRest;
    }
}

int searchRecord(int numbers[], int low, int high, int value) {
    if (low > high) {
        return -1;
    }

    int middleIndex = low + (high - low) / 2;
    if(numbers[middleIndex] == value) {
        return middleIndex;
    }

    if (numbers[middleIndex] > value) {
        return searchRecord(numbers, low, middleIndex - 1, value);
    } else {
        return searchRecord(numbers, middleIndex + 1, high, value);
    }
}
