/*
 * apiUART.c
 *
 *  Created on: 28/10/2017
 *      Author: Adrian
 */


#include "sapi.h"
#include "api.h"
#include "apiSD.h"


uint8_t uartReadString( uartMap_t uart, uint8_t* receivedString, uint8_t lengthString ) {
	bool_t stringEnd = FALSE;
	uint8_t caracterNumber = 0, dataUartByte;

	while (stringEnd == FALSE) {
		if( uartReadByte( uart, &dataUartByte )) {
			if ( isNumber(dataUartByte) || isCaracter (dataUartByte) ) {

				receivedString[caracterNumber] = dataUartByte;
				uartWriteByte( uart, dataUartByte);
				caracterNumber++;
			}
			if ( dataUartByte == '\r' || dataUartByte == '\n' || caracterNumber == lengthString-1) {
				stringEnd = TRUE;
				receivedString[caracterNumber] = '\0';
			}
		}
	}
	return caracterNumber;
}



uint8_t uartReadUint16	( uartMap_t uart, uint16_t* receivedNumber ) {

	uint8_t caracterNumber = 0, dataUartByte, dataUartString[11];
	bool_t numberEntry = FALSE;

	while (numberEntry == FALSE) {
		if( uartReadByte( uart, &dataUartByte )) {
			if ( caracterNumber != 0 || dataUartByte != '0' )			// Controla que el dato no empiece con un 0.
				if ( isNumber ( dataUartByte ) ) {	// Controla que sea un número de 0 a 9
					dataUartString[caracterNumber] = dataUartByte;		// y lo almacena en el array.
					uartWriteByte( uart, dataUartByte);					// Echo del caracter ingresado.
					caracterNumber++;
				}
			if ( dataUartByte == 13 || caracterNumber == 10) {
				numberEntry = TRUE;
				dataUartString[caracterNumber] = '\0';
			}
		}
	}

	*receivedNumber = atol ( dataUartString );
	return _API_STATE_OK;
}

uint8_t uartReadUint8	( uartMap_t uart, uint8_t* receivedNumber ) {

	uint8_t caracterNumber = 0, dataUartByte, dataUartString[6];
	bool_t numberEntry = FALSE;

	while (numberEntry == FALSE) {
		if( uartReadByte( uart, &dataUartByte )) {
			if ( caracterNumber != 0 || dataUartByte != '0' )			// Controla que el dato no empiece con un 0.
				if ( isNumber ( dataUartByte ) ) {	// Controla que sea un número de 0 a 9
					dataUartString[caracterNumber] = dataUartByte;		// y lo almacena en el array.
					uartWriteByte( uart, dataUartByte);					// Echo del caracter ingresado.
					caracterNumber++;
				}
			if ( dataUartByte == 13 || caracterNumber == 5) {
				numberEntry = TRUE;
				dataUartString[caracterNumber] = '\0';
			}
		}
	}

	*receivedNumber = atol ( dataUartString );
	return _API_STATE_OK;
}


