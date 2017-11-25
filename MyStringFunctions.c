#include "MyStringFunctions.h"
#include "TableErrorHandle.h"
#include <stdio.h>
#include <string.h>

static int getStringSum(const void *s);

void *cloneStr(const void *s)
{
	char *clone = (char *) malloc(sizeof(char) * strlen(s) + 1);
	char *src = (char *) s;
	char *dst = clone;
	while (*src != 0)
	{
		*dst = *src;
		dst++;
		src++;
	}

	*dst = 0;
	return clone;
}

void freeStr(void *s)
{
	if (s == NULL)
	{
		reportError(INVALID_ARGUMENT);
		return;
	}

	free(s);
}


int strFcn(const void *s, size_t tableSize)
{
	if (s == NULL)
	{
		reportError(INVALID_ARGUMENT);
		return 0;
	}

	int value = getStringSum(s);
	int mod = (int) (value % tableSize);

	if (mod < 0)
	{
		return tableSize + mod;
	}
	return mod;
}

void strPrint(const void *s)
{
	if (s == NULL)
	{
		reportError(MEM_OUT);
		return;
	}

	printf("%s", (char*)s);
}

int strCompare(const void *key1, const void *key2)
{
	if (key1 == 0)
	{
		reportError(INVALID_ARGUMENT);
		return 0;
	}

	char *pt1 = (char *) key1;
	char *pt2 = (char *) key2;

	while (*pt1 != 0 && *pt2 != 0)
	{
		if (*pt1 != *pt2)
		{
			return 1;
		}

		pt1++;
		pt2++;
	}

	if (*pt1 == 0 && *pt2 == 0)
	{
		return 0;
	}
	return 1;
}

static int getStringSum (const void *s)
{
	char *ptr = (char *) s;
	int asciiValue = 0;
	while (*ptr != 0)
	{
		asciiValue += *ptr;
		ptr++;
	}
	return asciiValue;
}

