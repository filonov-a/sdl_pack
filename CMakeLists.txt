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
)

include_directories(
        ${CMAKE_SOURCE_DIR}/src
)
add_library(SDL_pack ${PCK_SOURCES} ${PCK_HEADERS})
target_link_libraries(SDL_pack ${SDL2_LIBRARIES})

install(TARGETS SDL_pack EXPORT SDL_pack_targets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

install(FILES ${PCK_HEADERS} DESTINATION include/SDL2)


add_subdirectory(examples)
