#include <stdio.h>
#include "TableErrorHandle.h"

/**
 * @brief Receive an error and report it
 * Do all the errors report through this function.
 * 
 */

void reportError(TableErrors tableError)
{
	switch (tableError) 
	{
		case MEM_OUT:
			fprintf(stderr, "ERROR: Out Of Memory\n");
			return;
		default:
			fprintf(stderr, "ERROR: General error.\n");
			return;
	}
}
