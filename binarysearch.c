#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct nodeBST
{
    int fileNo;
    int blockNo;
    struct nodeBST *left;
    struct nodeBST *right;
} * root;

struct nodeBST *createNode(int value, int value2);
struct nodeBST *insert(struct nodeBST *root, int fileNo, int blockNo);
struct nodeBST *minValueNode(struct nodeBST *node);
bool checkFileExist(struct nodeBST *root, int fileNo);
struct nodeBST *deleteNode(struct nodeBST *root, int fileNo);
void inorder(struct nodeBST *root);

struct nodeBST *createNode(int value, int value2)
{
    struct nodeBST *newNode = malloc(sizeof(struct nodeBST));
    newNode->fileNo = value;
    newNode->blockNo = value2;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

struct nodeBST *insert(struct nodeBST *root, int fileNo, int blockNo)
{
    if (root == NULL)
        return createNode(fileNo, blockNo);

    //if fileNo is lesser than the root fileNo
    if (fileNo < root->fileNo)
        root->left = insert(root->left, fileNo, blockNo);
    else if (fileNo == root->fileNo) //if fileNo is the same, meaning entries from same file
    {
        if (blockNo < root->blockNo)
            root->left = insert(root->left, fileNo, blockNo);
        else if (blockNo > root->blockNo)
            root->right = insert(root->right, fileNo, blockNo);
    }
    else if (fileNo > root->fileNo)
        root->right = insert(root->right, fileNo, blockNo);
    //inorder(root);
    //printf("\n");
    return root;
}

struct nodeBST *minValueNode(struct nodeBST *node)
{
    struct nodeBST *current = node;

    /* loop down to find the leftmost leaf */
    while (current && current->left != NULL)
        current = current->left;

    return current;
}

bool checkFileExist(struct nodeBST *root, int fileNo)
{
    struct nodeBST *temp;
    temp = root;
    if (temp == NULL)
        return true;
    if (temp->fileNo == fileNo)
    {
        return false;
    }
    checkFileExist(temp->left, fileNo);
    checkFileExist(temp->right, fileNo);
    return true;
}

//Delete by FileNo Only.
struct nodeBST *deleteNode(struct nodeBST *root, int fileNo)
{
    // base case
    if (root == NULL)
        return root;
    if (fileNo < root->fileNo)
        root->left = deleteNode(root->left, fileNo);
    else if (fileNo > root->fileNo)
        root->right = deleteNode(root->right, fileNo);
    else
    {
        // node with only one child or no child
        if (root->left == NULL)
        {
            struct nodeBST *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct nodeBST *temp = root->left;
            free(root);
            return temp;
        }

        // node with two children: Get successor
        struct nodeBST *temp = minValueNode(root->right);

        // Copy the successor's content to this node
        root->fileNo = temp->fileNo;

        // Delete the successor
        root->right = deleteNode(root->right, temp->fileNo);
    }
    if (checkFileExist(root, fileNo) == false)
        deleteNode(root, fileNo);
    return root;
}

//Delete by specific Block No which stores part of the file
struct nodeBST *deleteNodebyBlock(struct nodeBST *root, int fileNo, int blockNo)
{
    // base case
    if (root == NULL)
        return root;
    if (fileNo < root->fileNo)
        root->left = deleteNodebyBlock(root->left, fileNo, blockNo);
    else if (fileNo > root->fileNo)
        root->right = deleteNodebyBlock(root->right, fileNo, blockNo);
    else
    {
        //Check if blockNo same as fileNo is the same
        if (blockNo == root->blockNo)
        {
            // node with only one child or no child
            if (root->left == NULL)
            {
                struct nodeBST *temp = root->right;
                free(root);
                return temp;
            }
            else if (root->right == NULL)
            {
                struct nodeBST *temp = root->left;
                free(root);
                return temp;
            }

            // node with two children: Get successor
            struct nodeBST *temp = minValueNode(root->right);

            // Copy the successor's content to this node
            root->fileNo = temp->fileNo;

            // Delete the successor
            root->right = deleteNodebyBlock(root->right, temp->fileNo, blockNo);
        }
        else if (blockNo < root->blockNo)
            root->left = deleteNodebyBlock(root->left, fileNo, blockNo);
        else if (blockNo > root->blockNo)
            root->right = deleteNodebyBlock(root->right, fileNo, blockNo);
    }
    return root;
}

void inorder(struct nodeBST *root)
{
    if (root == NULL)
        return;
    inorder(root->left);
    printf("%d, %d ->", root->fileNo, root->blockNo);
    inorder(root->right);
}

// int main() {
//     root = insert(root, 100, 8);
//     insert(root, 100, 3);
//     insert(root, 100, 10);
//     insert(root, 200, 6);
//     insert(root, 200, 7);
//     insert(root, 300, 10);
//     insert(root, 300, 14);
//     insert(root, 300, 4);

//     inorder(root);
// }