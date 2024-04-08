#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define LINESIZE 128

typedef struct Task Task;
typedef struct Node Node;
typedef struct NodeDLL NodeDLL;
typedef struct DoublyLinkedList DoublyLinkedList;

struct Task {
	int task_id;
	char* description;
	float resource_utilization;
};

struct NodeDLL {
	Task task;
	NodeDLL* prev;
	NodeDLL* next;
};

struct DoublyLinkedList {
	NodeDLL* head;
	NodeDLL* tail;
};

void printTaskToConsole(Task task) {
	printf("Task %d:\n\tDescription: %s\n\tResource utilization: %.2f%%\n\n", task.task_id, task.description, task.resource_utilization);
}

Task initTask(int task_id, char* description, float resource_utilization) {

	Task newTask = { .task_id = task_id, .resource_utilization = resource_utilization };

	newTask.description = malloc((strlen(description) + 1) * sizeof(char));
	strcpy_s(newTask.description, strlen(description) + 1, description);

	return newTask;
}

Task readTaskFromFile(FILE* f) {

	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	int task_id = (int)strtol(buffer, NULL, 10);

	fgets(buffer, LINESIZE, f);
	char* description = malloc(strlen(buffer) * sizeof(char));
	strncpy_s(description, strlen(buffer), buffer, strlen(buffer) - 1);

	fgets(buffer, LINESIZE, f);
	float resource_utilization = strtof(buffer, NULL);

	Task newTask = { .task_id = task_id, .description = description, .resource_utilization = resource_utilization };

	return newTask;
}

void readTasksFromFile(const char* filename, Task** tasksArray, int* tasksNumber) {

	FILE* f;
	fopen_s(&f, filename, "r");

	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	*tasksNumber = (int)strtol(buffer, NULL, 10);

	*tasksArray = malloc(*tasksNumber * sizeof(Task));

	for (int i = 0; i < *tasksNumber; i++) {
		(*tasksArray)[i] = readTaskFromFile(f);
	}

	fclose(f);
}


void insertAtBeginningDLL(DoublyLinkedList* dll, Task task) {

	NodeDLL* newNode = malloc(sizeof(NodeDLL));
	newNode->task = task;

	newNode->next = (*dll).head;
	newNode->prev = NULL;

	if ((*dll).head) {
		(*dll).head->prev = newNode;
	}
	else {
		(*dll).tail = newNode;
	}
	

	(*dll).head = newNode;
}

void insertAtEndDLL(DoublyLinkedList* dll, Task task) {

	NodeDLL* newNode = malloc(sizeof(NodeDLL));
	newNode->task = task;

	newNode->prev = (*dll).tail;
	newNode->next = NULL;

	if ((*dll).tail) {
		(*dll).tail->next = newNode;
	}
	else {
		(*dll).head = newNode;
	}

	(*dll).tail = newNode;
}

void parseListBtoE(DoublyLinkedList dll) {
	if (dll.head) {
		while (dll.head) {
			printTaskToConsole(dll.head->task);
			dll.head = dll.head->next;
		}
	}
	else {
		printf("List is empty\n");
	}
}

void parseListEtoB(DoublyLinkedList dll) {
	if (dll.tail) {
		while (dll.tail) {
			printTaskToConsole(dll.tail->task);
			dll.tail = dll.tail->prev;
		}
	}
	else {
		printf("List is empty\n");
	}
}

Task deleteFromBeginningDLL(DoublyLinkedList* dll) {

	if ((*dll).head) {

		NodeDLL* currentNode = (*dll).head;
		Task deletedTask = currentNode->task;

		if ((*dll).head->next) {
			(*dll).head = (*dll).head->next;
			(*dll).head->prev = NULL;
			free(currentNode);

			return deletedTask;
		}
		else {
			free(currentNode);
			(*dll).head = NULL;
			(*dll).tail = NULL;

			return deletedTask;
		}

		
	}

	return initTask(0, "ERROR TASK", 0);
}

Task deleteFromEndDLL(DoublyLinkedList* dll) {

	if ((*dll).tail) {

		NodeDLL* currentNode = (*dll).tail;
		Task deletedTask = currentNode->task;

		if ((*dll).tail->prev) {
			(*dll).tail = (*dll).tail->prev;
			(*dll).tail->next = NULL;
			free(currentNode);

			return deletedTask;
		}
		else {
			free(currentNode);
			(*dll).head = NULL;
			(*dll).tail = NULL;

			return deletedTask;
		}


	}

	return initTask(0, "ERROR TASK", 0);
}

int listLength(DoublyLinkedList dll) {
	int len = 0;
	while (dll.head) {
		len += 1;
		dll.head = dll.head->next;
	}
}


int main() {

	int* tasksNumber;
	Task* tasksArray;

	readTasksFromFile("scheduler.txt", &tasksArray, &tasksNumber);

	/*for (int i = 0; i < tasksNumber; i++) {
		printTaskToConsole(tasksArray[i]);
	}*/


	DoublyLinkedList dll;

	dll.head = NULL;
	dll.tail = NULL;

	insertAtBeginningDLL(&dll, tasksArray[0]);
	insertAtBeginningDLL(&dll, tasksArray[1]);
	insertAtBeginningDLL(&dll, tasksArray[1]);
	insertAtBeginningDLL(&dll, tasksArray[2]);
	insertAtBeginningDLL(&dll, tasksArray[3]);
	insertAtBeginningDLL(&dll, tasksArray[4]);
	insertAtEndDLL(&dll, tasksArray[0]);

	parseListBtoE(dll);

	printf("\n\n----\n\n");

	parseListBtoE(dll);

	printf("\n\n----\n\n");

	parseListEtoB(dll);

	Task dt = deleteFromBeginningDLL(&dll);
	printTaskToConsole(dt);

	dt = deleteFromEndDLL(&dll);

	printTaskToConsole(dt);

	return 0;
}