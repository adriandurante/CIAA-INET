/*
 * apiSD.c
 *
 *  Created on: Oct 25, 2017
 *      Author: seb
 */

#include "sapi.h"

/************* FAT **************/
#include "ff.h"
#include "fatfs_stubs.h"

#include "apiSD.h"

static FATFS FileSystem;
static FIL   File;

bool_t diskTickHook( void *ptr ){
	// Disk timer process
	disk_timerproc();
	return 1;
}
/***************************/

/************* SD **************/
/* supongo que el HW esta configurado antes de usar este módulo */
uint8_t apiSD_Init(void) {
	/************* FAT MOUNT **************/
	if( f_mount_( &FileSystem, "", 0 ) != FR_OK ){
		// ERROR
		return _API_STATE_ERROR;
	}
	return _API_STATE_OK;
}

uint8_t apiSD_Write(char * strDatalogFilename, char * strDatalogData) {
	uint32_t bytesWritten;

	/************* FAT WRITE **************/
	if( f_open_( &File, strDatalogFilename, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){
		f_write_(&File, strDatalogData, 50, &bytesWritten );
		f_close_(&File);
		if( bytesWritten == 50 ){
			// OK
			return _API_STATE_OK;
		}
	} else{
		//  ERROR
		return _API_STATE_ERROR;
	}
	return 0;
}
/***************************/


