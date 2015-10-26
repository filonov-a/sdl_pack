#include "SDL_pack.h"

#include <zip.h>

//#ifdef HAVE_ZLIB_H

typedef struct ZipInfo{
    Sint64     size;
    struct zip*        zip;
} ZipInfo;

#define  SDL_RW_TAG_CURRENT SDL_RWOPS_RWZIP

static Sint64 SDLCALL sdl_zip_seekfunc(SDL_RWops *context, Sint64 offset, int whence) {
    return -1;
}

static Sint64 SDLCALL sdl_zip_sizefunc(SDL_RWops *context) {
    return ((ZipInfo*)context->hidden.unknown.data2)->size;
}


static size_t SDLCALL sdl_zip_readfunc(SDL_RWops *context, void *ptr, size_t size, size_t num) {
    if (context->type != SDL_RW_TAG_CURRENT) {
        return SDL_SetError("bad RWops type");
    }
    size_t realSize = size * num;
    size_t bytesReaded = zip_fread((zip_file_t*)context->hidden.unknown.data1,ptr, realSize);
    return bytesReaded / size;
}

static size_t SDLCALL sdl_zip_writefunc(SDL_RWops *context, const void *ptr, size_t size, size_t num) {
    return -1;
}

static int SDLCALL sdl_zip_closefunc(SDL_RWops *context) {
    if (context->type != SDL_RW_TAG_CURRENT) {
        return SDL_SetError("bad RWops type");
    }
    zip_fclose((zip_file_t*)context->hidden.unknown.data1);
    SDL_free(context->hidden.unknown.data2);
    SDL_FreeRW(context);
    return 0;
}

SDL_RWops *SDL_RWFromZip(struct zip *z,const char *fname) {
    zip_file_t* zf;
    struct zip_stat st;
    Sint64 idx = zip_name_locate(z, fname, 0);

    if ( idx < 0){
        SDL_SetError("Can't find file %s",fname);
        return NULL;
    }
    //printf("Found file %s with idx %ld\n",fname,idx);

    zf=zip_fopen_index(z,idx,ZIP_FL_UNCHANGED);
    if(zf == NULL ){
        zip_error_t *error = zip_get_error(z);
        SDL_SetError("PCK_RWFromZip failed for idx=%ld:%s", idx,zip_error_strerror(error));
        zip_error_fini(error);
        return NULL;
    }

    zip_stat_init(&st);
    if (zip_stat_index(z, idx, 0, &st) == 0) {
    }
    SDL_RWops *c = SDL_AllocRW();
    if (c == NULL) return NULL;

    c->seek = sdl_zip_seekfunc;
    c->size = sdl_zip_sizefunc;
    c->read = sdl_zip_readfunc;
    c->write = sdl_zip_writefunc;
    c->close = sdl_zip_closefunc;
    c->type = SDL_RW_TAG_CURRENT;
    ZipInfo* zi = SDL_malloc(sizeof(ZipInfo));
    zi->size = st.size;
    zi->zip = z;

    c->hidden.unknown.data1 = zf;
    c->hidden.unknown.data2 = zi;
    return c;
}

static SDL_RWops *zip_OpenRW(PCK_Catalog *cat, const char *s) {
    return SDL_RWFromZip((struct zip*)cat->context, s);
}

static void zip_Close(struct PCK_Catalog *cat) {
    zip_close((struct zip*)cat->context);
    SDL_free(cat);
}

PCK_Catalog *PCK_OpenZipFile(const char *zipName){
    int err=0;
    zip_error_t error;
    zip_error_init(&error);
    struct zip* z = zip_open(zipName,ZIP_RDONLY,&err);
    if(err != ZIP_ER_OK ){
        zip_error_init_with_code(&error, err);
        SDL_SetError("PCK_OpenZipFile failed:%d %s ",err, zip_error_strerror(&error));
        zip_error_fini(&error);
        return NULL;
    }
    PCK_Catalog* ptr=PCK_AllocCatalog();
    ptr->openRW = zip_OpenRW;
    ptr->close = zip_Close;
    ptr->context = (SDL_RWops*)z;
}




//#endif
