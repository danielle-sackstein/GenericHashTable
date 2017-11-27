#include <stdio.h>
#include <stdlib.h>
#include "GenericHashTable.h"
#include "MyIntFunctions.h"

int main8(int argc, char *argv[])
{
	size_t tableSize = 2;

	TableP table = createTable(tableSize, &cloneInt, &freeInt, &intFcn,
							   &intPrint, &intPrint, &intCompare);

	int data[] = {0, 1, 2, 3, 4, 5};

	for (size_t i=0; i<sizeof(data) / sizeof(data[0]); i++)
	{

		insert(table, &data[i], &data[i]);
		printTable(table);
		printf("\n");
	}

	int a = 5;
	removeData(table, &a);
	printTable(table);

	int b = 3;
	removeData(table, &b);
	printTable(table);


	int cell = 0;
	findData(table, &data[4],&cell);
	printf("%d\n", cell);

	freeTable(table);

	return 0;
}
