#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GenericHashTable.h"
#include "MyStringFunctions.h"

#define STR "abcdefghijlmnop"
#define DATA_SIZE (15-4)

int main1(int argc, char *argv[])
{
    // (1) read table size and a key to find
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: GenericHashTable <table size> <key (string up to 9 chars)>\n");
        exit(1);
    }

    unsigned int tableSize;
    char val[10];
    sscanf(argv[1], "%d", &tableSize);
    sscanf(argv[2], "%s", val);

    // (2) create the table
    
    TableP table =  createTable(tableSize, &cloneStr, &freeStr, &strFcn,
    										&strPrint, &strPrint, &strCompare);
    if (table == NULL) 
    {
        printf("ERROR: failed to create table!\n");
        return 0;
    }

    
    // (3) insert objects
    
    char* str= STR;
    int insert_object_j;
    char* data[DATA_SIZE];
    for (unsigned int j=0; j< DATA_SIZE; j++) 
    {
        data[j] = malloc(sizeof(char) * 5);
        strncpy(data[j],str+j,4);
        data[j][4] = '\0';
        insert_object_j = insert(table, data[j], data[j]);
        if (insert_object_j == false) 
	{
            printf("ERROR: failed to insert object %d key %s data %s to the table!\n", j, data[j], data[j]);
            return 0;     
        }
    }

    // (4) print the table
    printTable(table);

    // (5) look for the key
    
    int arrCell;

    findData(table, val, &arrCell);
    printf("%s\t%d\n", val, arrCell);
    
    
    // (6) free the table
    freeTable(table);

	//	(7) free the data 	
 	for (unsigned int i = 0; i < DATA_SIZE; i++) 
    {
        free(data[i]);
    }
    return 0;
}




