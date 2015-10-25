#include <SDL_version.h>

// Put right version number here (Not yet ready :-)
#if SDL_VERSION_ATLEAST(3, 0, 0)

#include <SDL_rwops.h>

# else
//-------------- BlackMagicMode on ---------------------
// Extremely Dirty Hack - Don't try to reproduce

#define unknown  unknown; struct \
{ \
    Sint64 begin;\
    Sint64 current;\
    Sint64 end;\
    void *baseRW;\
    } rwops

#include <SDL_rwops.h>

// Another Hack - we want correct code with AllocRW but with different size
#define  SDL_AllocRW() SDL_malloc(sizeof(SDL_RWops))

// -------------- BlackMagicMode off --------------------
#endif

