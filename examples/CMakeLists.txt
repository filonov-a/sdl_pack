find_package(PkgConfig)

link_directories(${SDL2_LIBRARY_DIRS})
include_directories(${SDL2_INCLUDE_DIRS})

set(SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/scripts)

aux_source_directory(assets ASSETS_LIST)

add_custom_target(data.cat
        COMMAND "${SCRIPTS_DIR}/makeCat.sh" "${CMAKE_CURRENT_SOURCE_DIR}/data.lst" "${CMAKE_CURRENT_BINARY_DIR}/data"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        SOURCES data.lst
        DEPENDS ${ASSETS_LIST})

add_executable(test_rw2rw test_rw2rw.c debug.h debug.c)
add_executable(test_assets test_assets.c debug.h debug.c)
add_dependencies(test_assets data.cat)

target_link_libraries(test_rw2rw LINK_PUBLIC SDL_pack ${SDL2_LIBRARIES})
target_link_libraries(test_assets LINK_PUBLIC SDL_pack ${SDL2_LIBRARIES})
