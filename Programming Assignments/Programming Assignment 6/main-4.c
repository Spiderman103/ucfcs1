/* COP 3502C PA5
  This program is written by: Sahil Narsinghani
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 25
#define NUM_TRAITS 5

const char *TRAIT_NAMES[NUM_TRAITS] = {"friendly", "grumpy", "playful", "lazy", "curious"};

typedef struct Cat {
    char *name;
    char *breed;
    int charm;
    int traits[NUM_TRAITS];
} Cat;

typedef struct BST_Node {
    Cat *cat;
    struct BST_Node *left;
    struct BST_Node *right;
    int subtree_size;
} BST_Node;

// Helper functions made with the BST Code class files. 
BST_Node *createNode(Cat *cat);
void freeCat(Cat *cat);
void freeTree(BST_Node *root);
int getSubtreeSize(BST_Node *node);
void updateSize(BST_Node *node); // Updates current node subtree size based on left and right children.
int countActiveTraits(Cat *cat);
Cat *copyCat(Cat *s); // Creates copy of Cat struct to swap out data if query equals 3.
BST_Node *insert(BST_Node *root, Cat *cat, int depth, int *resDepth, int *replaced, int *ignored);
BST_Node *minValNode(BST_Node *node);
BST_Node *deleteNode(BST_Node *root, char *name);
Cat *findKth(BST_Node *root, int k);
void inorder(BST_Node *root); 
char **filterByTrait(BST_Node *root, int traitIndex, int traitValue, int *resultSize);
void collectNames(BST_Node *root, int traitIndex, int traitValue, char **names, int *count);

int main() {
    int n;
    int query;

    BST_Node *root = NULL;

    if (scanf("%d", &n) != 1) {
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        if (scanf("%d", &query) != 1) {
            break;
        }

        if (query == 1) {
            char tempName[MAX_NAME + 1];
            char tempBreed[MAX_NAME + 1];

            int charm;
            int traits[NUM_TRAITS];

            scanf("%s %s %d", tempName, tempBreed, &charm);
            for (int j = 0; j < NUM_TRAITS; ++j) {
                scanf("%d", &traits[j]);
            }

            Cat *newCat = (Cat *)malloc(sizeof(Cat));
            newCat->name = (char *)malloc((strlen(tempName) + 1) * sizeof(char));
            strcpy(newCat->name, tempName);

            newCat->breed = (char *)malloc((strlen(tempBreed) + 1) * sizeof(char));
            strcpy(newCat->breed, tempBreed);

            newCat->charm = charm;
            for (int j = 0; j < NUM_TRAITS; ++j) {
                newCat->traits[j] = traits[j];
            }

            int depth = 0;
            int replaced = 0;
            int ignored = 0;

            root = insert(root, newCat, depth, &depth, &replaced, &ignored);

            if (replaced) {
                printf("Replaced\n");
            } else if (!ignored) {
                printf("Insert: %d\n", depth);
            }
        } else if (query == 2) {
            char name[MAX_NAME + 1];
            scanf("%s", name);
            root = deleteNode(root, name);
            printf("Deletion Complete\n");
        } else if (query == 3) {
            int k = 0;
            scanf("%d", &k);

            Cat *kthCat = findKth(root, k);
            if (kthCat == NULL) {
                printf("NO SMALLEST ELEMENT FOUND\n");
            } else {
                printf("%s %s %d\n", kthCat->name, kthCat->breed, kthCat->charm);
            }
        } else if (query == 4) {
            int traitIndex = 0;
            int traitValue = 0;
            int size = 0;

            scanf("%d %d", &traitIndex, &traitValue);
            char **results = filterByTrait(root, traitIndex, traitValue, &size);

            if (size == 0) {
                printf("NONE FOUND\n");
            } else {
                printf("%s:", TRAIT_NAMES[traitIndex]);
                for (int j = 0; j < size; ++j) {
                    printf(" %s", results[j]);
                    free(results[j]);
                }
                printf("\n");
                free(results);
            }
        } else if (query == 5) {
            int traitIndex = 0;
            int traitValue = 0;
            int size = 0;

            scanf("%d %d", &traitIndex, &traitValue);
            char **remove = filterByTrait(root, traitIndex, traitValue, &size);

            if (size == 0) {
                printf("NONE REMOVED\n");
            } else {
                for (int j = 0; j < size; ++j) {
                    root = deleteNode(root, remove[j]);
                    free(remove[j]);
                }
                printf("%d\n", size);
                free(remove);
            }
        } else if (query == 6) {
            if (root == NULL) {
                printf("EMPTY\n");
            } else {
                inorder(root);
            }
        }
    }
    freeTree(root);
    return 0;
}

BST_Node *createNode(Cat *cat) {
    BST_Node *newNode = (BST_Node *)malloc(sizeof(BST_Node));
    newNode->cat = cat;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->subtree_size = 1;
    return newNode;
}

void freeCat(Cat *cat) {
    if (cat) {
        free(cat->name);
        free(cat->breed);
        free(cat);
    }
}

void freeTree(BST_Node *root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        freeCat(root->cat);
        free(root);
    }
}

int getSubtreeSize(BST_Node *node) {
    if (node == NULL) {
        return 0;
    }
    return node->subtree_size;
}

void updateSize(BST_Node *node)  {
    if (node != NULL) {
        node->subtree_size = 1 + getSubtreeSize(node->left) + getSubtreeSize(node->right);
    }
}

int countActiveTraits(Cat *cat) {
    int count = 0;
    for (int i = 0; i < NUM_TRAITS; ++i) {
        if (cat->traits[i] == 1) {
            ++count;
        }
    }
    return count;
}

Cat *copyCat(Cat *c) {
    Cat *newCat = (Cat *)malloc(sizeof(Cat));
    newCat->name = (char *)malloc((strlen(c->name) + 1) * sizeof(char));
    strcpy(newCat->name, c->name);

    newCat->breed = (char *)malloc((strlen(c->breed) + 1) * sizeof(char));
    strcpy(newCat->breed, c->breed);
    
    newCat->charm = c->charm;
    for (int i = 0; i < NUM_TRAITS; ++i) {
        newCat->traits[i] = c->traits[i];
    }
    return newCat;
}

BST_Node *insert(BST_Node *root, Cat *cat, int depth, int *resultDepth, int *replaced, int *ignored) {
    if (root == NULL) {
        *resultDepth = depth;
        return createNode(cat);
    }

    int compareValue = strcmp(cat->name, root->cat->name);
    if (compareValue < 0) {
        root->left = insert(root->left, cat, depth + 1, resultDepth, replaced, ignored);
    } else if (compareValue > 0) {
        root->right = insert(root->right, cat, depth + 1, resultDepth, replaced, ignored);
    } else {
        if (countActiveTraits(cat) > countActiveTraits(root->cat)) {
            freeCat(root->cat);
            root->cat = cat;
            *replaced = 1;
        } else {
            freeCat(cat);
            *ignored = 1;
        }
    }
    updateSize(root);
    return root;
}

BST_Node *minValNode(BST_Node *node) {
    BST_Node *curr = node;
    while (curr && curr->left != NULL) {
        curr = curr->left;
    }
    return curr;
}

BST_Node *deleteNode(BST_Node *root, char *name) {
    if (root == NULL) {
        return NULL;
    }

    int compareValue = strcmp(name, root->cat->name);
    if (compareValue < 0) {
        root->left = deleteNode(root->left, name);
    } else if (compareValue > 0) {
        root->right = deleteNode(root->right, name);
    } else {
        if (root->left == NULL) {
            BST_Node *temp = root->right;
            freeCat(root->cat);
            free(root);
            return temp;
        } else if (root->right == NULL) {
            BST_Node *temp = root->left;
            freeCat(root->cat);
            free(root);
            return temp;
        }
        BST_Node *temp = minValNode(root->right);
        Cat *tempCat = copyCat(temp->cat);
        root->right = deleteNode(root->right, temp->cat->name);
        freeCat(root->cat);
        root->cat = tempCat;
    }
    updateSize(root);
    return root;
}

Cat *findKth(BST_Node *root, int k) {
    if (root == NULL) {
        if (k <= 0 || k > getSubtreeSize(root)) {
            return NULL;
        }
    }

    int leftSize = getSubtreeSize(root->left);
    if (k <= leftSize) {
        return findKth(root->left, k);
    } else if (k == leftSize + 1) {
        return root->cat;
    } else {
        return findKth(root->right, k - leftSize - 1);
    }
}

void inorder(BST_Node *root) {
    if (root) {
        inorder(root->left);
        printf("%s %d %d\n", root->cat->name, root->cat->charm, root->subtree_size);
        inorder(root->right);
    }
}

char **filterByTrait(BST_Node *root, int traitIndex, int traitValue, int *resultSize) {
    int totalNodes = getSubtreeSize(root);
    if (totalNodes == 0) {
        *resultSize = 0;
        return NULL;
    } 
    
    char **names = (char **)malloc(totalNodes * sizeof(char *));
    int count = 0;

    collectNames(root, traitIndex, traitValue, names, &count);
    *resultSize = count;

    if (count == 0) {
        free(names);
        return NULL;
    }

    char **finalNames = (char **)realloc(names, count * sizeof(char *));
    return finalNames;
}

void collectNames(BST_Node *root, int traitIndex, int traitValue, char **names, int *count) {
    if (root == NULL) {
        return;
    }

    collectNames(root->left, traitIndex, traitValue, names, count);
    if (root->cat->traits[traitIndex] == traitValue) {
        names[*count] = (char *)malloc((strlen(root->cat->name) + 1) * sizeof(char));
        strcpy(names[*count], root->cat->name);
        ++(*count);
    }
    collectNames(root->right, traitIndex, traitValue, names, count);
}
