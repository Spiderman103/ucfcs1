// Part of this code credits to Mr. Guha and Dr. Ahmed
#include <stdio.h>
#include <stdlib.h>

// Define the structure of a tree node
typedef struct node_s {
    int data;
    struct node_s *leftchild;
    struct node_s *rightchild;
} treenode_t;

// Function prototypes
treenode_t *createNode(int val);
treenode_t *insertR(treenode_t *root, int val);
treenode_t *removeNode(treenode_t *root, int val);
treenode_t *search(treenode_t *root, int val);
void inorder(treenode_t *root);
void postorder(treenode_t *root);
void preorder(treenode_t *root);
void destroyTree(treenode_t *root);
treenode_t *findNode(treenode_t *current_ptr, int value);
treenode_t *parent(treenode_t *root, treenode_t *node);
treenode_t *minVal(treenode_t *root);
treenode_t *maxVal(treenode_t *root);
int isLeaf(treenode_t *node);
int hasOnlyLeftChild(treenode_t *node);
int hasOnlyRightChild(treenode_t *node);
int main() {
    // Initialize an empty BST
    treenode_t *root = NULL;

    // Insert hardcoded values into the BST
    root = insertR(root, 50);
    root = insertR(root, 30);
    root = insertR(root, 70);
    root = insertR(root, 20);
    root = insertR(root, 40);
    root = insertR(root, 60);
    root = insertR(root, 80);

    printf("Binary Search Tree created with hardcoded values.\n");

    // Perform different tree traversals
    printf("Inorder Traversal: ");
    inorder(root);
    printf("\n");

    printf("Preorder Traversal: ");
    preorder(root);
    printf("\n");

    printf("Postorder Traversal: ");
    postorder(root);
    printf("\n");

    // Search for a value in the BST
    int searchVal = 40;
    if (search(root, searchVal))
        printf("Value %d found in the BST.\n", searchVal);
    else
        printf("Value %d not found in the BST.\n", searchVal);

    // Remove a node from the BST
    printf("Removing value 30 from the BST.\n");
    root = removeNode(root, 30);

    // Inorder traversal after deletion
    printf("Inorder Traversal after deletion: ");
    inorder(root);
    printf("\n");

    // Free the allocated memory for BST
    destroyTree(root);
    printf("Binary Search Tree destroyed.\n");

    return 0;
}

// Function to create a new tree node
treenode_t *createNode(int val) {
    treenode_t *node = (treenode_t *)malloc(sizeof(treenode_t));

    if (node == NULL) {
        printf("Memory allocation failed!\n");
        return NULL;
    }

    node->data = val;
    node->leftchild = NULL;
    node->rightchild = NULL;

    return node;
}

// // Recursive function to insert a node into the BST - more clean code (compact)
// treenode_t *insertR(treenode_t *root, int val) {
//     if (root == NULL) {
//         return createNode(val);
//     }

//     if (val < root->data) {
//         root->leftchild = insertR(root->leftchild, val);
//     } else {
//         root->rightchild = insertR(root->rightchild, val);
//     }

//     return root;
// }

// longer version to insert a node
treenode_t *insertR(treenode_t *root, int val) {

    // Case 1: The tree is empty — create and return a new node.
    if (root == NULL) {
        printf("Element data %d -> address: %p\n", val, &val);
        return createNode(val);
    }
    else {
        // Case 2: Value should go to the left subtree.
        if (val < root->data) {

            // If the left child exists, recurse into that subtree.
            if (root->leftchild != NULL)
                root->leftchild = insertR(root->leftchild, val);

            // Otherwise, create a new node and link it to the left.
            else
                root->leftchild = createNode(val);
        }

        // Case 3: Value should go to the right subtree.
        else {

            // If the right child exists, recurse into that subtree.
            if (root->rightchild != NULL)
                root->rightchild = insertR(root->rightchild, val);

            // Otherwise, create a new node and link it to the right.
            else
                root->rightchild = createNode(val);
        }

        // Return the (unchanged) root pointer after insertion.
        return root;
    }
}


// Recursive function to search for a node in the BST
treenode_t *search(treenode_t *root, int val) {
    if (root == NULL || root->data == val) {
        return root;
    } else if (val < root->data) {
        return search(root->leftchild, val);
    } else {
        return search(root->rightchild, val);
    }
}

// Find a node storing value in the subtree rooted at current_ptr - this is very similar to
// the search function we wrote above
treenode_t *findNode(treenode_t *current_ptr, int value) {
    if (current_ptr == NULL) return NULL;
    if (current_ptr->data == value) 
        return current_ptr;
    if (value < current_ptr->data) 
        return findNode(current_ptr->leftchild, value);
    return findNode(current_ptr->rightchild, value);
}

// Return the parent of node in the tree rooted at root (or NULL if none/root not found).
treenode_t *parent(treenode_t *root, treenode_t *node) {
    if (root == NULL || root == node) 
        return NULL;
    if (root->leftchild == node || root->rightchild == node) 
        return root;
    if (node->data < root->data) 
        return parent(root->leftchild, node);
    if (node->data > root->data) 
        return parent(root->rightchild, node);
    return NULL;
}

