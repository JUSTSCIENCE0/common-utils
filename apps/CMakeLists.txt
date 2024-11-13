# Copyright (c) 2024, Yakov Usoltsev
# Email: yakovmen62@gmail.com
#
# License: MIT

cmake_minimum_required(VERSION 3.22)

if (CU_ENABLE_CPU_UTILS)
    add_subdirectory(cpu-descriptor)
endif(CU_ENABLE_CPU_UTILS)