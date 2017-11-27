#include <assert.h>
#include <stdio.h>
#include "GenericHashTable.h"
#include "TableErrorHandle.h"

typedef struct KeyValue
{
	void* key;
	DataP value;
} KeyValue;

typedef struct Table
{
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

static bool isInRange(TableP table, int arrCell);

//static void checkTable(TableP table);

/**
 * @brief Allocate memory for a hash table with which uses the given functions.
 * tableSize is the number of cells in the hash table.
 * If run out of memory, free all the memory that was already allocated by the function,
 * report error MEM_OUT to the standard error and return NULL.
 */
TableP createTable(
		size_t tableSize,
		CloneKeyFcn cloneKey,
		FreeKeyFcn freeKey,
		HashFcn hfun,
		PrintKeyFcn printKeyFun, PrintDataFcn printDataFun,
		ComparisonFcn fcomp)
{

	if (tableSize == 0 || cloneKey == NULL || freeKey == NULL ||
		hfun == NULL || printKeyFun == NULL ||
		printDataFun == NULL || fcomp == NULL)
	{
		reportError(INVALID_ARGUMENT);
		return 0;
	}

	TableP table = (TableP) malloc(sizeof(Table));
	if (table == NULL)
	{
		reportError(MEM_OUT);
		return NULL;
	}

	table->keyValues = (KeyValue *) malloc(sizeof(KeyValue) * tableSize);
	if (table->keyValues == NULL)
	{
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

/**
 * @brief Insert an object to the table with key.
 * If all the cells appropriate for this object are full, duplicate the table.
 * If run out of memory, report
 * MEM_OUT and do nothing (the table should stay at the same situation
 * as it was before the duplication).
 * If everything is OK, return true. Otherwise (an error occured) return false;
 */
int insert(const TableP table, const void *key, DataP object)
{
	if (table == NULL || key == NULL || object == NULL)
	{
		reportError(INVALID_ARGUMENT);
		return 0;
	}

	int hash = table->hfun(key, table->tableSize);

	bool inserted = false;

	int success = tryInsertAtHash(table, key, object, hash, &inserted);

	if (success == 0)
	{
		return 0;
	}

	if (inserted)
	{
		return 1;
	}

	success = growTheTable(table);
	if (success == 0)
	{
		return 0;
	}

	success = tryInsertAtHash(table, key, object, hash, &inserted);
	if (success == 0)
	{
		return 0;
	}

	assert(inserted);

	return 1;
}


/**
 * @brief remove an data from the table.
 * If everything is OK, return the pointer to the ejected data. Otherwise return NULL;
 */
DataP removeData(TableP table, const void *key)
{
	int arrCell;
	DataP data = findData(table, key, &arrCell);
	if (data == NULL){
		return NULL;
	}

	KeyValue* keyValue = &table->keyValues[arrCell];

	table->freeKey(keyValue->key);

	keyValue->key = NULL;
	keyValue->value = NULL;

	return data;
}


/**
 * @brief Search the table and look for an object with the given key.
 * If such object is found fill its cell number into arrCell (where 0 is the
 * first cell).
 * If the key was not found, fill both pointers with value of -1.
 * return pointer to the data or null
 */
DataP findData(const TableP table, const void *key, int *arrCell)
{
	if (table == NULL || key == NULL || arrCell == NULL)
	{
		reportError(INVALID_ARGUMENT);
		return NULL;
	}

	int hash = table->hfun(key, table->tableSize);

	size_t hashStart = hash * table->growthFactor;

	for (size_t i = 0; i < table->growthFactor; i++)
	{
		size_t index = hashStart + i;
		KeyValue* keyValue = &table->keyValues[index];

		if (table->fcomp(keyValue->key, key) == 0)
		{
			*arrCell = (int)index;
			return keyValue->value;
		}
	}
	// TODO: What is meant by "fill the two pointers with -1" We only have arrCell
	*arrCell = -1;
	return NULL;
}

/**
 * @brief return a pointer to the data that exist in the table in cell number arrCell (where 0 is
 * the first cell).
 * If such data not exist return NULL
 */
DataP getDataAt(const TableP table, int arrCell)
{
	if (table == NULL || !isInRange(table, arrCell))
	{
		reportError(INVALID_ARGUMENT);
		return NULL;
	}
	return table->keyValues[arrCell].value;
}

/**
 * @brief return the pointer to the key that exist in the table in cell number arrCell (where 0 is the
 * first cell).
 * If such key not exist return NULL
 */
ConstKeyP getKeyAt(const TableP table, int arrCell)
{
	if (table == NULL || !isInRange(table, arrCell))
	{
		reportError(INVALID_ARGUMENT);
		return NULL;
	}
	return table->keyValues[arrCell].key;
}

/**
 * @brief Print the table (use the format presented in PrintTableExample).
 */
void printTable1(const TableP table)
{
	for (size_t hash = 0; hash < table->tableSize; hash++)
	{
		printf("[%d]\t", (int) hash);

		size_t hashStart = hash*table->growthFactor;

		for (size_t i = 0; i < table->growthFactor; i++)
		{
			printKeyValue(table, hashStart + i);
		}

		printf("\n");
	}
}

void printTable(const TableP table)
{
	for (size_t hash = 0; hash < table->tableSize; hash++)
	{
		size_t hashStart = hash*table->growthFactor;

		for (size_t i = 0; i < table->tableSize * table->growthFactor; i++)
		{
			printf("[%zu]\t", i);
			printKeyValue(table, hashStart + i);
		}

		printf("\n");
	}
}

/**
 * @brief Free all the memory allocated for the table.
 * It's the user responsibility to call this function before exiting the program.
 */
void freeTable(TableP table)
{
	if (table == NULL)
	{
		return;
	}
	if (table->keyValues != NULL)
	{
		free(table->keyValues);
	}
	free(table);
}

/**
 * Go through all of the key values and initiate them to NULL
 * @param keyValues a pointer to a key-value
 * @param count the number of keyvalues
 */
static void zeroKeyValues(KeyValue *keyValues, size_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		keyValues[i].key = NULL;
		keyValues[i].value = NULL;
	}
}

/**
 * try to insert the key-data at a specific index
 * @param table a pointer to the table
 * @param key a pointer to the key
 * @param object a DataP object
 * @param index the specific index we want to insert at
 * @param inserted a flag which tells if the insertion was done or not
 * @return 0 if there was an error, 1 otherwise.
 */
static int tryInsertAt(
		TableP table, const void *key, DataP object, size_t index, bool *inserted)
{
	*inserted = false;

	KeyValue *keyValue = table->keyValues + index;

	if (keyValue->key == NULL)
	{
		void* clone = table->cloneKey(key);

		if (clone == NULL)
		{
			reportError(MEM_OUT);
			*inserted = false;
			return 0;
		}

		keyValue->key = clone;
		keyValue->value = object;

		*inserted = true;
		return 1;
	}

	if (table->fcomp(keyValue->key, key) == 0)
	{
		keyValue->value = object;
		*inserted = true;
		return 1;
	}

	*inserted = false;
	return 1;
}

/**
 * grow the table if there was a collision between two values hashes.
 * @param table a pointer to the table
 * @return 0 if there was an error, 1 otherwise.
 */
static int growTheTable(TableP table)
{
	size_t newGrowthFactor = table->growthFactor * 2;
	size_t keyValueCount = newGrowthFactor * table->tableSize;
	KeyValue *newKeyValues = (KeyValue *) malloc(sizeof(KeyValue) * keyValueCount);

	if (newKeyValues == NULL)
	{
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

/**
 * try to insert in the given hash
 * @param table a pointer to the table
 * @param key a pointer to the key
 * @param object a DataP object
 * @param index the specific index we want to insert at
 * @param inserted a flag which tells if the insertion was done or not
 * @return 0 if there was an error, 1 otherwise.
 */
static int tryInsertAtHash(
		TableP table, const void *key, DataP object, int hash, bool *inserted)
{
	*inserted = false;

	size_t hashStart = hash*table->growthFactor;

	for (size_t i = 0; i < table->growthFactor; i++)
	{
		int success = tryInsertAt(table, key, object, hashStart + i, inserted);
		if (success == 0)
		{
			return 0;
		}
		if (*inserted)
		{
			return 1;
		}
	}

	return 1;
}

/**
 * print the KeyValue
 * @param table a pointer to the tavle
 * @param i the index of the keyvalue we want to print
 */
static void printKeyValue(const struct Table *table, size_t i)
{
	KeyValue *keyValue = &table->keyValues[i];

	if (keyValue->key != NULL)
	{
		table->printKeyFun(keyValue->key);
		printf(",");
		table->printDataFun(keyValue->value);
//		printf("-->");
		printf("\t");
	}
	printf("\n");
}

/**
 * copy the key values to new key values
 * @param table a pointer to the table
 * @param newKeyValues a pointer to the new keyValues
 */
static void copyKeyValues(const struct Table *table, KeyValue *newKeyValues)
{
	for (size_t i = 0; i < table->tableSize * table->growthFactor; i++)
	{
		KeyValue *oldKeyValue = &table->keyValues[i];
		KeyValue *newKeyValue = &newKeyValues[i * 2];

		if (oldKeyValue->key != NULL)
		{
			newKeyValue->key = oldKeyValue->key;
			newKeyValue->value = oldKeyValue->value;
		}
	}
}

/**
 * check if the cell is in range of the list
 * @param table
 * @param arrCell
 * @return
 */
static bool isInRange(TableP table, int arrCell)
{
	return (arrCell >= 0) &&
		   (arrCell < (int)(table->tableSize * table->growthFactor));
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

