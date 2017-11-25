#include <assert.h>
#include <stdio.h>
#include "GenericHashTable.h"
#include "TableErrorHandle.h"

typedef struct KeyValue {
    ConstKeyP key;
    DataP value;
} KeyValue;

typedef struct Table {
    size_t tableSize;
    KeyValue *keyValues;
    size_t growthFactor;

    CloneKeyFcn cloneKey;
    FreeKeyFcn freeKey;
    HashFcn hfun;
    PrintKeyFcn printKeyFun;
    PrintDataFcn printDataFun;
    ComparisonFcn fcomp;

} Table;

static void zeroKeyValues(KeyValue *keyValues, size_t count);

static int tryInsertAt(
        TableP table,
        const void *key, DataP object,
        size_t index, bool *inserted);

static int growTheTable(TableP table);

static int tryInsertAtHash(
        TableP table,
        const void *key,
        DataP object,
        int hash,
        bool *inserted);

static void printKeyValue(const struct Table *table, size_t i);

static void copyKeyValues(const struct Table *table, KeyValue *newKeyValues);

static void checkTable(TableP table);

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
        reportError(INVALID_ARGUMENT);
        return 0;
    }

    TableP table = (TableP) malloc(sizeof(Table));
    if (table == NULL) {
        reportError(MEM_OUT);
        return NULL;
    }

    table->keyValues = (KeyValue *) malloc(sizeof(KeyValue) * tableSize);
    if (table->keyValues == NULL) {
        reportError(MEM_OUT);
        free(table);
        return NULL;
    }

    zeroKeyValues(table->keyValues, tableSize);

    table->tableSize = tableSize;
    table->growthFactor = 1;
    table->cloneKey = cloneKey;
    table->freeKey = freeKey;
    table->hfun = hfun;
    table->printKeyFun = printKeyFun;
    table->printDataFun = printDataFun;
    table->fcomp = fcomp;

    return table;
}

int insert(const TableP table, const void *key, DataP object) {

    if (table == NULL || key == NULL || object == NULL) {
        reportError(INVALID_ARGUMENT);
        return 0;
    }

    int hash = table->hfun(key, table->tableSize);

    bool inserted = false;

    int success = tryInsertAtHash(table, key, object, hash, &inserted);

    if (success == 0) {
        return 0;
    }

    if (inserted) {
        return 1;
    }

    success = growTheTable(table);
    if (success == 0) {
        return 0;
    }

    success = tryInsertAtHash(table, key, object, hash, &inserted);
    if (success == 0) {
        return 0;
    }

    assert(inserted);

    return 1;
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

    for (size_t hash = 0; hash < table->tableSize; hash++) {

        printf("[%d]\t", (int) hash);

        for (size_t i = 0; i < table->growthFactor; i++) {

            printKeyValue(table, hash + i);
        }

        printf("\n");
    }
}

void freeTable(TableP table) {

    if (table == NULL) {
        return;
    }
    if (table->keyValues != NULL) {
        free(table->keyValues);
    }
    free(table);
}

static void zeroKeyValues(KeyValue *keyValues, size_t count) {
    for (size_t i = 0; i < count; i++) {
        keyValues[i].key = NULL;
        keyValues[i].value = NULL;
    }
}

static int
tryInsertAt(TableP table, const void *key, DataP object, size_t index, bool *inserted) {
    *inserted = false;

    KeyValue *keyValue = table->keyValues + index;

    if (keyValue->key == NULL) {

        ConstKeyP clone = table->cloneKey(key);

        if (clone == NULL) {
            reportError(MEM_OUT);
            *inserted = false;
            return 0;
        }

        keyValue->key = clone;
        keyValue->value = object;

        *inserted = true;
        return 1;
    }

    if (table->fcomp(keyValue->key, key) == 0) {
        keyValue->value = object;
        *inserted = true;
        return 1;
    }

    *inserted = false;
    return 1;
}

static int growTheTable(TableP table) {

    size_t newGrowthFactor = table->growthFactor * 2;
    size_t keyValueCount = newGrowthFactor * table->tableSize;
    KeyValue *newKeyValues = (KeyValue *) malloc(sizeof(KeyValue) * keyValueCount);

    if (newKeyValues == NULL) {
        reportError(MEM_OUT);
        return 0;
    }

    zeroKeyValues(newKeyValues, keyValueCount);

    copyKeyValues(table, newKeyValues);

    free(table->keyValues);
    table->growthFactor = newGrowthFactor;
    table->keyValues = newKeyValues;

    return 1;
}

static int
tryInsertAtHash(TableP table, const void *key, DataP object, int hash, bool *inserted) {

    *inserted = false;

    for (size_t i = 0; i < table->growthFactor; i++) {
        int success = tryInsertAt(table, key, object, hash + i, inserted);
        if (success == 0) {
            return 0;
        }
        if (*inserted) {
            return 1;
        }
    }

    return 1;
}

static void printKeyValue(const struct Table *table, size_t i) {
    KeyValue *keyValue = &table->keyValues[i];

    if (keyValue->key != NULL) {

        table->printKeyFun(keyValue->key);
        printf(",");
        table->printDataFun(keyValue->value);
        printf("-->");
        printf("\t");
    }
}

static void copyKeyValues(const struct Table *table, KeyValue *newKeyValues) {
    for (size_t i = 0; i < table->tableSize * table->growthFactor; i++) {
        KeyValue* oldKeyValue = &table->keyValues[i];
        KeyValue* newKeyValue = &newKeyValues[i * 2];

        if (oldKeyValue->key != NULL) {
            newKeyValue->key = oldKeyValue->key;
            newKeyValue->value = oldKeyValue->value;
        }
    }
}

//static void checkTable(TableP table) {
//    for (size_t i = 0; i < table->tableSize * table->growthFactor; i++) {
//        KeyValue* keyValue = &table->keyValues[i];
//        bool keyIsNull = keyValue->key == NULL;
//        bool valueIsNull = keyValue->value == NULL;
//        if (keyIsNull != valueIsNull) {
//            reportError(GENERAL_ERROR);
//        }
//    }
//}

