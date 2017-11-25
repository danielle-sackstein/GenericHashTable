#include <stdio.h>
#include <stdlib.h>
#include "GenericHashTable.h"
#include "MyStringFunctions.h"

int main(int argc, char *argv[])
{
	size_t tableSize = 2;

	TableP table = createTable(tableSize, &cloneStr, &freeStr, &strFcn,
							   &strPrint, &strPrint, &strCompare);

	char* data[] = {"a", "b", "c", "d", "e"};

	for (size_t i=0; i<sizeof(data) / sizeof(data[0]); i++)
	{
		insert(table, &data[i], &data[i]);
		printTable(table);
		printf("\n");
	}

//	int a = 5;
//	removeData(table, &a);
//
//	printTable(table);

	freeTable(table);

	return 0;
}
