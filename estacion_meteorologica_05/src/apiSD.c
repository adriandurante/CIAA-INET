/*
 * apiSD.c
 *
 *  Created on: Oct 25, 2017
 *      Author: seb
 */

#include "sapi.h"
#include "fatfs_stubs.h"
#include "ff.h"
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
	if( f_mount( &FileSystem, "", 0 ) != FR_OK ){
		// ERROR
		return _API_STATE_ERROR;
	}
	return _API_STATE_OK;
}

uint8_t apiSD_Write(uint8_t * strDatalogFilename, uint8_t * stringData) {
	uint32_t bytesWritten, bytesToWrite;

	bytesToWrite = strlen (stringData) - 1;				// Calcula la cantidad de bytes a almacenar,
														// descontando el caracter nulo.
	/************* FAT WRITE **************/
	if( f_open( &File, (char *) strDatalogFilename, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){
		f_write(&File, (char *) stringData, bytesToWrite, &bytesWritten );
		f_close(&File);
		if( bytesWritten == bytesToWrite ){
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


