cmake_minimum_required(VERSION 3.20)
project(demo LANGUAGES CXX)

# === 基础设置 ===
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === 第三方依赖 (PkgConfig) ===
include(FindPkgConfig)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_MIXER REQUIRED IMPORTED_TARGET SDL2_mixer)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)
pkg_check_modules(SDL2GFX REQUIRED IMPORTED_TARGET SDL2_gfx)
pkg_check_modules(ZLIB REQUIRED IMPORTED_TARGET zlib)

# === 源文件收集 ===
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    src/*.cpp inc/*.h inc/*.hpp
    src/*/*.cpp inc/*/*.h inc/*/*.hpp
    3rd/lib/*.cpp 3rd/lib/*.h 3rd/lib/*.hpp
)

# === 可执行文件 ===
add_executable(${PROJECT_NAME} ${SOURCES})

# === 输出目录 ===
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/
)

# === 包含目录 ===
target_include_directories(${PROJECT_NAME} PRIVATE
    inc
    ${CMAKE_SOURCE_DIR}/3rd/entt/single_include
    ${CMAKE_SOURCE_DIR}/3rd/lib
)

# === 编译选项 ===
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror=return-type
    -Werror=uninitialized
    -g
)

# === 链接库 ===
target_link_libraries(${PROJECT_NAME} PRIVATE
    PkgConfig::SDL2
    PkgConfig::SDL2_MIXER
    PkgConfig::SDL2_IMAGE
    PkgConfig::SDL2_TTF
    PkgConfig::SDL2GFX
    PkgConfig::ZLIB
)

# ==========================================
# === 开发辅助工具 ===
# ==========================================

# 收集所有源文件用于工具
file(GLOB_RECURSE DEV_SOURCES CONFIGURE_DEPENDS
    src/*.cpp inc/*.h inc/*.hpp
    src/*/*.cpp inc/*/*.h inc/*/*.hpp
)

# 1. clang-format
find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
    add_custom_target(generate-clang-format
        COMMAND ${CLANG_FORMAT_EXE} --style=llvm --dump-config 
        > ${CMAKE_SOURCE_DIR}/.clang-format
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating .clang-format (LLVM style)"
        VERBATIM
    )
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${DEV_SOURCES}
        COMMENT "Formatting all C++ sources"
        VERBATIM
    )
endif()

# 2. clang-tidy
find_program(CLANG_TIDY_EXE clang-tidy)
if(CLANG_TIDY_EXE)
    add_custom_target(tidy
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} ${DEV_SOURCES}
        COMMENT "Running clang-tidy"
        VERBATIM
    )
    add_custom_target(tidy-fix
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} -fix ${DEV_SOURCES}
        COMMENT "Auto-fixing with clang-tidy"
        VERBATIM
    )
endif()

# 4. ctags 生成
find_program(CTAGS_EXE ctags)
if(CTAGS_EXE)
    add_custom_target(tags
        COMMAND ${CTAGS_EXE}
        --c++-kinds=+p+l
        --fields=+iaS
        --extras=+q+f
        --exclude=build
        --exclude=.git
        --exclude=CMakeFiles
        -f ${CMAKE_SOURCE_DIR}/.tags
        ${DEV_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "🏷️ Generating .tags for src/..."
        VERBATIM
        USES_TERMINAL
    )
    # 可选：构建时自动更新 tags
    add_dependencies(${PROJECT_NAME} tags)
endif()
