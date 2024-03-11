#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Song {
    int trackId;
    char* title;
    char* artist;
    float royalty;
};

void printSong(struct Song currentSong) {
    printf("==========================\n");
    printf("trackId: %d\ntitle: %sartist: %sroyalty: %.3f", currentSong.trackId, currentSong.title, currentSong.artist, currentSong.royalty);
    printf("\n==========================\n");
}

struct Song extractSong(FILE* currentFile) {
    struct Song dummySong;

    char buffer[200];

    //trackId
    fgets(buffer, 200, currentFile);
    dummySong.trackId = atoi(buffer);
    //title
    fgets(buffer, 200, currentFile);
    dummySong.title = (char*)malloc(strlen(buffer) + 1);
    strcpy(dummySong.title, buffer);
    //artist
    fgets(buffer, 200, currentFile);
    dummySong.artist = (char*)malloc(strlen(buffer) + 1);
    strcpy(dummySong.artist, buffer);
    //royalty
    fgets(buffer, 200, currentFile);
    dummySong.royalty = atof(buffer);

    return dummySong;

}

void readSongsFromFile(char* fileName, struct Song** songArr, int* numberOfSongs) {
    FILE* currentFile = NULL;
    currentFile = fopen(fileName, "r");

    if(currentFile!=NULL) {
        char buffer[200];
        fgets(buffer, 200, currentFile);

        *numberOfSongs = atoi(buffer);

        *songArr = (struct Song*)malloc(sizeof(struct Song) * (*numberOfSongs));
        for(int i=0; i<*numberOfSongs; i++) {
            (*songArr)[i] = extractSong(currentFile);
        }
    }
    else {
        printf("ERR_OPENING_FILE");
    }
    
    fclose(currentFile);
}

int main() {
    struct Song* songArr;
    int numberOfSongs;

    readSongsFromFile("songs.txt", &songArr, &numberOfSongs);

    for(int i=0; i<numberOfSongs; i++)
        printSong(songArr[i]);

    return 0;
}


