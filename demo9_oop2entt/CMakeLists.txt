cmake_minimum_required(VERSION 3.28)
project(demo LANGUAGES CXX)

# === 基础设置 ===
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 注：CMake 3.28+ 已稳定 C++20 模块 API，无需再设置 CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API
# 建议通过命令行指定编译器，而非硬编码在 CMakeLists 中：
# cmake -DCMAKE_CXX_COMPILER=clang++ -B build
# set(CMAKE_CXX_COMPILER clang++)

# === 第三方依赖 (PkgConfig 现代用法) ===
include(FindPkgConfig)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
pkg_check_modules(SDL2_MIXER REQUIRED IMPORTED_TARGET SDL2_mixer)
pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)
pkg_check_modules(SDL2GFX REQUIRED IMPORTED_TARGET SDL2_gfx)
pkg_check_modules(ZLIB REQUIRED IMPORTED_TARGET zlib)

# === 可执行文件定义 ===
add_executable(${PROJECT_NAME})

# === 源文件声明 (C++20 模块最佳实践：弃用 GLOB，显式声明) ===
# 1. 注册 C++20 模块接口单元（必须放入 FILE_SET）
target_sources(${PROJECT_NAME} PRIVATE
    FILE_SET CXX_MODULES FILES
    src/ecs/entt_wrapper.cppm
    src/ecs/systems/InputSystem.cppm
    src/core/InputHandler.cppm
    src/core/RenderContext.cppm
    src/core/Game.cppm
)
# 2. 传统 .cpp 源文件
target_sources(${PROJECT_NAME} PRIVATE
    src/main.cpp
)

# === 输出目录 ===
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# === 编译选项 ===
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wempty-body -Werror -Warray-bounds -g
    $<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Werror=maybe-uninitialized>
    $<$<COMPILE_LANG_AND_ID:CXX,Clang>:
        -fmodules
        -fno-implicit-module-maps  # 🔑 核心修复：禁用 Clang 对 libstdc++ 的隐式模块映射
        -fno-implicit-modules      # 🔑 防止系统头文件自动转为模块引发 ODR 冲突
    >
)

# === 包含目录 ===
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/3rd/entt/single_include
    # PkgConfig IMPORTED_TARGET 已自动处理 SDL/Zlib 的 include，无需手动添加
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
# === 开发辅助工具 (Format / Tidy / AST / Tags) ===
# ==========================================
# 为工具链收集源文件（仅用于辅助命令，不影响构建依赖图）
file(GLOB_RECURSE DEV_SOURCES CONFIGURE_DEPENDS
    src/*.cpp src/*.cppm src/*.h
)

# 1. clang-format
find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
    add_custom_target(generate-clang-format
        COMMAND ${CLANG_FORMAT_EXE} --style=llvm --dump-config > ${CMAKE_SOURCE_DIR}/.clang-format
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "🔧 Generating .clang-format (LLVM style)"
        VERBATIM
    )
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${DEV_SOURCES}
        COMMENT "✨ Formatting all C++ sources with clang-format"
        VERBATIM
    )
else()
    message(WARNING "clang-format not found. Install it to enable format targets.")
endif()

# 2. clang-tidy
find_program(CLANG_TIDY_EXE clang-tidy)
if(CLANG_TIDY_EXE)
    add_custom_target(tidy
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} ${DEV_SOURCES}
        COMMENT "🔍 Running clang-tidy using compile_commands.json"
        VERBATIM
    )
    add_custom_target(tidy-fix
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} -fix ${DEV_SOURCES}
        COMMENT "🩹 Auto-fixing fixable issues with clang-tidy"
        VERBATIM
    )
else()
    message(WARNING "clang-tidy not found. Install it to enable tidy targets.")
endif()

# 3. AST / IR 调试
set(MAIN_SOURCE_FILE ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_custom_target(ast
    COMMAND ${CMAKE_CXX_COMPILER} -Xclang -ast-dump -fsyntax-only ${MAIN_SOURCE_FILE}
    COMMENT "🌳 Dumping AST of ${MAIN_SOURCE_FILE}"
    VERBATIM
)
add_custom_target(ir
    COMMAND ${CMAKE_CXX_COMPILER} -S -emit-llvm ${MAIN_SOURCE_FILE} -o ${CMAKE_BINARY_DIR}/main.ll
    COMMENT "⚙️ Generating LLVM IR: ${CMAKE_BINARY_DIR}/main.ll"
    VERBATIM
)

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
    )
    # 可选：构建时自动更新 tags
    # add_dependencies(${PROJECT_NAME} tags)
endif()
