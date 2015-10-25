//
// Created by aef on 10/22/15.
//

#include "SDL_pack.h"


PCK_CatalogItem *PCK_AllocCatalogItem(const char *name, Sint64 begin, Sint64 size) {
    PCK_CatalogItem *ptr = (PCK_CatalogItem *) SDL_malloc(sizeof(PCK_CatalogItem));
    if (ptr) {
        ptr->next = NULL;
        ptr->name = SDL_strdup(name);
        ptr->begin = begin;
        ptr->size = size;
    }
    return ptr;
}


void PCK_FreeCatalogItem(PCK_CatalogItem *item) {
    PCK_CatalogItem *next;
    while (item != NULL) {
        next = item->next;
        SDL_free(item->name);
        SDL_free(item);
        item = next;
    }
}

PCK_CatalogItem *PCK_FindItem(PCK_CatalogItem *item, const char *s) {
    while (item != NULL) {
        if (SDL_strcmp(s, item->name) == 0) {
            return item;
        }
        item = item->next;
    }
    return NULL;
}

PCK_Catalog *PCK_AllocCatalog() {
    PCK_Catalog *ptr = (PCK_Catalog *) SDL_malloc(sizeof(PCK_Catalog));
    if (NULL == ptr) {
        return NULL;
    }
    ptr->context = NULL;
    ptr->items = NULL;
    ptr->next = NULL;
    return ptr;
}

PCK_Catalog *PCK_OpenCatForRW(const char *catName, SDL_RWops *context) {
    Sint64 first;
    Sint64 size;
    Sint64 current = 0;
    PCK_CatalogItem *itemPtr;
    PCK_CatalogItem **currentPtr;

    int n;

    PCK_Catalog *ptr = PCK_AllocCatalog();
    if (NULL == ptr) {
        return NULL;
    }
    currentPtr = &ptr->items;
    ptr->context = context;

#ifndef HAVE_STDIO_H
#error  Catalogs without STDIO not implemented yet
#endif
#define MAX_LINE_SIZE 1024
    FILE *f = fopen(catName, "rb");
    if (f == NULL) {
        PCK_FreeCatalog(ptr);
        return NULL;
    }
    char buffer[MAX_LINE_SIZE];
    char name[MAX_LINE_SIZE];
    char *s;

    while (!feof(f)) {
        s = fgets(buffer, sizeof(buffer), f);
        if (s != NULL) {
            n = SDL_sscanf(buffer, "%ld %ld %s\n", &first, &size, &name);
            if (3 == n) {
                current = (0 == first) ? current : first;
                itemPtr = PCK_AllocCatalogItem(name, current, size);
                current += size;
                *currentPtr = itemPtr;
                currentPtr = &itemPtr->next;
            } else {
                n = SDL_sscanf(buffer, "%ld %s\n", &size, &name);
                if (2 == n) {
                    itemPtr = PCK_AllocCatalogItem(name, current, size);
                    current += size;
                    *currentPtr = itemPtr;
                    currentPtr = &itemPtr->next;
                }
            }
        }
    }
    return ptr;
}

void PCK_FreeCatalog(PCK_Catalog *cat) {
    PCK_Catalog *next;
    while (cat != NULL) {
        next = cat->next;
        if (cat->context != NULL) {
            SDL_RWclose(cat->context);
        }
        PCK_FreeCatalogItem(cat->items);
        SDL_free(cat);
        cat = next;
    }
}

SDL_RWops *PCK_OpenRW(PCK_Catalog *cat, const char *s) {
    PCK_CatalogItem *item = PCK_FindItem(cat->items, s);
    if (item == NULL || cat->context == NULL) {
        return NULL;
    }
    return SDL_RWFromRW(cat->context, item->begin, item->size);
}

PCK_Catalog *PCK_OpenCatForFile(const char *catName, const char *packname) {
    SDL_RWops *rw = SDL_RWFromFile(packname, "rb");
    if (rw == NULL) {
        return NULL;
    }
    return PCK_OpenCatForRW(catName, rw);
}
