/*
 * api.c
 *
 *  Created on: Oct 25, 2017
 *      Author: seb
 */

#include "sapi.h"
#include "api.h"
#include "apiSD.h"

static void FormatInformationArray	(uint16_t valorAdc, uint8_t *destiny);
static void formatInfoAlmacenar 	( uint8_t *cadenaDestino, uint8_t *cadenaOrigen, uint8_t longitudMax);


uint8_t apiReadTemperatureHumdity(uint16_t * dataTemp, uint16_t * dataHum) {
	uint16_t adcValue1, adcValue2;

	adcValue1 = adcRead(CH1); // temp
	*dataTemp = adcValue1;
	adcValue2 = adcRead(CH2); // hum
	*dataHum = adcValue2;
	return _API_STATE_OK;
}

uint8_t apiReadWind(uint16_t * dataWind) {
	uint16_t adcValue3 = 0;

	adcValue3 = adcRead(CH3); // viento
	*dataWind = adcValue3;

	return _API_STATE_OK;
}

uint8_t apiReadSensor(uint16_t * dataTemp, uint16_t * dataHum, uint16_t * dataWind) {

	apiReadTemperatureHumdity(dataTemp, dataHum);

	apiReadWind(dataWind);

	return _API_STATE_OK;
}

static void FormatInformationArray	(uint16_t valorAdc, uint8_t *destiny){
	destiny[0] = valorAdc/1000 			+ '0';
	destiny[1] = (valorAdc%1000)/100 	+ '0';
	destiny[2] = (valorAdc%100)/10 		+ '0';
	destiny[3] = (valorAdc%10) 			+ '0';
	destiny[4] = '\0';
}

uint8_t apiProcessInformation(uint16_t dataTemp, uint16_t dataHum, uint16_t dataWind, uint8_t * destiny) {
	uint8_t sensorData[5];
	destiny[0]= '\0';
	// Formatea el array InformationArray con el valor leido del potenciometro.
	FormatInformationArray 	(dataTemp, sensorData);
	formatInfoAlmacenar		(destiny,  sensorData, 100);
	FormatInformationArray	(dataHum,  sensorData);
	formatInfoAlmacenar		(destiny,  sensorData, 100);
	FormatInformationArray	(dataWind, sensorData);
	formatInfoAlmacenar		(destiny,  sensorData, 100);

	return _API_STATE_OK;
}

uint8_t apiWriteSD(uint8_t * filename, uint8_t * stringData) {

	if(apiSD_Init() == _API_STATE_ERROR) {
		// error
	} else {
		if(apiSD_Write(filename, stringData) != 0) {
			// error
			gpioWrite( LEDR, ON );
		} else {
			// ok
			gpioWrite( LEDG, ON );
		}
	}

	return _API_STATE_OK;
}


void formatInfoAlmacenar ( uint8_t *cadenaDestino, uint8_t *cadenaOrigen, uint8_t longitudMax) {
	uint16_t i = 0, j=0;

	if ( cadenaDestino [i] != '\0') {
		while ( cadenaDestino[i] != '\n' )
			i++;
		cadenaDestino[i] = ';';
		i++;
	}

	while ( cadenaOrigen[j] != '\0' && i < longitudMax) {
		cadenaDestino[i] = cadenaOrigen[j];
		i++;
		j++;
	}
	cadenaDestino[i]   = '\n';
	cadenaDestino[i+1] = '\r';
	cadenaDestino[i+2] = '\0';
}
