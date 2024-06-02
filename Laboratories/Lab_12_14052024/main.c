#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define LINESIZE 128
#define COUNT 10

typedef struct FileProperties FileProperties;
typedef struct TreeNode TreeNode;

struct FileProperties {
	int fileId;
	char* fileName;
	int size;
	char isFolder;
};

struct TreeNode {
	FileProperties fileProperties;
	TreeNode* left;
	TreeNode* right;
};


FileProperties readOneFromFile(FILE* f) {
	
	FileProperties fileProperties;
	char buffer[LINESIZE];

	char* token = NULL;
	char* data = NULL;

	fgets(buffer, LINESIZE, f);
	fileProperties.fileId = (int) strtol(strtok_s(buffer, ",", &token), NULL, 10);
	
	data = strtok_s(NULL, ",", &token);
	fileProperties.fileName = malloc((strlen(data) + 1) * sizeof(char));
	strcpy_s(fileProperties.fileName, strlen(data) + 1, data);

	fileProperties.size = (int)strtol(strtok_s(NULL, ",", &token), NULL, 10);

	fileProperties.isFolder = (char)strtol(strtok_s(NULL, ",", &token), NULL, 10);


	return fileProperties;
}

void readAllFromFile(const char* filename, FileProperties** filePropertiesArray, int* filePropertiesCount) {

	FILE* f;
	fopen_s(&f, filename, "r");

	int filePropertiesRead = 0;
	*filePropertiesArray = malloc(filePropertiesRead * sizeof(FileProperties));

	while (!feof(f)) {

		*filePropertiesArray = realloc(*filePropertiesArray, (++filePropertiesRead) * sizeof(FileProperties));
		(*filePropertiesArray)[filePropertiesRead - 1] = readOneFromFile(f);

	}

	*filePropertiesCount = filePropertiesRead;

	fclose(f);
}

void printFilePropertiesToConsole(FileProperties fileProperties) {
	printf("%d. %s - %d B %d\n", fileProperties.fileId, fileProperties.fileName, fileProperties.size, fileProperties.isFolder);
}

void insertTreeNode(TreeNode** root, FileProperties fileProperties) {
	if (*root) {
		if ((*root)->fileProperties.fileId > fileProperties.fileId) {
			insertTreeNode(&(*root)->left, fileProperties);
		}
		else {
			insertTreeNode(&(*root)->right, fileProperties);
		}
	}
	else {
		TreeNode* newNode = malloc(sizeof(TreeNode));
		newNode->fileProperties = fileProperties;
		newNode->left = NULL;
		newNode->right = NULL;
		*root = newNode;
	}
}

void inorderTraversal(TreeNode* root) {
	if (root) {
		inorderTraversal(root->left);
		printf("%d ", root->fileProperties.fileId);
		inorderTraversal(root->right);
	}
}

void preorderTraversal(TreeNode* root) {
	if (root) {
		printf("%d ", root->fileProperties.fileId);
		preorderTraversal(root->left);
		preorderTraversal(root->right);
	}
	
}

void postorderTraversal(TreeNode* root) {
	if (root) {
		postorderTraversal(root->left);
		postorderTraversal(root->right);
		printf("%d ", root->fileProperties.fileId);
	}
}

FileProperties searchTree(TreeNode* root, int fileId) {
	if (root) {
		if (root->fileProperties.fileId == fileId) {
			return root->fileProperties;
		}

		if (root->fileProperties.fileId > fileId) {
			return searchTree(root->left, fileId);
		}
		else {
			return searchTree(root->right, fileId);
		}
	}
	else {
		FileProperties fileProperties = { .fileId = 0, .fileName = "ERR", .isFolder = 0, .size = 0};
		return fileProperties;
	}
}

int treeHeight(TreeNode* root) {
	if (root) {
		int leftHeight = treeHeight(root->left);
		int rightHeight = treeHeight(root->right);
		return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
	}
	else {
		return 0;
	}
}

void printTree(TreeNode* root, int space) {
	if (root) {
		
		space += COUNT;

		printTree(root->right, space);

		printf("\n");
		for (int i = COUNT; i < space; i++) {
			printf(" ");
		}
		printf("%d\n", root->fileProperties.fileId);

		printTree(root->left, space);
	}
}

int main() {

	int filePropertiesCount;
	FileProperties* filePropertiesArray;

	readAllFromFile("data.txt", &filePropertiesArray, &filePropertiesCount);

	for (int i = 0; i < filePropertiesCount; i++) {
		printFilePropertiesToConsole(filePropertiesArray[i]);
	}

	TreeNode* root = NULL;

	insertTreeNode(&root, filePropertiesArray[4]);
	insertTreeNode(&root, filePropertiesArray[1]);
	insertTreeNode(&root, filePropertiesArray[6]);
	insertTreeNode(&root, filePropertiesArray[0]);
	insertTreeNode(&root, filePropertiesArray[2]);
	insertTreeNode(&root, filePropertiesArray[5]);
	insertTreeNode(&root, filePropertiesArray[7]);

	inorderTraversal(root);
	printf("\n");

	preorderTraversal(root);
	printf("\n");

	postorderTraversal(root);
	printf("\n");

	printTree(root, 20);

	printFilePropertiesToConsole(searchTree(root, 8));

	return 0;
}


// Code for an ordinary tree
/*

void searchNode(TreeNode* root, int key, TreeNode** found) {

	if (root) {
		printf("Current node is: %d\n", root->fileProperties.fileId);

		if (key == root->fileProperties.fileId) {
			*found = root;
		}
		else {
			searchNode(root->child, key, found);

			if (!(*found)) {

				if (root->child) {
					TreeNode* tmp = root->child;
					while (tmp->sibling) {
						if (!(*found)) {
							tmp = tmp->sibling;
						}
						searchNode(tmp, key, found);
					}
				}
			}
		}
	}
}

void insertInTree(TreeNode** root, FileProperties fileProperties, int parentId) {

	TreeNode* newNode = malloc(sizeof(TreeNode));
	newNode->fileProperties = fileProperties;
	newNode->sibling = NULL;

	if (!parentId) {
		printf("\n Insert root node: %d\n", newNode->fileProperties.fileId);
		newNode->child = *root;
		*root = newNode;
	}
	else {
		newNode->child = NULL;

		TreeNode* parent = NULL;
		printf("\n Search parent node by key %d\n", parentId);


		searchNode(*root, parentId, &parent);

		if (parent && !parent->child) {

			parent->child = newNode;
		}
		else {
			if (parent) {

				if (!parent->child->sibling) {
					parent->child->sibling = newNode;
				}
				else {
					TreeNode* tmp = parent->child;

					while (tmp->sibling) {
						tmp = tmp->sibling;
					}

					tmp->sibling = newNode;
				}
			}
			else {
				printf("Parent not found");
				free(newNode);
			}
		}
	}

}

void preorderTraversal(struct TreeNode* root) {
	if (root) {
		printf("%d  ", root->fileProperties.fileId); // prelucrare nod curent

		preorderTraversal(root->child); // parsez primul subarbore descendent lui r

		if (root->sibling) {
			struct TreeNode* tmp = root->child;
			while (tmp->sibling) { // parsez restul de subarbori descendenti lui r
				preorderTraversal(tmp->sibling);
				tmp = tmp->sibling;
			}
		}
	}
}
*/