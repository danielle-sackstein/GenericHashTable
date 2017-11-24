cmake_minimum_required(VERSION 3.8)
project(GenericHashTable)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES
        GenericHashTable.h
        GenericHashTable.c
        HashIntSearch.c
        HashStrSearch.c
        Key.h
        MyIntFunctions.h
        MyStringFunctions.h
        TableErrorHandle.c
        TableErrorHandle.h MyIntFunctions.c MyStringFunctions.c)

add_executable(GenericHashTable ${SOURCE_FILES})