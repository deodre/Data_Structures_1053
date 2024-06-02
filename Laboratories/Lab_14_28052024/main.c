#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct City City;
typedef struct PrimaryNode PNode;
typedef struct SecondaryNode SNode;
typedef struct StackNode StackNode;

struct City {
	int id;
	char* name;
};;

struct PrimaryNode {
	City city;
	SNode* neighbour;
	PNode* next;
};

struct SecondaryNode {
	PNode* primaryNodeAddress;
	SNode* next;
};

struct StackNode {
	int id;
	StackNode* next;
};

void push(StackNode** head, int id) {
	StackNode* newNode = malloc(sizeof(StackNode));
	newNode->id = id;
	newNode->next = *head;
	*head = newNode;
}

int pop(StackNode** head) {
	int result = (*head)->id;
	StackNode* oldHead = *head;
	*head = (*head)->next;
	free(oldHead);
	return result;
}

void insertPrimaryList(PNode** head, City city) {
	PNode* newNode = malloc(sizeof(PNode));
	newNode->city = city;
	newNode->neighbour = NULL;
	newNode->next = NULL;

	if (*head) {
		PNode* aux = *head;

		while (aux->next) {
			aux = aux->next;
		}

		aux->next = newNode;
	}
	else {
		*head = newNode;
	}
}

void printCity(City city) {
	printf("%d. %s\n", city.id, city.name);
}

void printSecondaryListIDs(SNode* head) {
	printf(" ");
	while (head) {
		printf("-> %d ", head->primaryNodeAddress->city.id);
		head = head->next;
	}
	printf("\n--------------\n");
}

void printPrimaryList(PNode* head) {
	while (head) {
		printCity(head->city);
		printSecondaryListIDs(head->neighbour);
		head = head->next;
	}
}

PNode* searchNodeByID(PNode* graph, int id) {
	while (graph && graph->city.id != id) {
		graph = graph->next;
	}
	return graph;
}

void insertSecondaryList(SNode** head, PNode* pNode) {
	
	SNode* newNode = malloc(sizeof(SNode));
	newNode->next = NULL;
	newNode->primaryNodeAddress = pNode;

	if (*head) {
		SNode* aux = *head;
		while (aux->next) {
			aux = aux->next;
		}
		aux->next = newNode;
	}
	else {
		*head = newNode;
	}

}

void addEdge(PNode* graph, int startNodeId, int endNodeId) {
	PNode* startNode = searchNodeByID(graph, startNodeId);
	PNode* endNode = searchNodeByID(graph, endNodeId);

	if (startNode && endNode) {
		insertSecondaryList(&(startNode->neighbour), endNode);
		insertSecondaryList(&(endNode->neighbour), startNode);
	}

}

void depthTraversal(PNode* graph, int startNodeId, int nodesNo) {

	StackNode* stack = NULL;
	int* visited = malloc(sizeof(int) * nodesNo);

	for (int i = 0; i < nodesNo; i++) {
		visited[i] = 0;
	}

	visited[startNodeId - 1] = 1;
	push(&stack, startNodeId);

	while (stack) {
		int currentId = pop(&stack);
		PNode* currentNode = searchNodeByID(graph, currentId);
		printCity(currentNode->city);
		SNode* temp = currentNode->neighbour;

		while (temp) {
			if (visited[temp->primaryNodeAddress->city.id - 1] == 0) {
				visited[temp->primaryNodeAddress->city.id - 1] = 1;
				push(&stack, temp->primaryNodeAddress->city.id);
			}
			temp = temp->next;
		}
	}
}

int main() {

	PNode* graph = NULL;

	int nodesNo = 0;
	printf("Node number:");
	scanf("%d", &nodesNo);
	for (int i = 0; i < nodesNo; i++) {
		City c;
		c.id = i + 1;
		char buffer[20];
		printf("City name: ");
		scanf("%s", &buffer);
		c.name = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
		strcpy(c.name, buffer);
		insertPrimaryList(&graph, c);
	}

	int edgesNo = 0;

	printf("Edges number:");
	scanf("%d", &edgesNo);
	for (int i = 0; i < edgesNo; i++) {
		int startId = 0;
		int stopId = 0;
		printf("Edge start from the city with ID:");
		scanf("%d", &startId);
		printf("Edge start from the city with ID:");
		scanf("%d", &stopId);
		addEdge(graph, startId, stopId);
	}

	printPrimaryList(graph);

	depthTraversal(graph, 1, nodesNo);

}