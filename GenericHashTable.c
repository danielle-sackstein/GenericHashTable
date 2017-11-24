#include "GenericHashTable.h"

TableP createTable(size_t tableSize, CloneKeyFcn cloneKey, FreeKeyFcn freeKey
		,HashFcn hfun,PrintKeyFcn printKeyFun, PrintDataFcn printDataFun
		, ComparisonFcn fcomp)
{
    return 0;
}

int  insert(const TableP table, const void* key, DataP object)
{
    return 0;
}

DataP removeData(TableP table, const void* key)
{
    return 0;
}

DataP findData(const TableP table, const void* key, int* arrCell, int* listNode)
{
    return 0;
}

DataP getDataAt(const TableP table, int arrCell, int listNode)
{
    return 0;
}

ConstKeyP getKeyAt(const TableP table, int arrCell, int listNode)
{
    return 0;
}

void printTable(const TableP table)
{
}

void freeTable(TableP table)
{
}
