#include <SDL.h>
#include "SDL_pack.h"

#include "debug.h"

#define BUFSIZE 64

int main(int argc, char **argv) {
    int i;
    size_t size;
    char srcbuff[BUFSIZE];
    char dstbuff[BUFSIZE];

    for (i = 0; i < sizeof(srcbuff); i++) {
        srcbuff[i] = i + 64;
    }

    hexDump("Starting data", srcbuff, sizeof(srcbuff));
    SDL_RWops *rw = SDL_RWFromMem(srcbuff, sizeof(srcbuff));

    // we create virtual RWops from RWops starting from 16, length 8
    SDL_RWops *rw2 = SDL_RWFromRW(rw, 16, 8);
    size = SDL_RWread(rw2, dstbuff, 1, sizeof(dstbuff));
    hexDump("Try to read 64 bytes but read only", dstbuff, size);

    size = SDL_RWread(rw2, dstbuff, 1, sizeof(dstbuff));
    hexDump("Second Read - we reach EOF of RW2", dstbuff, size);

    // read last 3 bytes
    SDL_RWseek(rw2, -3, RW_SEEK_END);
    size = SDL_RWread(rw2, dstbuff, 1, sizeof(dstbuff));
    hexDump("Seek and read again", dstbuff, size);

    // seek to start and write to rw2 bytes 1..6 from srcbuff
    SDL_RWseek(rw2, 0, RW_SEEK_SET);
    SDL_RWwrite(rw2, srcbuff, 1, 5);
    hexDump("SRC After writing 5 bytes", srcbuff, sizeof(srcbuff));

    SDL_RWwrite(rw2, srcbuff, 1, 10);
    hexDump("SRC After writing another 10 bytes", srcbuff, sizeof(srcbuff));

    debug("Try to open catalog\n");
    PCK_Catalog *cat = PCK_OpenCatForRW("data.cat", NULL);
    SDL_RWclose(rw2);
    SDL_RWclose(rw);
    debug("catalog opened %p\n", cat);
    PCK_CatalogItem *catItem = cat->items;
    while (catItem != NULL) {
        debug("catItem(%p->%p) begin=%ld size=%ld n=%s\n",
              catItem,
              catItem->next,
              catItem->begin,
              catItem->size,
              catItem->name);
        catItem = catItem->next;
    }
    PCK_FreeCatalog(cat);
    return 0;
}
