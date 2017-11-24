cmake_minimum_required(VERSION 3.8)
project(GenericHashTable)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        GenericHashTable.h
        GenericHashTable.c
        HashIntSearch.c
        HashStrSearch.c
        Key.h
        MyIntFunctions.h
        MyStringFunctions.h
        TableErrorHandle.c
        TableErrorHandle.h)

add_executable(GenericHashTable ${SOURCE_FILES})