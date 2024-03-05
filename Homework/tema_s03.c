#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct Song Song;

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

// prima varianta, care introduce cate un rand gol
// sub title si artist
/*Song readSongFromFile(FILE* f)
{
    Song song;
    if(f != NULL) 
    {
        char buffer[20];
        fgets(buffer, 15, f);
        char* token = strtok(buffer, "\n");

        //fgets(buffer, 10, f);
        song.trackId = atoi(buffer);

        fgets(buffer, 20, f);
        song.title = (char*)malloc(strlen(token) + 1);
        strcpy(song.title, token);

        fgets(buffer, 20, f);
        song.artist = (char*)malloc(strlen(token) + 1);
        strcpy(song.artist, token);

        fgets(buffer, 10, f);
        song.royalityPerPlay = atof(token);
    }
    return song;
}*/

Song readSongFromFile(FILE* f)
{
    Song song;
    if(f != NULL) 
    {
        char buffer[100];

        fgets(buffer, 100, f);
        song.trackId = atoi(buffer);

        fgets(buffer, 100, f);
        // Inlocuieste \n cu \0
        buffer[strlen(buffer) - 1] = '\0'; 
        song.title = (char*)malloc(strlen(buffer) + 1);
        strcpy(song.title, buffer);

        fgets(buffer, 100, f);
        buffer[strlen(buffer) - 1] = '\0'; 
        song.artist = (char*)malloc(strlen(buffer) + 1);
        strcpy(song.artist, buffer);

        fgets(buffer, 100, f);
        song.royalityPerPlay = atof(buffer);
    }
    return song;
}


void readArrayOfSongs(char* numeFis, Song** songs, int* dim)
{
    FILE* f = NULL;
    f = fopen(numeFis, "r");

    if(f != NULL)
    {
        char buffer[100];
        fgets(buffer, 100, f);
        *dim = atoi(buffer);

        *songs = (Song*)malloc(sizeof(Song) * (*dim));
        for(int i = 0; i < *dim; i++)
            (*songs)[i] = readSongFromFile(f);
        fclose(f);
    }
    else {
        printf("Error!");
    }
}

int main()
{
    Song* songs;
    int dim;

    readArrayOfSongs("songs.txt", &songs, &dim);
    for(int i = 0; i < dim; i++)
        printSong(songs[i]);
}