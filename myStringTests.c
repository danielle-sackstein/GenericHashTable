#include <stdio.h>
#include <stdlib.h>
#include "GenericHashTable.h"
#include "MyStringFunctions.h"

int main7(int argc, char *argv[])
{
	size_t tableSize = 2;

	TableP table = createTable(tableSize, &cloneStr, &freeStr, &strFcn,
							   &strPrint, &strPrint, &strCompare);


	char* data[] = {"a", "b","c","d","e","f"};

	for (size_t i=0; i<sizeof(data) / sizeof(data[0]); i++)
	{
		insert(table, data[i], data[i]);
		printTable(table);
		printf("\n");
	}

//	int a = 5;
//	removeData(table, &a);
//
//	printTable(table);


	int arrCell;

	findData(table,data[0], &arrCell);

	printf("%s\t%d\n",data[0], arrCell);
	freeTable(table);

	return 0;
}
