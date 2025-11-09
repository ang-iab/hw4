#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int findH(Node* root)
{
    int lenLeft = 0;
    int lenRight = 0;

    if (root->left != nullptr)
    {
        lenLeft = 1 + findH(root->left);
    }
    
    if (root->right != nullptr)
    {
        lenRight =  1 + findH(root->right);
    }

    if (lenLeft > lenRight) return lenLeft;
    return lenRight;
}

bool equalPaths(Node * root)
{
    // Add your code below
    if (root == nullptr) return false;
    else if (root->left == nullptr && root->right == nullptr) return true;
    else if (root->left == nullptr || root->right == nullptr) return false;

    int leftH = findH(root->left);
    int rightH = findH(root->right);

    if (leftH != rightH) return false;

    return equalPaths(root->left) && equalPaths(root->right);
}

