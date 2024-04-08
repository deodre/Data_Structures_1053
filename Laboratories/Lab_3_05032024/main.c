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

struct Song initSong(int trackId, char* title, char* artist, float royaltyPerPlay) {
	
	struct Song song;
	
	song.trackId = trackId;
	
	song.title = malloc((strlen(title) + 1) * sizeof(char));
	strcpy_s(song.title, strlen(title) + 1, title);

	song.artist = malloc((strlen(artist) + 1) * sizeof(char));
	strcpy_s(song.artist, strlen(artist) + 1, artist);

	song.royaltyPerPlay = royaltyPerPlay;

	return song;
}

void printSongToConsole(struct Song song) {
	printf("--------\nSong ID: %d\nSong title: %s\nSong artist: %s\nRoyalty per play: %.3f\n--------\n", song.trackId, song.title, song.artist, song.royaltyPerPlay);
}

struct Song readSongFromFile(FILE* f) {
	
	struct Song song;

	char buffer[LINESIZE];
	char* endptr = NULL; // for strtol and strtof, store the rest of the string
	char* nextToken = NULL; // for strtok, stores the rest of the string that hasn't been yet tokenized
	char* songData = NULL; // saves the token at each call of strtok

	fgets(buffer, LINESIZE, f);

	songData = strtok_s(buffer, ",", &nextToken);
	song.trackId = (int)strtol(songData, &endptr, 10);

	songData = strtok_s(NULL, ",", &nextToken);
	song.title = malloc((strlen(songData) + 1) * sizeof(char));
	strcpy_s(song.title, strlen(songData) + 1, songData);

	songData = strtok_s(NULL, ",", &nextToken);
	song.artist = malloc((strlen(songData) + 1) * sizeof(char));
	strcpy_s(song.artist, strlen(songData) + 1, songData);

	songData = strtok_s(NULL, ",", &nextToken);
	song.royaltyPerPlay = strtof(songData, &endptr);

	return song;
}

struct Song* readSongsFromFile(const char* fileName, int* songsNo) {

	FILE* f;
	fopen_s(&f, fileName, "r");

	char buffer[LINESIZE];
	fgets(buffer, LINESIZE, f);

	char* endptr = NULL;
	*songsNo = (int)strtol(buffer, &endptr, 10);
	
	struct Song* songs = malloc(*songsNo * sizeof(struct Song));

	for (int i = 0; i < (*songsNo); i++) {
		songs[i] = readSongFromFile(f);
	}

	fclose(f);


	return songs;
}

int main() {

	struct Song song1 = initSong(0, "One", "Metallica", 0.586);
	printSongToConsole(song1);

	int songsNo;
	struct Song* songs = readSongsFromFile("./songs.txt", &songsNo);

	for (int i = 0; i < songsNo; i++) {
		printSongToConsole(songs[i]);
	}

	FILE* f;
	fopen_s(&f, "./songs.txt", "r");

	int songsNumber;
	fscanf_s(f, "%d", &songsNumber);

	printf("Songs Number: %d", songsNumber);


	return 0;
}