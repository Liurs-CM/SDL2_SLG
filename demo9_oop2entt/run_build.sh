#!/bin/bash
# 1. 配置（推荐指定 clang++ 与 Ninja）
#CC=clang-18 CXX=clang++-18 cmake -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_STANDARD=20
CC=clang-18 CXX=clang++-18 cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug

# 2. 编译
cmake --build build

# 3. 运行
#./bin/demo

# 4. 开发工具调用
#cmake --build build --target format      # 代码格式化
#cmake --build build --target tidy        # 静态分析
#cmake --build build --target ast         # 查看 main.cpp AST
#cmake --build build --target tags        # 生成 .tags
#cmake --build build

#ln -sf build/compile_commands.json .
