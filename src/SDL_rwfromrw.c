#include <SDL_stdinc.h>
#include <SDL_error.h>

#define SDL_RWOPS_RW2RW 0x52573252

#define  SDL_RW_TAG_CURRENT SDL_RWOPS_RW2RW


#include "SDL_rwops_hack.h"


static Sint64 SDLCALL rw_seekfunc(SDL_RWops *context, Sint64 offset, int whence) {
    Sint64 newpos;

    if (context->type != SDL_RW_TAG_CURRENT) {
        return SDL_SetError("bad RWops type");
    }

    switch (whence) {
        case RW_SEEK_SET:
            newpos = context->hidden.rwops.begin + offset;
            break;
        case RW_SEEK_CUR:
            newpos = context->hidden.rwops.current + offset;
            break;
        case RW_SEEK_END:
            newpos = context->hidden.rwops.end + offset;
            break;
        default:
            return SDL_SetError("Unknown value for seek 'whence'");
    }
    context->hidden.rwops.current = SDL_max(
            SDL_min(newpos, context->hidden.rwops.end),
            context->hidden.rwops.begin
    );
    return newpos - context->hidden.rwops.begin;
}

static Sint64 SDLCALL rw_sizefunc(SDL_RWops *context) {
    if (context->type != SDL_RW_TAG_CURRENT) {
        return SDL_SetError("bad RWops type");
    }
    return context->hidden.rwops.end - context->hidden.rwops.begin;
}


static size_t SDLCALL rw_readfunc(SDL_RWops *context, void *ptr, size_t size, size_t num) {

    if (context->type != SDL_RW_TAG_CURRENT) {
        return SDL_SetError("bad RWops type");
    }
    Sint64 pos = SDL_RWseek((SDL_RWops *) context->hidden.rwops.baseRW, context->hidden.rwops.current, RW_SEEK_SET);
    if (pos == -1) {
        return SDL_SetError("Can't seek");
    }
    size_t realSize = SDL_min(size * num, context->hidden.rwops.end - pos);
    size_t bytesReaded = SDL_RWread((SDL_RWops *) context->hidden.rwops.baseRW, ptr, 1, realSize);
    context->hidden.rwops.current += bytesReaded;
    return bytesReaded / size;
}

static size_t SDLCALL rw_writefunc(SDL_RWops *context, const void *ptr, size_t size, size_t num) {
    if (context->type != SDL_RW_TAG_CURRENT) {
        return SDL_SetError("bad RWops type");
    }
    Sint64 pos = SDL_RWseek((SDL_RWops *) context->hidden.rwops.baseRW, context->hidden.rwops.current, RW_SEEK_SET);
    if (pos == -1) {
        return SDL_SetError("Can't seek");
    }
    size_t realSize = SDL_min(size * num, context->hidden.rwops.end - pos);
    size_t bytesWritten = SDL_RWwrite((SDL_RWops *) context->hidden.rwops.baseRW, ptr, 1, realSize);
    context->hidden.rwops.current += bytesWritten;
    return bytesWritten / size;
}

static int SDLCALL rw_closefunc(SDL_RWops *context) {
    if (context->type != SDL_RW_TAG_CURRENT) {
        return SDL_SetError("bad RWops type");
    }
    SDL_FreeRW(context);
    return 0;
}

SDL_RWops *SDL_RWFromRW(SDL_RWops *src, Sint64 off, Sint64 size) {
    SDL_RWops *c = SDL_AllocRW();
    if (c == NULL) return NULL;

    c->seek = rw_seekfunc;
    c->size = rw_sizefunc;
    c->read = rw_readfunc;
    c->write = rw_writefunc;
    c->close = rw_closefunc;
    c->type = SDL_RW_TAG_CURRENT;
    c->hidden.rwops.baseRW = src;
    c->hidden.rwops.current = off;
    c->hidden.rwops.begin = off;
    c->hidden.rwops.end = off + size;
    return c;
}

SDL_RWops *SDL_RWFromFileRange(const char *name, Sint64 off, Sint64 size) {
    SDL_RWops *rw = SDL_RWFromFile(name, "rb");
    if (rw == NULL) {
        return NULL;
    }
    return SDL_RWFromRW(rw, off, size);
}
