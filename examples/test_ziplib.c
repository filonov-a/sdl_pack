#include <SDL.h>
#include "SDL_pack.h"

#include <zip.h>

#include <stdio.h>
#include "debug.h"

#define BUFSIZE 64

int main(int argc, char **argv) {
    int err;
    int i;
    size_t size;
    char buffer[1024];
    char* filename = argv[1];

    if(argc == 1) {
        printf("no file given\n");
        exit(1);
    }
    PCK_Catalog *cat = PCK_OpenZipFile(filename);
    if(cat == NULL){
        printf("Catalog open Error: %s",SDL_GetError());
        return 0;
    }
    SDL_RWops* rw =  PCK_OpenRW(cat,"data.lst");
    if(rw == NULL){
        printf("Rw open failed: %s",SDL_GetError());
        return 0;
    }

    size = SDL_RWread(rw,buffer,1,sizeof(buffer));
    hexDump("Readed",buffer,size);
    SDL_RWclose(rw);
    PCK_CloseCatalog(cat);
}
