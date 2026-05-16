cmake_minimum_required(VERSION 3.8)
project(EnTT_Pacman)

file(GLOB SOURCES 
    src/*/*.hpp
    src/*/*.cpp
    src/main.cpp
)

add_executable(pacman ${SOURCES})

if(CMAKE_TOOLCHAIN_FILE)
    message("Using SDL2 toolchain file")
    message(${CMAKE_TOOLCHAIN_FILE})
    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(pacman
        PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
    )
else()
    message("Finding SDL2")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
    find_package(SDL2 REQUIRED)

    target_include_directories(pacman
        PRIVATE
        "${SDL2_INCLUDE_DIR}"
    )
    target_link_libraries(pacman
        PRIVATE
        "${SDL2_LIBRARY}"
    )
endif()

target_include_directories(pacman
    PRIVATE
    "${CMAKE_SOURCE_DIR}/third_party"
    "${CMAKE_SOURCE_DIR}/src"
)

target_compile_features(pacman
    PRIVATE
    cxx_std_17
)

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    target_compile_options(pacman
        PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()

execute_process(
    COMMAND ctags
    --c++-kinds=+p+l
    --fields=+iaS
    --extras=+q+f
    -f .tags
    ${SOURCES}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE CTAGS_RESULT
)
if(CTAGS_RESULT EQUAL 0)
    message(STATUS "Successfully generated .tags file in project root.")
else()
    message(WARNING "Failed to generate .tags file.")
endif()
