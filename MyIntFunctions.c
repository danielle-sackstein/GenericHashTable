#include <stdio.h>
#include "MyIntFunctions.h"
#include "TableErrorHandle.h"

void *cloneInt(const void *i)
{
	if (i == NULL)
	{
		reportError(INVALID_ARGUMENT);
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

void freeInt(void *i)
{

	if (i == NULL)
	{
		reportError(INVALID_ARGUMENT);
	}

	free(i);
}

int intFcn(const void *key, size_t tableSize)
{
	if (key == NULL)
	{
		reportError(INVALID_ARGUMENT);
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

