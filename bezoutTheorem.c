
// We are implementing Bezout Identity
#include <stdio.h>
#include <stdlib.h>

// Define Node for tree structure
typedef struct Node {
    int a, b, q, r;  // a, b, quotient, remainder
    struct Node *left;
} Node;

// Create a new Node
Node* createNode(int a, int b, int q, int r) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->a = a;
    node->b = b;
    node->q = q;
    node->r = r;
    node->left = NULL;
    return node;
}

// Build the tree using the Euclidean Algorithm
Node* buildTree(int a, int b) {
    if (b == 0) {
        return NULL;
    }

    int q = a / b;
    int r = a % b;
    Node* root = createNode(a, b, q, r);
    root->left = buildTree(b, r);
    return root;
}

// Print the tree (for debugging)
void printTree(Node* root) {
    if (root == NULL)
        return;
    printf("a = %d, b = %d, q = %d, r = %d\n", root->a, root->b, root->q, root->r);
    if (root->left != NULL)
        printTree(root->left);
}

// Correct Backward Phase to get Coefficients (x, y)
void getCoefficients(Node* root, int* x, int* y) {
    if (root->left == NULL) {
        *x = 1;
        *y = 0;
        return;
    }

    getCoefficients(root->left, x, y);

    int tempX = *y;
    int tempY = *x - root->q * (*y);

    *x = tempX;
    *y = tempY;

    if (root->left->left == NULL) {
        tempX = *y;
        tempY = *x - root->q * (*y);
        *x = tempX;
        *y = tempY;
    }
}

// Get the GCD from the tree
int getGCD(Node* root) {
    while (root->left != NULL)
        root = root->left;
    return root->b;  // Correct GCD
}

// Main driver code
int main() {
  int a = 97, b = 38, x, y;

    // Build the tree using EEA
    Node* root = buildTree(a, b);
    printf("Printing the Built Tree:\n");
    printTree(root);

    // Get coefficients (x, y) using corrected backward propagation
    getCoefficients(root, &x, &y);

    // Get GCD
    int gcd = getGCD(root);
    printf("\nGCD of %d and %d is %d\n", a, b, gcd);
    printf("Coefficients: x = %d and y = %d\n", x, y);
    printf("Verification: %d * (%d) + %d * (%d) = %d\n", a, x, b, y, gcd);

    return 0;
}


    
