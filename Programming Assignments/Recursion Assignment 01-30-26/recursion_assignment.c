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

#include <stdio.h>

int main () {

}

int zeros(int n) {
    if (n == 0)
        return 0;
    else 
        return n/5 + zeros(n/5);
}

int isPalindrome(char *str, int left, int right) {
    
}