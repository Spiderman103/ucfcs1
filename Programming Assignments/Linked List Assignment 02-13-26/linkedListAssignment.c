/*
 * COP 3502H | Spring 2026 | Section 0206
 * Name: Sahil Narsinghani
 * UCF ID: 5753629
*/

#include <stdio.h>
#include <stdlib.h>

// Node struct for problem 1
typedef struct node1 {
    char letter;
    struct node1 *next;
} node1;

int length(node1 *head);

char *toCString(node1 *head) {
    int len = length(head);
    int index = 0;
    
    char *str = (char *)malloc((len + 1) * sizeof(char));
    node1 *curr = head;

    while (curr != NULL) {
        str[index] = curr->letter;
        ++index;
        curr = curr->next;
    }

    str[len] = '\0';
    return str;
}

// Node struct for problems 2-5
typedef struct node2 {
    int data;
    struct node2 *next;
} node2;


void markEven(node2 *head) {
    node2 *curr = head;

    while (curr != NULL) {
        if (curr->data % 2 == 0) {
            node2 *newNode = (node2 *)malloc(sizeof(node2));
            newNode->data = -1;

            newNode->next = curr->next;
            curr->next = newNode;

            curr = newNode->next;
        } else {
            curr = curr->next;
        }
    }
}


node2* addValue3(node2 *head) {
    if (head == NULL || head->data > 3) {
        node2 *tmp = (node2 *)malloc(sizeof(node2));
        tmp->data = 3;
        tmp->next = head;
        return tmp;
    }

    if (head->data == 3) {
        return head;
    }

    node2 *iter = head;

    while (iter->next != NULL && iter->next->data < 3) {
        iter = iter->next;
    }

    if (iter->next != NULL && iter->next->data == 3) {
        return head;
    }

    node2 *tmp = malloc(sizeof(node2));
    tmp->data = 3;

    tmp->next = iter->next;
    iter->next = tmp;
    return head;
}


/*
    Problem 4 Responses
    a). Is the function recursive?
        - Yes the function is recursive because eFunction calls itself 
        on line 10 with eFunction(aNode->next);

    b). The function eFunction reverses the linked list and the 
        recursive call goes through the rest of the linked list. 

    c). Line 12 sets the original first node;s next pointer to NULL, making
        it the last node now. 
*/


void insertAfterN(node2 *head, int M, int N) {
    node2 *curr = head;

    while (curr != NULL) {
        if (curr->data == N) {
            node2 *newNode = (node2 *)malloc(sizeof(node2));
            newNode->data = M;

            newNode->next = curr->next;
            curr->next = newNode;

            curr = newNode->next;
        } else {
            curr = curr->next;
        }
    }
}