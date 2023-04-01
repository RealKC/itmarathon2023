cmake_minimum_required(VERSION 3.25)
project(itmarathon2023)

set(CMAKE_CXX_STANDARD 17)

add_executable(itmarathon2023 src/main.cpp
        src/Memory.cpp src/Memory.h
        src/Instruction.cpp src/Instruction.h
        src/DataCache.cpp src/DataCache.h
        src/Execute.cpp src/Execute.h
        src/InstructionCache.cpp src/InstructionCache.h
        )
