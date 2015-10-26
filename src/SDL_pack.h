//
// Created by aef on 10/22/15.
//

#ifndef SDL_PACK_H
#define SDL_PACK_H

#include <SDL_stdinc.h>
#include <SDL_rwops.h>

#define HAVE_ZLIB_H 1
#define SDL_RWOPS_RW2RW 0x52573252
#define SDL_RWOPS_RWZIP 0x52575a49

extern DECLSPEC SDL_RWops *SDLCALL SDL_RWFromRW(SDL_RWops *rw2, Sint64 off, Sint64 size);
extern DECLSPEC SDL_RWops *SDLCALL SDL_RWFromFileRange(const char *name, Sint64 off, Sint64 size);


typedef struct PCK_CatalogItem {
    Sint64 begin;
    Sint64 size;
    char *name;
    struct PCK_CatalogItem *next;
} PCK_CatalogItem;

typedef struct PCK_Catalog {
    SDL_RWops *context;       // Base handler
    PCK_CatalogItem *items;   // Pointer to Items. Can be NULL
    struct PCK_Catalog *next; // Pointer to next catalog in list
    // functions
    SDL_RWops* (SDLCALL * openRW) (struct PCK_Catalog *,const char *s); // Real Open function
    void (SDLCALL * close) (struct PCK_Catalog *); // Real Close function

} PCK_Catalog;

extern DECLSPEC PCK_CatalogItem *SDLCALL PCK_AllocCatalogItem(const char *name, Sint64 begin, Sint64 size);

extern DECLSPEC void SDLCALL PCK_FreeCatalogItem(PCK_CatalogItem *);

extern DECLSPEC PCK_CatalogItem *SDLCALL PCK_FindItem(PCK_CatalogItem *, const char *s);


extern DECLSPEC PCK_Catalog *SDLCALL PCK_AllocCatalog();

extern DECLSPEC PCK_Catalog *SDLCALL PCK_OpenCatForRW(const char *catName, SDL_RWops *context);

extern DECLSPEC PCK_Catalog *SDLCALL PCK_OpenCatForFile(const char *catName, const char *packname);

extern DECLSPEC void SDLCALL PCK_CloseCatalog(PCK_Catalog *);

extern DECLSPEC SDL_RWops *SDLCALL PCK_OpenRW(PCK_Catalog *, const char *s);

#define IMG_Load_PCK(a, b) IMG_Load_RW(PCK_OpenRW((a),(b)),1)
#define SDL_LoadBMP_PCK(a, b) SDL_LoadBMP_RW(PCK_OpenRW((a),(b)),1)

extern DECLSPEC PCK_Catalog *SDLCALL PCK_OpenZipFile(const char *fname);

#endif
