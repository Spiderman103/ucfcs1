
/*
 * Author: Sahil Narsinghani
 * Course: COP 3502H - Spring 2026
 * Assignment: Dynamic Memory Allocation Assignment
 * Date: January 24, 2026
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *title; // dynamically allocated string (exact size)
    int pages; // number of pages
} Book;

int main() {
    int n;
    int pages;
    char bookTitle[100]; // Picked 100 characters to be limit of title string.
    Book **arr = NULL;

    scanf("%d", &n); 

    if (n > 0) {
        arr = malloc(n * sizeof(Book *)); 
        if (arr == NULL) {
            return -1;
        }
    }

    for (int i = 0; i < n; ++i) {
        scanf("%s %d", bookTitle, &pages); 
        
        arr[i] = malloc(sizeof(Book));
        if (arr[i] == NULL) { 
            for (int j = 0; j < i; ++j) {
                free(arr[j]->title);
                free(arr[j]);
            }
            free(arr);
            return -1;
        }

        arr[i]->title = malloc(strlen(bookTitle) + 1);
        if (arr[i]->title == NULL) {
            free(arr[i]);
            for (int j = 0; j < i; ++j) {
                free(arr[j]->title);
                free(arr[j]);
            }
            free(arr);
            return -1;
        }

        strcpy(arr[i]->title, bookTitle);
        arr[i]->pages = pages;
    }

    int queries;
    scanf("%d", &queries);

    for (int i = 0; i < queries; ++i) {
        int queryOption;
        scanf("%d", &queryOption);

        if (queryOption == 1) {
            for (int j = 0; j < n; ++j) {
                printf("%s %d\n", arr[j]->title, arr[j]->pages);
            }
        } else if (queryOption == 2) {
            scanf("%s", bookTitle);

            int foundIndex = -1;

            for (int j = 0; j < n; ++j) {
                if (strcmp(arr[j]->title, bookTitle) == 0) {
                    foundIndex = j;
                    break;
                }
            }

            if (foundIndex != -1) {
                free(arr[foundIndex]->title);
                free(arr[foundIndex]);

                for (int j = foundIndex; j < n - 1; ++j) {
                    arr[j] = arr[j + 1];
                }

                n--;

                if (n == 0) {
                    free(arr);
                    arr = NULL;
                } else {
                    Book **tempBook = realloc(arr, n * sizeof(Book *));
                    if (tempBook != NULL) {
                        arr = tempBook;
                    }
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        free(arr[i]->title);
        free(arr[i]);
    }
    free(arr);

    return 0;
}