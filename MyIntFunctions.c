#include <stdio.h>
#include "MyIntFunctions.h"
#include "TableErrorHandle.h"

/**
 * @brief clone an int
 */
void *cloneInt(const void *i)
{
	if (i == NULL)
	{
		reportError(GENERAL_ERROR);
		return NULL;
	}

	int *newKey = (int *) malloc(sizeof(int));
	if (newKey == NULL)
	{
		reportError(MEM_OUT);
		return NULL;
	}

	*newKey = *(int *) i;

	return newKey;
}

/**
 * @brief free an int
 */
void freeInt(void *i)
{

	if (i == NULL)
	{
		reportError(GENERAL_ERROR);
	}

	free(i);
}

/**
 * @brief hash value of key for HashTable with size tableSize
 *  assuming key pointer to an int
 * @return number between 0-(tableSize-1)
 */
int intFcn(const void *key, size_t tableSize)
{
	if (key == NULL)
	{
		reportError(GENERAL_ERROR);
		return 0;
	}

	int value = *(int *) key;
	int mod = (int) (value % tableSize);

	if (mod < 0)
	{
		return tableSize + mod;
	}
	return mod;
}

/**
 * @brief print a string
 *  assuming key pointer to an int
 */
void intPrint(const void *key)
{
	if (key == NULL)
	{
		reportError(MEM_OUT);
		return;
	}
	int value = *(int *) key;

	printf("%d", value);
}

/**
 *  intCompare - pointer to int comparison function:
 * @brief compare 2 ints
 *   returns zero int if both ints are equal, otherwise
 *   returns non-zero.
 *
 */
int intCompare(const void *key1, const void *key2)
{
	if (key1 == NULL)
	{
		if (key2 == NULL)
		{
			return 0;
		}
		return 1;
	}

	if (key2 == NULL)
	{
		return 1;
	}

	int value1 = *(int *) key1;
	int value2 = *(int *) key2;

	return value1 - value2;
}

