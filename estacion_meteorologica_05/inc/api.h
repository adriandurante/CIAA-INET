/*
 * api.h
 *
 *  Created on: Oct 25, 2017
 *      Author: seb
 */

#ifndef API_H_
#define API_H_

#include "sapi.h"
#include "apiSD.h"

typedef enum { INICIAL_STATE, RUN_STATE, CONFIGURATION_STATE, } meteoStationMEF_Status_t;

typedef struct sensorSetup {
	uint8_t  name [20];
	uint8_t  id;
	uint8_t  sensorType;
	bool_t   enable;
} sensorSetup_t;

typedef struct sensorData {
	uint8_t  id;
	uint16_t rawValue;
	float    formattedValue;
} sensorData_t;

uint8_t  numberOfSensor;
uint16_t samplingTime;

uint8_t apiReadTemperatureHumdity	(uint16_t * dataTemp, uint16_t * dataHum);
uint8_t apiReadWind					(uint16_t * dataWind);
uint8_t apiReadSensor				(uint16_t * dataTemp, uint16_t * dataHum, uint16_t * dataWind);
uint8_t apiProcessInformation		(uint8_t * destiny);
uint8_t apiWriteSD					(uint8_t * filename, uint8_t * stringData);

void 	BorrarConsola				( void );
void 	RutinaBienvenida			( void );
uint8_t apiSensorNameSetup			( void );
uint8_t apiSensorStateInitilize		( void );
uint8_t apiSensorSetup 				( void );
uint8_t apiSamplingTimeSetup		( void );
uint8_t apiSensorEnable				( void );
uint8_t apiNumberOfSensorSetup		( void );
uint8_t apiSetTime					( rtc_t *rtc);
uint8_t apiRtcInicialize 			( void );
void 	apiTimeStamp				( uint8_t *infoToSave, uint8_t pos );

bool_t  isCaracter					( uint8_t dataByte );
bool_t  isNumber					( uint8_t dataByte );

uint8_t apiDisplayTime				( rtc_t *rtc );
uint8_t apiDisplayDate				( rtc_t *rtc );

#endif /* API_H_ */
