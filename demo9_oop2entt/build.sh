#!/bin/bash
CC=clang-18 CXX=clang++-18 cmake -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_STANDARD=20
cmake --build build
#ln -sf build/compile_commands.json .
