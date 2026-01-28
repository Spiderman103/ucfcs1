/*
 * COP 3502H | Spring 2026 | Section 0206
 * Name: Sahil Narsinghani
 * UCF ID: 5753629
*/

int zeros(int n);
int zeros1(int n);
int isPalindrome(char *str, int left, int right);
int isPalindrome1(char *str, int left, int right);
int findMax(int arr[], int start, int end);
int findMin(int arr[], int start, int end);
int searchRecord(int numbers[], int low, int high, int value);

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main () {

    // Problem 1: Testing Given Code and My Recursion
    int stopNumber = 1;
    while(stopNumber != 0)
    {
        int res;

        printf("Type in a number: ");
        scanf("%d", &res);

        stopNumber = res;

        int out = zeros(res);
        int out1 = zeros1(res);

        printf("%d %d\n", out, out1);
    }

    char *palindrome = malloc(100 * sizeof(char));

    int stop = 1;
    while(stop != 0) {
        int res;
        int res1;

        printf("Type in a palindrome: ");
        scanf("%s", palindrome);

        res = isPalindrome(palindrome, 0, strlen(palindrome)-1);
        res1 = isPalindrome1(palindrome, 0, strlen(palindrome)-1);

        printf("%d %d\n", res, res1);
    }


}

// My Recursion Code
int zeros(int n) {
    if (n == 0)
        return 0;
    else 
        return n/5 + zeros(n/5);
}

// Given Test Code
int zeros1(int n) {
    int res = 0;
    while (n != 0) {
        res += n / 5;
        n /= 5;
    }   
    return res;
}

//My Recursion Code
int isPalindrome(char *str, int left, int right) {
    if(left >= right)
        return 1;
    
    if(str[left] != right)
        return 0;

    return isPalindrome(str, left + 1, right - 1);
}

//Test Code I Made
int isPalindrome1(char *str, int left, int right) {
    left = 0;
    right = strlen(str) - 1;

    while (left < right) {
        if(str[left] != str[right])
            return 0;
        left++;
        right--;
    }
    return 1;
}
