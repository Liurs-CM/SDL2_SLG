#!/bin/bash
# 一键配置+编译+运行

# 配置（使用 clang，RelWithDebInfo 模式）
cmake -B build \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_CXX_COMPILER=clang++

# 编译（使用所有核心）
cmake --build build -j$(nproc)

# 如果编译成功，运行程序
if [ $? -eq 0 ]; then
    echo "========== 运行程序 =========="
    ./demo
fi
