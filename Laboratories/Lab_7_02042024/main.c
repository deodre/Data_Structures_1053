#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define LINESIZE 128

typedef struct Task Task;
typedef struct Node Node;
typedef struct HashTable HT;

struct Task {
	int taskId;
	char* description;
	float resourceUtil;
};

struct Node {
	Task task;
	Node* next;
};

struct HashTable {
	int size;
	Node** lists;
};

Task initTask(int taskId, char* description, float resourceUtil) {
	Task task = { .taskId = taskId, .resourceUtil = resourceUtil };
	task.description = malloc((strlen(description) + 1) * sizeof(char));
	strcpy(task.description, description);
	return task;
}

Task readTaskFromFile(FILE* f) {

	Task task;

	char buffer[LINESIZE];
	fgets(buffer, LINESIZE, f);
	task.taskId = (int)strtol(buffer, NULL, 10);

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = '\0';
	task.description = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(task.description, buffer);

	fgets(buffer, LINESIZE, f);
	task.resourceUtil = strtof(buffer, NULL); 
	
	return task;
}

void readTasksFromFile(const char* filename, Task** tasksArray, int* tasksNumber) {

	FILE* f;
	f = fopen(filename, "r");

	int readTasks = 0;

	*tasksArray = malloc(readTasks * sizeof(Task));

	while (!feof(f)) {
		Task task = readTaskFromFile(f);
		readTasks += 1;
		*tasksArray = realloc(*tasksArray, readTasks * sizeof(Task));
		(*tasksArray)[readTasks - 1] = task;
	}

	*tasksNumber = readTasks;

}

void printTaskToConsole(Task task) {
	printf("Task %d - %s - %.2f%%\n", task.taskId, task.description, task.resourceUtil);
}

int hashFunction(int htSize, char* key) {
	int sum = 0;
	for (int i = 0; i < strlen(key); i++) {
		sum += key[i];
	}
	return sum % htSize;
}

HT initHashTable(int size) {
	HT ht;
	ht.size = size;
	ht.lists = malloc(ht.size * sizeof(Node*));

	for (int i = 0; i < ht.size; i++) {
		ht.lists[i] = NULL;
	}

	return ht;
}

void insertList(Node** head, Task task) {

	Node* newNode = malloc(sizeof(Node));
	newNode->task = task;
	newNode->next = NULL;


	Node* aux = *head;

	int different = strcmp(aux->task.description, task.description);

	while (different && aux->next) {
		aux = aux->next;
		different = strcmp(aux->task.description, task.description);
	}

	if (!different) {
		free(newNode);
		aux->task = task;
	}
	else {
		aux->next = newNode;
	}
	
}

void insertHashTable(HT ht, Task task) {

	int position = hashFunction(ht.size, task.description);

	if (ht.lists[position]) {
		// collision detected
		insertList(&ht.lists[position], task);
	}
	else {
		// empty bucket
		Node* newNode = malloc(sizeof(Node));
		newNode->task = task;
		newNode->next = NULL;

		ht.lists[position] = newNode;
	}

}

void parseListAndPrint(Node* head) {
	if (head) {
		while (head) {
			printf("\t");
			printTaskToConsole(head->task);
			head = head->next;
		}
	}
	else {
		printf("\tEmpty bucket\n");
	}
}

void printHashTable(HT ht) {
	for (int i = 0; i < ht.size; i++) {
		printf("Position %d:\n", i);
		parseListAndPrint(ht.lists[i]);
	}
}

Task searchHashTable(HT ht, char* key) {

	int position = hashFunction(ht.size, key);

	Node* aux = ht.lists[position];

	while (aux && strcmp(key, aux->task.description)) {
		aux = aux->next;
	}

	if (aux) {
		printf("\nFound at position %d: ", position);
		return initTask(aux->task.taskId, aux->task.description, aux->task.resourceUtil);
	}
	else {
		printf("\nFound at position %d: ", position);
		Task err = { .taskId = 0, .description = "???", .resourceUtil = 0 };
		return err;
	}

}

int main() {

	int tasksNumber;
	Task* tasksArray;

	readTasksFromFile("scheduler.txt", &tasksArray, &tasksNumber);

	for (int i = 0; i < tasksNumber; i++) {
		printTaskToConsole(tasksArray[i]);
	}

	printf("\n------------\n");

	HT ht = initHashTable(tasksNumber);

	insertHashTable(ht, initTask(tasksArray[0].taskId, tasksArray[0].description, tasksArray[0].resourceUtil));
	insertHashTable(ht, initTask(tasksArray[1].taskId, tasksArray[1].description, tasksArray[1].resourceUtil));
	insertHashTable(ht, initTask(tasksArray[2].taskId, tasksArray[2].description, tasksArray[2].resourceUtil));
	insertHashTable(ht, initTask(tasksArray[3].taskId, tasksArray[3].description, tasksArray[3].resourceUtil));
	insertHashTable(ht, initTask(tasksArray[4].taskId, tasksArray[4].description, tasksArray[4].resourceUtil));
	insertHashTable(ht, initTask(tasksArray[5].taskId, tasksArray[5].description, tasksArray[5].resourceUtil));
	insertHashTable(ht, initTask(tasksArray[6].taskId, tasksArray[6].description, tasksArray[6].resourceUtil));
	insertHashTable(ht, initTask(tasksArray[7].taskId, tasksArray[7].description, tasksArray[7].resourceUtil));

	printHashTable(ht);

	printf("\n------------\n");

	printTaskToConsole(searchHashTable(ht, "Code Editor"));

	printTaskToConsole(searchHashTable(ht, "Antivirus"));

	printTaskToConsole(searchHashTable(ht, "File Explorer"));

	printf("\n------------\n");

	insertHashTable(ht, initTask(10000, tasksArray[6].description, 100));

	printHashTable(ht);

	return 0;
}