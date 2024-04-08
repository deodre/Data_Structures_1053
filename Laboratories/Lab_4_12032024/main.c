#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#define LINESIZE 128

struct Song {
	int trackId;
	char* title;
	char* artist;
	float royaltyPerPlay;
};

struct Node {
	struct Song song;
	struct Node* next;
};

void printSongToConsole(struct Song song) {
	printf("--------\nSong ID: %d\nSong title: %sSong artist: %sRoyalty per play: %.2f\n--------\n", song.trackId, song.title, song.artist, song.royaltyPerPlay);
}

struct Song readSongFromFile(FILE* f) {

	char buffer[LINESIZE];

	char* endptr = NULL;

	struct Song song;

	fgets(buffer, LINESIZE, f);
	song.trackId = (int)strtol(buffer, &endptr, 10);

	fgets(buffer, LINESIZE, f);
	song.title = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy_s(song.title, strlen(buffer) + 1, buffer);

	fgets(buffer, LINESIZE, f);
	song.artist = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy_s(song.artist, strlen(buffer) + 1, buffer);

	fgets(buffer, LINESIZE, f);
	song.royaltyPerPlay = strtof(buffer, &endptr);

	return song;
}

void readSongsFromFile(const char* filename, struct Song** songsArray, int* songsNumber) {

	FILE* f;
	fopen_s(&f, filename, "r");

	char buffer[LINESIZE];
	char* endptr = NULL;
	
	fgets(buffer, LINESIZE, f);
	*songsNumber = (int)strtol(buffer, &endptr, 10);

	*songsArray = malloc((*songsNumber) * sizeof(struct Song));

	for (int i = 0; i < *songsNumber; i++) {
		(*songsArray)[i] = readSongFromFile(f);
	}

	fclose(f);
}

void insertAtBeginning(struct Node** head, struct Song song) {

	struct Node* newNode = malloc(sizeof(struct Node));

	newNode->song = song;
	newNode->next = *head;

	*head = newNode;

}

void insertAtEnd(struct Node** head, struct Song song) {
	if (*head) {
		struct Node* aux = *head;
		while (aux->next) {
			aux = aux->next;
		}

		struct Node* newNode = malloc(sizeof(struct Node));
		newNode->song = song;
		newNode->next = NULL;

		aux->next = newNode;
	}
	else {
		struct Node* newNode = malloc(sizeof(struct Node));
		newNode->song = song;
		newNode->next = *head;
		*head = newNode;
	}
}

void parseListAndPrint(struct Node* head) {
	if (head) {
		while (head) {
			printSongToConsole(head->song);
			head = head->next;
		}
	}
	else {
		printf("List is empty\n");
	}
}

int main() {

	int songsNumber;
	struct Song* songsArray;

	readSongsFromFile("playlist.txt", &songsArray, &songsNumber);

	/*for (int i = 0; i < songsNumber; i++) {
		printSongToConsole(songsArray[i]);
	}*/

	struct Node* head = NULL;

	parseListAndPrint(head);
	

	printf("\n\nList after one insert:\n");
	insertAtBeginning(&head, songsArray[0]);
	parseListAndPrint(head);
	printf("------------------------\n\n");

	printf("\n\nList after the second insert:\n");
	insertAtBeginning(&head, songsArray[1]);
	parseListAndPrint(head);
	printf("------------------------\n\n");

	printf("\n\nList after the third insert:\n");
	insertAtBeginning(&head, songsArray[2]);
	parseListAndPrint(head);
	printf("------------------------\n\n");

	struct Node* head2 = NULL;

	for (int i = 0; i < songsNumber; i++) {
		insertAtBeginning(&head2, songsArray[i]);
	}

	printf("\n\nList after inserting all songs:\n");
	parseListAndPrint(head2);


	printf("\n\nINSERTING AT THE END:\n");

	struct Node* head3 = NULL;

	parseListAndPrint(head3);


	printf("\n\nList after one insert:\n");
	insertAtEnd(&head3, songsArray[0]);
	parseListAndPrint(head3);
	printf("------------------------\n\n");

	printf("\n\nList after the second insert:\n");
	insertAtEnd(&head3, songsArray[1]);
	parseListAndPrint(head3);
	printf("------------------------\n\n");

	printf("\n\nList after the third insert:\n");
	insertAtEnd(&head3, songsArray[2]);
	parseListAndPrint(head3);
	printf("------------------------\n\n");

	return 0;
}
