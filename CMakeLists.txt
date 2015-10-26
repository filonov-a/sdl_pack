cmake_minimum_required(VERSION 2.8)
project(sdl_pack)

find_package(PkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)

link_directories(${SDL2_LIBRARY_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})


set(PCK_HEADERS
        src/SDL_pack.h
)

set(PCK_SOURCES
    src/SDL_pack.c
    src/SDL_rwfromrw.c
    src/SDL_rwops_hack.h
    src/SDL_zip.c
)


set(ZIP_SOURCES
    src/SDL_zip.c
)

#set(ARC_SOURCES
#    src/SDL_arc.c
#)
include_directories(
        ${CMAKE_SOURCE_DIR}/src
)
add_library(SDL_pack ${PCK_SOURCES} ${PCK_HEADERS})
add_library(SDL_pack_zip ${ZIP_SOURCES} ${PCK_HEADERS})
#add_library(SDL_pack_arc ${ARC_SOURCES} ${PCK_HEADERS})

target_link_libraries(SDL_pack ${SDL2_LIBRARIES})

install(TARGETS SDL_pack SDL_pack_zip EXPORT SDL_pack_targets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(FILES ${PCK_HEADERS} DESTINATION include/SDL2)


add_subdirectory(examples)
