#ifndef _TABLE_ERROR_HANDLE_H_
#define _TABLE_ERROR_HANDLE_H_

/*! This is TableErrors enum  */
typedef enum 
{
	MEM_OUT, /*!< this is out of memory */ 
	GENERAL_ERROR /*!< this is general error */
	
} TableErrors;



/**
 * @brief Receive an error and report it
 * Do all the errors report through this function.
 * 
 */

void reportError(TableErrors tableError);


#endif // _TABLE_ERROR_HANDLE_H_
