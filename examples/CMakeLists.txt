find_package(PkgConfig)

link_directories(${SDL2_LIBRARY_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})

set(SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/scripts)

set(DEBUG_SOURCES
    debug.h debug.c
)
add_executable(test_rw2rw test_rw2rw.c ${DEBUG_SOURCES})
target_link_libraries(test_rw2rw LINK_PUBLIC SDL_pack ${SDL2_LIBRARIES})

# --- add all files in directory to depends for cat/pck
aux_source_directory(assets ASSETS_LIST)

# --- create custom target for make cat/pck
add_custom_target(data.cat
        COMMAND "${SCRIPTS_DIR}/makeCat.sh" "${CMAKE_CURRENT_SOURCE_DIR}/data.lst" "${CMAKE_CURRENT_BINARY_DIR}/data"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        SOURCES data.lst
        DEPENDS ${ASSETS_LIST})


add_executable(test_assets test_assets.c debug.h debug.c)
target_link_libraries(test_assets LINK_PUBLIC SDL_pack ${SDL2_LIBRARIES})

add_dependencies(test_assets data.cat)

add_custom_target(data.zip
        COMMAND zip -0 -r  "${CMAKE_CURRENT_BINARY_DIR}/data" data.lst
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        SOURCES data.lst
        DEPENDS ${ASSETS_LIST})

add_executable(test_ziplib test_ziplib.c  ${DEBUG_SOURCES})
target_link_libraries(test_ziplib LINK_PUBLIC zip SDL_pack SDL_pack_zip zip ${SDL2_LIBRARIES})
add_dependencies(test_ziplib data.zip)

