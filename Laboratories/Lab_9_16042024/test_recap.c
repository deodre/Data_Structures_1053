#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 128

typedef struct Task Task;
typedef struct Node Node;

struct Task {
	int taskId;
	char* description;
	float resourceUtilization;
};

void printTaskToConsole(Task task) {
	printf("Task %d:\n\tDescription: %s\tResource utilization: %.2f%%\n", task.taskId, task.description, task.resourceUtilization);
}

Task readTaskFromFile(FILE* f) {

	char buffer[LINESIZE];

	Task task;

	fgets(buffer, LINESIZE, f);
	task.taskId = (int)strtol(buffer, NULL, 10);

	fgets(buffer, LINESIZE, f);
	task.description = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy_s(task.description, strlen(buffer) + 1, buffer);

	fgets(buffer, LINESIZE, f);
	task.resourceUtilization = strtof(buffer, NULL);

	return task;
}

void readTasksFromFile(const char* filename, struct Task** tasksArray, int* tasksNumber) {

	FILE* f;
	fopen_s(&f, filename, "r");

	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	*tasksNumber = (int)strtol(buffer, NULL, 10);

	*tasksArray = malloc((*tasksNumber) * sizeof(Task));

	for (int i = 0; i < *tasksNumber; i++) {
		(*tasksArray)[i] = readTaskFromFile(f);
	}

	fclose(f);
}

struct Node {
	Task task;
	Node* next;
};

Task deepCopyTask(Task task) {
	Task copyTask;
	copyTask.taskId = task.taskId;
	copyTask.description = malloc((strlen(task.description) + 1) * sizeof(char));
	strcpy_s(copyTask.description, strlen(task.description) + 1, task.description);
	copyTask.resourceUtilization = task.resourceUtilization;
	return copyTask;
}

void insertAtBeginning(Node** head, Task task) {

	Node* newNode = malloc(sizeof(Node));

	newNode->task = task;
	newNode->next = *head;

	*head = newNode;

}

void insertAtEnd(Node** head, Task task) {
	if (*head) {

		Node* aux = *head;

		while (aux->next) {
			aux = aux->next;
		}

		Node* newNode = malloc(sizeof(Node));

		newNode->task = task;
		newNode->next = NULL;

		aux->next = newNode;
	}
	else {
		Node* newNode = malloc(sizeof(Node));

		newNode->task = task;
		newNode->next = *head;

		*head = newNode;
	}
}

void parseListAndPrint(Node* head) {
	if (head) {
		while (head) {
			printTaskToConsole(head->task);
			head = head->next;
		}
	}
	else {
		printf("List is empty\n");
	}
}

Task deleteFromBeginning(Node** head) {

	if (*head) {
		Task deletedTask;
		Node* oldHead = *head;

		*head = (*head)->next;

		deletedTask = oldHead->task;
		free(oldHead);

		return deletedTask;
	}
	else {
		Task errorTask = { .taskId = 0, .description = "ERROR TASK\n", .resourceUtilization = 0 };
		return errorTask;
	}
}

Task deleteFromEnd(Node** head) {

	if (*head) {

		if ((*head)->next) {

			Node* aux = *head;

			while (aux->next->next) { // positioning on the last but one node
				aux = aux->next;
			}
			
			// aux = last but one node
			// aux->next = the last node

			Task task = aux->next->task;

			free(aux->next);

			aux->next = NULL;

			return task;

		}
		else {
			
			Task task = (*head)->task;

			free(*head);

			*head = NULL;

			return task;
		}


	}
	else {
		Task empty = { .taskId = 0, .description = "???", .resourceUtilization = 0 };
		return empty;
	}

}

Task deleteWithCondition(Node** head, int taskId) {
	if (*head) {

		if ((*head)->task.taskId == taskId) {
			return deleteFromBeginning(head);
		}

		if ((*head)->next) {

			Node* aux = *head;

			while (aux->next && (aux->next->task.taskId != taskId)) { 
				aux = aux->next;
			}

			if (!aux->next) {
				printf("Item not found\n");
				Task empty = { .taskId = 0, .description = "???", .resourceUtilization = 0 };
				return empty;
			}

			Task task = aux->next->task; // save data

			Node* deletedNode = aux->next; // save node for later freeing

			aux->next = aux->next->next; // update link

			free(deletedNode); // free memory of deleted node

			return task;

		}
		else {

			if ((*head)->task.taskId == taskId) {
				Task task = (*head)->task;

				free(*head);

				*head = NULL;

				return task;
			}
			else {
				Task empty = { .taskId = 0, .description = "???", .resourceUtilization = 0 };
				return empty;
			}


		
		}


	}
	else {
		Task empty = { .taskId = 0, .description = "???", .resourceUtilization = 0 };
		return empty;
	}
}


