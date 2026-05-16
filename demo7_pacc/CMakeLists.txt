cmake_minimum_required (VERSION 3.28)
project (demo LANGUAGES CXX)

#CC=clang-18 CXX=clang++-18 cmake -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_STANDARD=20
#cmake --build build
#ln -sf build/compile_commands.json .

# === 基础设置 ===
set(CMAKE_CXX_STANDARD 20)
#advise comment to use "cmake -DCMAKE_CXX_COMPILER=clang++ -B build"
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # 生成 compile_commands.json
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API "aa1f7df0-828a-4fcd-9afc-2dc805d74296") # CMake 3.28+

# === 第三方依赖（SDL2 系列 + Zlib）===
include(FindPkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2GFX REQUIRED SDL2_gfx)
pkg_check_modules(ZLIB REQUIRED zlib)

# === 源文件收集（包含 .cppm 模块文件）===
#file(GLOB SOURCES   src/defs.h src/structs.h src/*.c src/*.h src/*/*.c src/*/*.h)
file(GLOB SOURCES CONFIGURE_DEPENDS
    inc/*.h inc/*/*.h
    src/*.hpp src/*/*.hpp
    src/*.cpp src/*/*.cpp 
    src/*.cppm src/*/*.cppm
)

# === 输出目录 ===
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

# === 可执行文件定义 ===
add_executable (${CMAKE_PROJECT_NAME} ${SOURCES})
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# === 编译选项 ===
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    -Wall -Wempty-body -Werror -Warray-bounds -g
    $<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Werror=maybe-uninitialized>
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2GFX_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
)

# 链接库
Target_Link_Libraries (
 	${CMAKE_PROJECT_NAME}
 	${SDL2_LIBRARIES}
 	${SDL2_MIXER_LIBRARIES}
 	${SDL2_IMAGE_LIBRARIES}
 	${SDL2_TTF_LIBRARIES}
    ${SDL2GFX_LIBRARIES}
    ${ZLIB_LIBRARIES}
)

# === 1. 自动生成 .clang-format（仅首次或手动调用）===
find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
    add_custom_target(generate-clang-format
        COMMAND ${CLANG_FORMAT_EXE} --style=llvm --dump-config > ${CMAKE_SOURCE_DIR}/.clang-format
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "📝 Generating .clang-format (LLVM style)"
        VERBATIM
    )
endif()

# === 2. 代码格式化目标 ===
if(CLANG_FORMAT_EXE)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${SOURCES}
        COMMENT "✅ Formatting all C++ sources with clang-format"
        VERBATIM
    )
else()
    message(WARNING "clang-format not found. Install it to enable 'format' and 'generate-clang-format' targets.")
endif()

# === 3. 静态分析：clang-tidy ===
find_program(CLANG_TIDY_EXE clang-tidy)
if(CLANG_TIDY_EXE)
    add_custom_target(tidy
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} ${SOURCES}
        COMMENT "🔍 Running clang-tidy using compile_commands.json"
        VERBATIM
    )

    add_custom_target(tidy-fix
        COMMAND ${CLANG_TIDY_EXE} -p ${CMAKE_BINARY_DIR} -fix ${SOURCES}
        COMMENT "🔧 Auto-fixing fixable issues with clang-tidy"
        VERBATIM
    )
else()
    message(WARNING "clang-tidy not found. Install it to enable 'tidy' targets.")
endif()

# === 4. 调试辅助：AST / IR 查看（以 main.cpp 为例）===
# 自动查找主源文件（假设存在 src/main.cpp 或类似）
set(MAIN_FILES ${SOURCES})
list(FILTER MAIN_FILES INCLUDE REGEX "main\\.cpp$")
list(LENGTH MAIN_FILES main_count)
if(main_count GREATER 0)
    list(GET MAIN_FILES 0 MAIN_SOURCE_FILE)
else()
    set(MAIN_SOURCE_FILE ${CMAKE_SOURCE_DIR}/src/main.cpp)
endif()


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

# === 5. tags 生成（保留你原有的逻辑，但优化为 target-only）===
# 移除 execute_process（避免配置时运行），统一用 custom target
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
        ${CMAKE_SOURCE_DIR}/inc
        ${CMAKE_SOURCE_DIR}/src
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "🔖 Generating .tags for inc/ and src/..."
        VERBATIM
    )
    # 可选：构建时自动更新 tags（取消注释下一行即可）
    add_dependencies(${CMAKE_PROJECT_NAME} tags)
endif()

#execute_process(
#    COMMAND ctags
#    --c++-kinds=+p+l
#    --fields=+iaS
#    --extras=+q+f
#    -f ${CMAKE_SOURCE_DIR}/.tags
#    ${SOURCES}
#    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#    RESULT_VARIABLE CTAGS_RESULT
#)
#if(CTAGS_RESULT EQUAL 0)
#    message(STATUS "Successfully generated .tags file in project root.")
#else()
#    message(WARNING "Failed to generate .tags file.")
#endif()
