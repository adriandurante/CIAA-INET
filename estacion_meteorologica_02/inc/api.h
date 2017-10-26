/*
 * api.h
 *
 *  Created on: Oct 25, 2017
 *      Author: seb
 */

#ifndef PROJECTS_INET_EMB_02_06_ESTACION_METEREOLOGICA_02_INC_API_H_
#define PROJECTS_INET_EMB_02_06_ESTACION_METEREOLOGICA_02_INC_API_H_

#include "apiSD.h"

uint8_t apiReadTemperatureHumdity		(uint16_t * dataTemp, uint16_t * dataHum);
uint8_t apiReadWind						(uint16_t * dataWind);
uint8_t apiReadSensor					(uint16_t * dataTemp, uint16_t * dataHum, uint16_t * dataWind);
uint8_t apiProcessInformation			(uint16_t dataTemp, uint16_t dataHum, uint16_t dataWind, uint8_t * destiny);
uint8_t apiWriteSD						(uint8_t * filename, uint8_t * stringData);

#endif /* PROJECTS_INET_EMB_02_06_ESTACION_METEREOLOGICA_02_INC_API_H_ */
