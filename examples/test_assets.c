#include <SDL.h>
#include "SDL_pack.h"
#include "SDL_assert.h"
#include "debug.h"


// tools/makePack.sh assets data/data before start

int main(int argc, char **argv) {
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *bitmapTex = NULL;
    SDL_Texture *testTex = NULL;
    SDL_Surface *helloSurface = NULL;
    SDL_Surface *testSurface = NULL;
    SDL_Rect rect = {0, 0, 40, 40};
    int posX = 100, posY = 100, width = 640, height = 480;
    Uint32 ticks;

    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Hello World", posX, posY, width, height, 0);

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);


    PCK_Catalog *cat = PCK_OpenCatForFile("data.cat", "data.pck");
    SDL_assert(("Cannot open catfile", cat != NULL));

    helloSurface = SDL_LoadBMP_PCK(cat, "assets/hello.bmp");
    if (helloSurface == NULL) {
        debug("LoadBMP error:%s\n", SDL_GetError());
        exit(1);
    }
    testSurface = SDL_LoadBMP_PCK(cat, "assets/test.bmp");
    if (testSurface == NULL) {
        debug("LoadBMP error:%s\n", SDL_GetError());
        exit(1);
    }
    bitmapTex = SDL_CreateTextureFromSurface(renderer, helloSurface);
    testTex = SDL_CreateTextureFromSurface(renderer, testSurface);
    SDL_FreeSurface(helloSurface);
    ticks = 0;
    while (1) {

        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT || e.type == SDL_KEYUP) {
                break;
            }
        }
        if (SDL_GetTicks() - ticks > 1000 || ticks == 0) {
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, bitmapTex, NULL, NULL);
            // move testTex to random position
            rect.x = rand() % (width - rect.w);
            rect.y = rand() % (height - rect.h);
            SDL_RenderCopy(renderer, testTex, NULL, &rect);
            SDL_RenderPresent(renderer);
            ticks = SDL_GetTicks();
        } else {
            SDL_Delay(5);
        }
    }

    SDL_DestroyTexture(bitmapTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();
    PCK_CloseCatalog(cat);
    return 0;
}
