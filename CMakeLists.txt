cmake_minimum_required(VERSION 3.8)
project(GenericHashTable)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
    GenericHashTable.h
    HashIntSearch.c
    HashStrSearch.c
    Key.h
    main.c
    MyIntFunctions.h
    MyStringFunctions.h
    TableErrorHandle.c
    TableErrorHandle.h)

add_executable(GenericHashTable ${SOURCE_FILES})