int main() {

	int tasksNumber;
	Task* tasksArray;

	readTasksFromFile("scheduler.txt", &tasksArray, &tasksNumber);

	for (int i = 0; i < tasksNumber; i++) {
		printTaskToConsole(tasksArray[i]);
	}

	printf("---------------------\n");

	Node* head = NULL;

	insertAtBeginning(&head, tasksArray[0]);
	insertAtBeginning(&head, deepCopyTask(tasksArray[1]));
	insertAtBeginning(&head, tasksArray[2]);
	insertAtEnd(&head, tasksArray[3]);

	parseListAndPrint(head);

	printf("\nDeleted task: \n");

	Task deletedTask = deleteFromBeginning(&head);
	printTaskToConsole(deletedTask);

	printf("\nList after one delete: \n");
	parseListAndPrint(head);

	printf("-------------------\n\n");

	/*deletedTask = deleteFromBeginning(&head);
	free(deletedTask.description);*/

	//printTaskToConsole(tasksArray[1]);

	//deletedTask = deleteFromBeginning(&head);
	//deletedTask = deleteFromBeginning(&head);
	//deletedTask = deleteFromBeginning(&head);
	//parseListAndPrint(head);

	//printf("-------------------\n\n");

	//deletedTask = deleteFromBeginning(&head);
	//printTaskToConsole(deletedTask);

	//1

	//printf("Deleted element: \n");
	//deletedTask = deleteFromEnd(&head);
	//printTaskToConsole(deletedTask);
	//printf("-------------------\n\n");

	//printf("List after deleting from end\n");

	//parseListAndPrint(head);

	//printf("-------------------\n\n");

	//

	//


	////2

	//printf("Deleted element: \n");
	//deletedTask = deleteFromEnd(&head);
	//printTaskToConsole(deletedTask);
	//printf("-------------------\n\n");

	//printf("List after deleting from end\n");

	//parseListAndPrint(head);

	//printf("-------------------\n\n");


	////3 
	//printf("Deleted element: \n");
	//deletedTask = deleteFromEnd(&head);
	//printTaskToConsole(deletedTask);
	//printf("-------------------\n\n");

	//printf("List after deleting from end\n");

	//parseListAndPrint(head);

	//printf("-------------------\n\n");



	//// 4
	//printf("Deleted element: \n");
	//deletedTask = deleteFromEnd(&head);
	//printTaskToConsole(deletedTask);
	//printf("-------------------\n\n");

	//printf("List after deleting from end\n");

	//parseListAndPrint(head);

	//printf("-------------------\n\n");

	printf("Deleted element: \n");
	deletedTask = deleteWithCondition(&head, 100);
	printTaskToConsole(deletedTask);
	printf("-------------------\n\n");


	parseListAndPrint(head);

	printf("-------------------\n\n");

	printf("Deleted element: \n");
	deletedTask = deleteWithCondition(&head, 200);
	printTaskToConsole(deletedTask);
	printf("-------------------\n\n");


	parseListAndPrint(head);

	printf("-------------------\n\n");

	printf("Deleted element: \n");
	deletedTask = deleteWithCondition(&head, 300);
	printTaskToConsole(deletedTask);
	printf("-------------------\n\n");


	parseListAndPrint(head);

	printf("-------------------\n\n");

	printf("Deleted element: \n");
	deletedTask = deleteWithCondition(&head, 400);
	printTaskToConsole(deletedTask);
	printf("-------------------\n\n");


	parseListAndPrint(head);

	printf("-------------------\n\n");

	printf("Deleted element: \n");

	deletedTask = deleteWithCondition(&head, 400);
	printTaskToConsole(deletedTask);
	printf("-------------------\n\n");


	parseListAndPrint(head);

	printf("-------------------\n\n");
	return 0;
}
