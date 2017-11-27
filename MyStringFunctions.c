#include "MyStringFunctions.h"
#include "TableErrorHandle.h"
#include <stdio.h>
#include <string.h>

static int getStringSum(const void *s);

/**
 * @brief clone a string
 */
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

/**
 * @brief free a string
 */
void freeStr(void *s)
{
	if (s == NULL)
	{
		reportError(GENERAL_ERROR);
		return;
	}

	free(s);
}


/**
 * @brief hash value of key for HashTable with size tableSize
 *  assuming key pointer to string
 * @return number between 0-(tableSize-1)

 */
int strFcn(const void *s, size_t tableSize)
{
	if (s == NULL)
	{
		reportError(GENERAL_ERROR);
		return 0;
	}

	int value = getStringSum(s);
	int mod = (int) (value % tableSize);

	if (mod < 0)
	{
		return (int) tableSize + mod;
	}
	return mod;
}


/**
 * @brief print a string
 *  assuming key pointer to string
 *
 */
void strPrint(const void *s)
{
	if (s == NULL)
	{
		reportError(MEM_OUT);
		return;
	}

	printf("%s", (char*)s);
}

/**
 *  strCompare - pointer to int comparison function:
 * @brief compare 2 strings
 *   returns zero int if both strings are equal, otherwise
 *   returns non-zero.
 */
int strCompare(const void *key1, const void *key2)
{
	if (key1 == 0)
	{
		reportError(GENERAL_ERROR);
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

/**
 * gets the ascii value of the string
 * @param s a pointer to the string
 * @return return the ascii value of the string
 */
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

