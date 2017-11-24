#include "GenericHashTable.h"

typedef struct Table {
    size_t tableSize;
    CloneKeyFcn cloneKey;
    FreeKeyFcn freeKey;
    HashFcn hfun;
    PrintKeyFcn printKeyFun;
    PrintDataFcn printDataFun;
    ComparisonFcn fcomp;
} Table;

TableP createTable(
        size_t tableSize,
        CloneKeyFcn cloneKey,
        FreeKeyFcn freeKey,
        HashFcn hfun,
        PrintKeyFcn printKeyFun, PrintDataFcn printDataFun,
        ComparisonFcn fcomp) {

    if (tableSize == 0 || cloneKey == NULL || freeKey == NULL ||
        hfun == NULL || printKeyFun == NULL ||
        printDataFun == NULL || fcomp == NULL) {
        return 0;
    }

    TableP table = (TableP) malloc(sizeof(Table));
    if (table == NULL) {
        return NULL;
    }

    table->tableSize = tableSize;
    table->cloneKey = cloneKey;
    table->freeKey = freeKey;
    table->hfun = hfun;
    table->printKeyFun = printKeyFun;
    table->printDataFun = printDataFun;
    table->fcomp = fcomp;

    return table;
}

int insert(const TableP table, const void *key, DataP object) {
    return 0;
}

DataP removeData(TableP table, const void *key) {
    return 0;
}

DataP findData(const TableP table, const void *key, int *arrCell, int *listNode) {
    return 0;
}

DataP getDataAt(const TableP table, int arrCell, int listNode) {
    return 0;
}

ConstKeyP getKeyAt(const TableP table, int arrCell, int listNode) {
    return 0;
}

void printTable(const TableP table) {
}

void freeTable(TableP table) {
}
