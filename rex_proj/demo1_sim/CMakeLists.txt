CMake_Minimum_Required (VERSION 3.10)
Project (demo1)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2GFX REQUIRED SDL2_gfx)

add_compile_options(-Wall -Wempty-body -Werror -Werror=maybe-uninitialized -Warray-bounds -g)

#Add the include directories of the (found) libraries.
Include_Directories(
 	${SDL2_INCLUDE_DIRS}
 	${SDL2_MIXER_INCLUDE_DIRS}
 	${SDL2_IMAGE_INCLUDE_DIRS}
 	${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2GFX_INCLUDE_DIRS}
)

#file(GLOB SOURCES   src/defs.h src/structs.h src/*.c src/*.h src/*/*.c src/*/*.h)
file(GLOB SOURCES src/*.cpp src/*.h src/*/*.cpp src/*/*.h)

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# 生成 tags 目标
add_custom_target(tags
    COMMAND ctags -R -o .tags .
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating tags file..."
)
# 可选：编译后自动运行
#add_dependencies(ALL tags)  # 注意：这会让 tags 每次都运行

Add_Executable (${CMAKE_PROJECT_NAME} ${SOURCES})
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)

Target_Link_Libraries (
 	${CMAKE_PROJECT_NAME}
 	${SDL2_LIBRARIES}
 	${SDL2_MIXER_LIBRARIES}
 	${SDL2_IMAGE_LIBRARIES}
 	${SDL2_TTF_LIBRARIES}
    ${SDL2GFX_LIBRARIES}
)
