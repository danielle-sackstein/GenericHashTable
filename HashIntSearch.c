#include <stdio.h>
#include <stdlib.h>
#include "GenericHashTable.h"
#include "MyIntFunctions.h"

#define MINIMAL_VAL -15
#define MAXIMAL_VAL 15
#define DATA_SIZE (MAXIMAL_VAL - MINIMAL_VAL +1)
/**
* main
*/
int main(int argc, char *argv[]) 
{
	// (1) read table size and a key to find
    if (argc < 3) 
    {
        fprintf(stderr, "Usage: GenericHashTable <table size> <key>\n");
        exit(1);
    }

    size_t tableSize;
    int val;
    sscanf(argv[1], "%d", (unsigned int *)(&tableSize));
    sscanf(argv[2], "%d", &val);
    
    
    // (2) create the table
    
    TableP table = createTable(tableSize, &cloneInt, &freeInt, &intFcn,
    										&intPrint, &intPrint, &intCompare);
    if (table == NULL) 
    {
        printf("ERROR: failed to create table!\n");
        return 0;
    }
    

    // (3) insert objects
    
    int i;
    int insert_object_i;
    int* data[DATA_SIZE];
    for (i = 0; i < DATA_SIZE; i++) 
    {
        data[i] = malloc(sizeof(int));
        *data[i] = i+MINIMAL_VAL;
        
        insert_object_i = insert(table, data[i],data[i]);
        if (insert_object_i == false)	
        {
            printf("ERROR: failed to insert object %d key %d data %d to the table!\n", i,*data[i],*data[i]);
            return 0;   
        }
    }

    // (4) print the table
    printTable(table);

    // (5) look for the key
    
    int arrCell;

    int *res=findData(table, &val, &arrCell);
    printf("%d=%d\t%d\n", *res,val, arrCell);


    // (6) free the table
    freeTable(table);
        
        
            
	//	(7) free the data 	
 	for (unsigned int i = 0; i < DATA_SIZE; i++) 
    {
        free(data[i]);
    }
    return 0;
}
