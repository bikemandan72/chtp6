/*
12.22 (Binary Tree Delete)
In this exercise, we discuss deleting items from binary search trees. The deletion
algorithm is not as straightforward as the insertion algorithm. There are three cases
that are encountered when deleting an item—the item is contained in a leaf node
(i.e., it has no children), the item is contained in a node that has one child, or
the item is contained in a node that has two children.
If the item to be deleted is contained in a leaf node, the node is deleted and the
pointer in the parent node is set to NULL .
If the item to be deleted is contained in a node with one child, the pointer in the
parent node is set to point to the child node and the node containing the data item
is deleted. This causes the child node to take the place of the deleted node in the
tree.
The last case is the most difficult. When a node with two children is deleted, another
node must take its place. However, the pointer in the parent node cannot simply be
assigned to point to one of the children of the node to be deleted. In most cases,
the resulting binary search tree would not adhere to the following characteristic of
binary search trees: The values in any left subtree are less than the value in the
parent node, and the values in any right subtree are greater than the value in the
parent node.
Which node is used as a replacement node to maintain this characteristic?
Either the node containing the largest value in the tree less than the value in the
node being deleted, or the node containing the smallest value in the tree greater
than the value in the node being deleted. Let’s consider the node with the smaller
value. In a binary search tree, the largest value less than a parent’s value is
located in the left subtree of the parent node and is guaranteed to be contained
in the rightmost node of the subtree. This node is located by walking down the
left subtree to the right until the pointer to the right child of the current node is
NULL . We’re now pointing to the replacement node which is either a leaf node or a
node with one child to its left. If the replacement node is a leaf node, the steps
to perform the deletion are as follows:
1) Store the pointer to the node to be deleted in a temporary pointer variable
(this pointer is used to delete the dynamically allocated memory).
2) Set the pointer in the parent of the node being deleted to point to the replacement
node.
3) Set the pointer in the parent of the replacement node to null.
4) Set the pointer to the right subtree in the replacement node to point to the right
subtree of the node to be deleted.
5) Delete the node to which the temporary pointer variable points.
The deletion steps for a replacement node with a left child are similar to those for
a replacement node with no children, but the algorithm also must move the child to the
replacement node’s position. If the replacement node is a node with a left child, the
steps to perform the deletion are as follows:
1) Store the pointer to the node to be deleted in a temporary pointer variable.
2) Set the pointer in the parent of the node being deleted to point to the replacement
node.
3) Set the pointer in the parent of the replacement node to point to the left child of
the replacement node.
4) Set the pointer to the right subtree in the replacement node to point to the right
subtree of the node to be deleted.
5) Delete the node to which the temporary pointer variable points.
Write function deleteNode which takes as its arguments a pointer to the root node of
the tree and the value to be deleted. The function should locate in the tree the node
containing the value to be deleted and use the algorithms discussed here to delete
the node. If the value is not found in the tree, the function should print a message
that indicates whether or not the value is deleted. Modify the program of Fig. 12.19
to use this function. After deleting an item, call the inOrder , preOrder and
postOrder traversal functions to confirm that the delete operation was performed
correctly.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// self-referential structure
struct treeNode {
	struct treeNode *leftPtr; // pointer to left subtree
	int data; // node value
	struct treeNode *rightPtr; // pointer to right subtree
}; 

typedef struct treeNode TreeNode; // synonym for struct treeNode
typedef TreeNode *TreeNodePtr; // synonym for TreeNode*

// prototypes
void insertNode(TreeNodePtr *treePtr, int value);
void inOrder(TreeNodePtr treePtr);
void preOrder(TreeNodePtr treePtr);
void postOrder(TreeNodePtr treePtr);
void outputTree(TreeNodePtr *treePtr);
void printIndentation(unsigned int depth);
void freeTree(TreeNodePtr *treePtr);
void deleteNode(TreeNodePtr *treePtr, int searchKey);

// function main begins program execution
int main(void)
{ 
	TreeNodePtr rootPtr = NULL; // tree initially empty

	//srand(time(NULL));
	int seed = 1699108271;
	srand(seed);
	//nice seeds: 1699107993, 1699108173, 1699108171, 1699108271
	puts("The numbers being placed in the tree are:");

	insertNode(&rootPtr, 10);
	// insert random values between 0 and 14 in the tree
	for (unsigned int i = 1; i <= 20; ++i) { 
		int item = rand() % 15;
		printf("%3d", item);
		insertNode(&rootPtr, item);
	}

	// traverse the tree preOrder
	puts("\n\nThe preOrder traversal is:");
	preOrder(rootPtr);

	// traverse the tree inOrder
	puts("\n\nThe inOrder traversal is:");
	inOrder(rootPtr);

	// traverse the tree postOrder
	puts("\n\nThe postOrder traversal is:");
	postOrder(rootPtr);
	
	puts("");
	printf("Random generator seed is %d\n", seed);
	printf("Root node is %d\n", rootPtr->data);
	puts("");
	outputTree(&rootPtr);
	puts("");
	
	int searchKey = 10;
	deleteNode(&rootPtr, searchKey);
	
	puts("");
	outputTree(&rootPtr);
	puts("");
	freeTree(&rootPtr);
}
void deleteNode(TreeNodePtr *treePtr, int searchKey)
{
	//search node with value searchKey
	static TreeNodePtr searchPtr = NULL, parentPtr = NULL;
	int bypass = 1;
	if (*treePtr != NULL) {
		if (searchKey < (*treePtr)->data) {
			parentPtr = *treePtr;
			deleteNode(&((*treePtr)->leftPtr), searchKey);
		} else if (searchKey > (*treePtr)->data) {
			parentPtr = *treePtr;
			deleteNode(&((*treePtr)->rightPtr), searchKey);
		} else if (searchKey == (*treePtr)->data) {
			searchPtr = *treePtr;
			bypass = 0;
		}
	}
	if (bypass) {
		return;
	}
	//delete node if value is found
	if (searchPtr != NULL) {
		printf("%d was found at node address %p (%d).\n",
			searchKey, searchPtr, searchPtr->data);
		if (parentPtr != NULL) {
			printf("Parent node address is %p (%d).\n",
				parentPtr, parentPtr->data);
			TreeNodePtr *child = NULL;
			if (parentPtr->leftPtr == searchPtr) {
				child = &(parentPtr->leftPtr);
			} else if (parentPtr->rightPtr == searchPtr) {
				child = &(parentPtr->rightPtr);
			}
			if (searchPtr->leftPtr == NULL && searchPtr->rightPtr == NULL) {
				//node is a leaf
				*child = NULL;
			} else if (searchPtr->leftPtr != NULL && searchPtr->rightPtr == NULL) {
				//node is neither a leaf nor a root and has left child
				*child = searchPtr->leftPtr;
			} else if (searchPtr->leftPtr == NULL && searchPtr->rightPtr != NULL) {
				//node is neither a leaf nor a root and has right child
				*child = searchPtr->rightPtr;
			} else {
				//node is neither a leaf nor a root and has 2 childs
				//we voluntarily choose the right child to be connected to parent
				*child = searchPtr->rightPtr;
				//now look for the leftmost child
				TreeNodePtr leftmostPtr = searchPtr->rightPtr;
				while (leftmostPtr->leftPtr != NULL) {
					leftmostPtr = leftmostPtr->leftPtr;
				}
				//and connect it to the left child of searchPtr
				leftmostPtr->leftPtr = searchPtr->leftPtr;
			}
		} else {
			//node is a root of the tree
			printf("Value %d found at the root node of the tree %d(%p).\n",
				searchKey, (*treePtr)->data, *treePtr);
			if (searchPtr->leftPtr == NULL && searchPtr->rightPtr == NULL) {
				//tree contains only the root
				*treePtr = NULL;
			} else if (searchPtr->leftPtr != NULL && searchPtr->rightPtr == NULL) {
				//tree only has left subtree
				*treePtr = searchPtr->leftPtr;
			} else if (searchPtr->leftPtr == NULL && searchPtr->rightPtr != NULL) {
				//tree only has right subtree
				*treePtr = searchPtr->rightPtr;
			} else {
				//tree has 2 subtrees
				//we voluntarily choose the right subtree to be the new root
				*treePtr = searchPtr->rightPtr;
				//now look for the leftmost child
				TreeNodePtr leftmostPtr = searchPtr->rightPtr;
				while (leftmostPtr->leftPtr != NULL) {
					leftmostPtr = leftmostPtr->leftPtr;
				}
				//and connect it to the left child of searchPtr
				leftmostPtr->leftPtr = searchPtr->leftPtr;
			}
		}
		free(searchPtr);
		searchPtr = NULL;
	} else {
		printf("%d wasn't found in the tree %p.\n", searchKey, *treePtr);
	}
}

void outputTree(TreeNodePtr *treePtr) {
	static unsigned int depth = 0;
	if (*treePtr != NULL) {
		depth++;
		outputTree(&((*treePtr)->rightPtr));
		depth--;
		printIndentation(depth);
		printf("%d(%p)\n", (*treePtr)->data, *treePtr);
		depth++;
		outputTree(&((*treePtr)->leftPtr));
		depth--;
	} else {
		printIndentation(depth);
		puts("NULL");
	}
}
void freeTree(TreeNodePtr *treePtr) {
	if (*treePtr != NULL) {
		freeTree(&((*treePtr)->leftPtr));
		freeTree(&((*treePtr)->rightPtr));
		free((*treePtr)->leftPtr);
		(*treePtr)->leftPtr = NULL;
		free((*treePtr)->rightPtr);
		(*treePtr)->rightPtr = NULL;
		free(*treePtr);
		*treePtr = NULL;
	}
}
void printIndentation(unsigned int depth) {
	for (size_t i = 0; i < depth; ++i) {
		printf("%c", '\t');
	}
}
// insert node into tree
void insertNode(TreeNodePtr *treePtr, int value)
{ 
	// if tree is empty
	if (*treePtr == NULL) {
		*treePtr = malloc(sizeof(TreeNode));
		// if memory was allocated, then assign data
		if (*treePtr != NULL) { 
			(*treePtr)->data = value;
			(*treePtr)->leftPtr = NULL;
			(*treePtr)->rightPtr = NULL;
		} else {
			printf("%d not inserted. No memory available.\n", value);
		}
	} else { // tree is not empty
		// data to insert is less than data in current node
		if (value < (*treePtr)->data) {
			insertNode(&((*treePtr)->leftPtr), value);
		}// data to insert is greater than data in current node
		else if (value > (*treePtr)->data) {
			insertNode(&((*treePtr)->rightPtr), value);
		} else { // duplicate data value ignored
			printf("%s", "dup");
		}
	}
}

// begin inorder traversal of tree
void inOrder(TreeNodePtr treePtr)
{ 
	// if tree is not empty, then traverse
	if (treePtr != NULL) {
		inOrder(treePtr->leftPtr);
		printf("%3d", treePtr->data);
		inOrder(treePtr->rightPtr);
	}
}

// begin preorder traversal of tree
void preOrder(TreeNodePtr treePtr)
{
	// if tree is not empty, then traverse
	if (treePtr != NULL) {
		printf("%3d", treePtr->data);
		preOrder(treePtr->leftPtr);
		preOrder(treePtr->rightPtr);
	}
}

// begin postorder traversal of tree
void postOrder(TreeNodePtr treePtr)
{ 
	// if tree is not empty, then traverse
	if (treePtr != NULL) {
		postOrder(treePtr->leftPtr);
		postOrder(treePtr->rightPtr);
		printf("%3d", treePtr->data);
	}
}