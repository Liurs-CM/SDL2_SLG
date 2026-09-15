#!/bin/bash
# 一键配置+编译+运行
cmake -B build \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
    -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=mold"

# 编译时使用 ninja
ninja -C build -j$(nproc)

# 如果编译成功，运行程序
if [ $? -eq 0 ]; then
    echo "========== 运行程序 =========="
    ./demo
fi


## old配置（使用 clang，RelWithDebInfo 模式）
#cmake -B build \
#    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
#    -DCMAKE_CXX_COMPILER=clang++
#
## 编译（使用所有核心）
#cmake --build build -j$(nproc)