// Minimum value node in a (non-empty) subtree
treenode_t *minVal(treenode_t *root) {
    if (root == NULL) 
        return NULL;
    if (root->leftchild == NULL) 
        return root;
    return minVal(root->leftchild);
}

// Maximum value node in a (non-empty) subtree
treenode_t *maxVal(treenode_t *root) {
    if (root == NULL) 
        return NULL;
    if (root->rightchild == NULL) 
        return root;
    return maxVal(root->rightchild);
}

int isLeaf(treenode_t *node) {
    return (node != NULL && node->leftchild == NULL && node->rightchild == NULL);
}

int hasOnlyLeftChild(treenode_t *node) {
    return (node != NULL && node->leftchild != NULL && node->rightchild == NULL);
}

int hasOnlyRightChild(treenode_t *node) {
    return (node != NULL && node->leftchild == NULL && node->rightchild != NULL);
}

// Delete the node storing val in the BST rooted at root
// Returns the (possibly new) root after deletion.
treenode_t *removeNode(treenode_t *root, int val) {
    treenode_t *delnode = findNode(root, val);
    if (delnode == NULL) {
        // Value not found; return root unchanged.
        return root;
    }

    treenode_t *par = parent(root, delnode);

    // Case 1: leaf node
    if (isLeaf(delnode)) {
        if (par == NULL) {                // deleting the only node (root)
            free(root);
            return NULL;
        }
        if (val < par->data) {            // delnode is left child
            free(par->leftchild);
            par->leftchild = NULL;
        } else {                          // delnode is right child
            free(par->rightchild);
            par->rightchild = NULL;
        }
        return root;
    }

    // Case 2-a: only left child
    if (hasOnlyLeftChild(delnode)) {
        if (par == NULL) {                // deleting root
            treenode_t *save = delnode->leftchild;
            free(delnode);
            return save;
        }
        if (val < par->data) {            // delnode is left child
            treenode_t *save = par->leftchild;
            par->leftchild = par->leftchild->leftchild;
            free(save);
        } else {                          // delnode is right child
            treenode_t *save = par->rightchild;
            par->rightchild = par->rightchild->leftchild;
            free(save);
        }
        return root;
    }

    // Case 2-b: only right child
    if (hasOnlyRightChild(delnode)) {
        if (par == NULL) {                // deleting root
            treenode_t *save = delnode->rightchild;
            free(delnode);
            return save;
        }
        if (val < par->data) {            // delnode is left child
            treenode_t *save = par->leftchild;
            par->leftchild = par->leftchild->rightchild;
            free(save);
        } else {                          // delnode is right child
            treenode_t *save = par->rightchild;
            par->rightchild = par->rightchild->rightchild;
            free(save);
        }
        return root;
    }

    // Case 3: two children
    // If your code reaches hear it means delnode has two children
    // Find the new physical node to delete.
    treenode_t *new_del_node = minVal(delnode->rightchild);
    int save_val = new_del_node->data;

    root = removeNode(root, save_val);    // delete the successor node
    delnode->data = save_val;             // put minVal value into original node

    return root;
}

// Function to remove a node from the BST - Note that this code is more compact than the previous one but they
// both works the same
// treenode_t *removeNode(treenode_t *root, int val) {
//     if (root == NULL) {
//         printf("Node doesn't exist.\n");
//         return root;
//     }

//     if (val < root->data) {
//         root->leftchild = removeNode(root->leftchild, val);
//     } 
//     else if (val > root->data) {
//         root->rightchild = removeNode(root->rightchild, val);
//     } 
//     else {
//         // Case 1: No child
//         if (root->leftchild == NULL && root->rightchild == NULL) {
//             free(root);
//             return NULL;
//         }
//         // Case 2: One child (right child exists)
//         else if (root->leftchild == NULL) {
//             treenode_t *temp = root->rightchild;
//             free(root);
//             return temp;
//         }
//         // Case 2: One child (left child exists)
//         else if (root->rightchild == NULL) {
//             treenode_t *temp = root->leftchild;
//             free(root);
//             return temp;
//         }
//         // Case 3: Two children
//         treenode_t *temp = minVal(root->rightchild); // successor
//         root->data = temp->data;
//         root->rightchild = removeNode(root->rightchild, temp->data);
//     }

//     return root;
// }


// Function to perform an inorder traversal (Left, Root, Right)
void inorder(treenode_t *root) {
    if (root != NULL) {
        inorder(root->leftchild);
        printf("%d ", root->data);
        inorder(root->rightchild);
    }
}

// Function to perform a preorder traversal (Root, Left, Right)
void preorder(treenode_t *root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorder(root->leftchild);
        preorder(root->rightchild);
    }
}

// Function to perform a postorder traversal (Left, Right, Root)
void postorder(treenode_t *root) {
    if (root != NULL) {
        postorder(root->leftchild);
        postorder(root->rightchild);
        printf("%d ", root->data);
    }
}

// Function to deallocate all nodes in the tree
void destroyTree(treenode_t *root) {
    if (root != NULL) {
        destroyTree(root->leftchild);
        destroyTree(root->rightchild);
        free(root);
    }
}
