#include <stdio.h>
#include <stdlib.h>

struct Node {
	int val;
	struct Node* left;
	struct Node* right;
};

typedef struct {
	struct Node *root;
	int level;
} BinaryTree;

enum NodePosition {
	LEFT,
	RIGHT,
	ROOT,
};

void insert(BinaryTree **tree, struct Node *node) 
{
	struct Node *prev;
	if(tree == NULL || (*tree) == NULL || node == NULL) return;
	struct Node **head = &(*tree)->root;
	while(*head) {
		prev = (*head);
		if(node->val > (*head)->val) head = &((*head)->right);
		else if(node->val < (*head)->val) head = &((*head)->left);
	}
	if(prev == NULL || ((prev->left == NULL) && (prev->right == NULL))) (*tree)->level++;
	(*head) = node;
}

void printnode(struct Node *head, int level, enum NodePosition np) 
{
	if(head) {
		const char *POS = ((np == LEFT) ? "LEFT: " : ((np == ROOT) ? "ROOT: " : "RIGHT: "));  
		printf("[%d] %s%.*s%d%.*s\n", level, POS, level + 1, "\t", head->val, level + 1, "\t");
		if(head->left) printnode(head->left, level - 1, LEFT);
		if(head->right) printnode(head->right, level - 1, RIGHT);
	}
}

void printtree(BinaryTree *tree)
{
	printf("Binary Tree\n");
	printf("levels: %d\n\n", tree->level);
	printnode(tree->root, tree->level, ROOT);
}

int main(void) 
{
	BinaryTree *tree;
	if(!(tree = (BinaryTree *) malloc(sizeof(BinaryTree)))) return 1;

	struct Node root = {
		.val = 5,
		.left = NULL,
		.right = NULL,
	};

	struct Node left = {
		.val = 2,
		.left = NULL,
		.right = NULL,
	};

	struct Node right = {
		.val = 9,
		.left = NULL,
		.right = NULL,
	};

	struct Node right_again = {
		.val = 12,
		.left = NULL,
		.right = NULL,
	};

	insert(&tree, &root);
	insert(&tree, &left);
	insert(&tree, &right);
	insert(&tree, &right_again);
	printtree(tree);
	free(tree);
}
