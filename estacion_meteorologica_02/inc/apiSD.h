/*
 * apiSD.h
 *
 *  Created on: Oct 25, 2017
 *      Author: seb
 */

#ifndef PROJECTS_INET_EMB_02_06_ESTACION_METEREOLOGICA_02_INC_APISD_H_
#define PROJECTS_INET_EMB_02_06_ESTACION_METEREOLOGICA_02_INC_APISD_H_

/* estos son los estados de salida */
#define _API_STATE_OK			(0)
#define _API_STATE_ERROR		(1)

/************* FAT **************/
bool_t 		diskTickHook	( void *ptr );
/***************************/

/************* SD **************/
uint8_t 	apiSD_Init		(void);
uint8_t 	apiSD_Write		(char * strDatalogFilename, char * strDatalogData);
/***************************/


#endif /* PROJECTS_INET_EMB_02_06_ESTACION_METEREOLOGICA_02_INC_APISD_H_ */
