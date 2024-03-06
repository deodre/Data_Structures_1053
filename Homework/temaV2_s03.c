#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Song Song;

#define LINESIZE 128

struct Song 
{
    int trackId;
    char* title;
    char* artist;
    float royalityPerPlay;
};

Song initSong(int trackId, char* title, char* artist, float royalityPerPlay)
{
    Song song;

    song.trackId = trackId;
    song.title = (char*)malloc(strlen(title) + 1);
    strcpy(song.title, title);
    song.artist = (char*)malloc(strlen(artist) + 1);
    strcpy(song.artist, artist);
    song.royalityPerPlay = royalityPerPlay;

    return song;
}

void printSong(Song song) 
{
    printf("---------\nSong ID: %d\nSong title: %s\nSong artist: %s\nRoyality per play: %.3f\n---------\n",
    song.trackId, song.title, song.artist, song.royalityPerPlay);
}

Song readSongFromFile(FILE* f)
{
    Song song;

    fscanf(f, "%d\n", &song.trackId);

    song.title = malloc(LINESIZE * sizeof(char));
    song.artist = malloc(LINESIZE * sizeof(char));

    fscanf(f, "%[^\n]\n", song.title); // Citire până la newline

    fscanf(f, "%[^\n]\n", song.artist); 

    fscanf(f, "%f\n", &song.royalityPerPlay);

    return song;
}

Song* readSongsFromFile(char* fileName, int* songsNo)
{
    FILE* f = fopen(fileName, "r");

    fscanf(f, "%d\n", songsNo);

    Song* songs = (Song*)malloc(*songsNo * sizeof(Song));

    for (int i = 0; i < *songsNo; i++) {
        songs[i] = readSongFromFile(f);
    }

    fclose(f);
    return songs;
}

int main()
{
    int songsNo;
    Song* songs = readSongsFromFile("songs.txt", &songsNo);

    for (int i = 0; i < songsNo; i++) {
        printSong(songs[i]);
    }

    for (int i = 0; i < songsNo; i++) {
        free(songs[i].title);
        free(songs[i].artist);
    }
    free(songs);

    return 0;
}
