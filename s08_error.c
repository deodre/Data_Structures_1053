#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#define LINESIZE 128


typedef struct Task Task;
typedef struct Heap Heap;


struct Task
{
	char* description;
	int priority;
};

struct Heap
{
	Task* tasks;
	int size;
};

void swapTask(Task* t1, Task* t2)
{
	Task aux = *t1;
	*t1 = *t2;
	*t2 = aux;
}

Task readTaskFromFile(FILE* f) {

	Task task;

	char buffer[LINESIZE];

	fgets(buffer, LINESIZE, f);
	buffer[strcspn(buffer, "\n")] = '\0';
	task.description = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(task.description, buffer);

	fgets(buffer, LINESIZE, f);
	task.priority = (int)strtol(buffer, NULL,10);

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


Heap initHeap()
{
	Heap heap;
	heap.size = 0;
	heap.tasks = (Task*)malloc(heap.size * sizeof(Task));
	return heap;
}

void insertHeap(Heap* heap, Task task)
{
	heap->size += 1;
	heap->tasks = (Task*)realloc(heap->tasks, heap->size * sizeof(Task));

	int offset_element = heap->size - 1;

	heap->tasks[offset_element] = task;

	int parent_offset = (offset_element - 1) / 2;

	while (heap->tasks[offset_element].priority > heap->tasks[parent_offset].priority)
	{//max-heap condition not me
		swapTask(&heap->tasks[offset_element], &heap->tasks[parent_offset]);

		offset_element = parent_offset;

		parent_offset = (offset_element - 1) / 2;
	}
}

void printTaskToConsole(Task task) {
	printf("%s - priority - %d\n", task.description, task.priority);
}

void printHeap(Heap heap)
{
	for (int i = 0; i < heap.size; i++)
	{
		printf("%d ", heap.tasks[i].priority);
	}

	printf("\n----------------------------");
}


Task initTask(char* description, float priority) {
	Task task = {.priority = priority};
	task.description = malloc((strlen(description) + 1) * sizeof(char));
	strcpy(task.description, description);
	return task;
}

void heapify(Heap heap, int index)
{
	int largest = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	if (left < heap.size && heap.tasks[left].priority > heap.tasks[largest].priority)
	{
		largest = left;
	}

	if (left < heap.size && heap.tasks[right].priority > heap.tasks[largest].priority)
	{
		largest = right;
	}

	if (largest != index)
	{
		swapTask(&heap.tasks[largest], &heap.tasks[index]);

		heapify(heap, largest);
	}
}


Heap buildHeap(Task* tasks, int size)
{
	Heap heap;
	heap.size = size;
	heap.tasks = (Task*)malloc(size * sizeof(Task));
	for (int i = 0; i < size; i++)
	{
		heap.tasks[i] = initTask(tasks[i].description, tasks[i].priority);
	}

	int startIndex = heap.size / 2 - 1;

	for (int i = startIndex; i >= 0; i--)
	{
		heapify(heap, i);
	}

	return heap;
}

Task deleteFromHeap(Heap* heap)
{
	Task task = heap->tasks[0];

	swapTask(&heap->tasks[0], &heap->tasks[heap->size - 1]);

	heap->size -= 1;

	heap->tasks = (Task*)realloc(heap->tasks, heap->size * sizeof(Task));

	int startIndex = heap->size / 2 - 1;

	for (int i = startIndex; i >= 0; i--)
	{
		heapify(*heap, i);
	}
	return task;
}

int main()
{
	int tasksNumber;
	Task* tasks;
	
	readTasksFromFile("scheduler.txt", &tasks, &tasksNumber);

	for (int i = 0; i < tasksNumber; i++)
		printTaskToConsole(tasks[i]);
	
	Heap heap = initHeap();

	printf("\n--------------------------------------\n");

	insertHeap(&heap, tasks[0]);
	insertHeap(&heap, tasks[1]);
	insertHeap(&heap, tasks[2]);
	insertHeap(&heap, tasks[3]);
	insertHeap(&heap, tasks[4]);
	insertHeap(&heap, tasks[5]);
	insertHeap(&heap, tasks[6]);
	insertHeap(&heap, tasks[7]);
	insertHeap(&heap, initTask("Task 8", 27));

	printHeap(heap);


	printf("\n-------------------------\n");
	for (int i = 0; i < tasksNumber; i++)
		printf("%d ", tasks[i].priority);


	Heap heap2 = buildHeap(tasks, tasksNumber);

	printHeap(heap2);

	Task deletedTask = deleteFromHeap(&heap2);
	printTaskToConsole(deletedTask);
	free(deletedTask.description);
	printHeap(heap2);


	return 0;
}
