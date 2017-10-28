/*
 * api.c
 *
 *  Created on: Oct 25, 2017
 *      Author: seb
 */

#include "sapi.h"
#include "api.h"
#include "apiSD.h"

static void FormatInformationArray(uint16_t valor, uint8_t * destiny, uint8_t pos);
sensorData  sensorDataArray[3];
sensorSetup sensorSetupArray[3];


CONSOLE_PRINT_ENABLE

uint8_t apiReadTemperatureHumdity(uint16_t * dataTemp, uint16_t * dataHum) {
	uint16_t adcValue1, adcValue2;

	adcValue1 = adcRead(CH1); // temp
	(*dataTemp) = adcValue1;

	adcValue2 = adcRead(CH2); // hum
	(*dataHum) = adcValue2;

	return _API_STATE_OK;
}

uint8_t apiReadWind(uint16_t * dataWind) {
	uint16_t adcValue3 = 0;

	adcValue3 = adcRead(CH3); // viento
	(*dataWind) = adcValue3;

	return _API_STATE_OK;
}

uint8_t apiReadSensor(uint16_t * dataTemp, uint16_t * dataHum, uint16_t * dataWind) {

	apiReadTemperatureHumdity(dataTemp, dataHum);

	apiReadWind(dataWind);

	sensorDataArray[0].rawValue = *dataTemp;
	sensorDataArray[1].rawValue = *dataHum;
	sensorDataArray[2].rawValue = *dataWind;

	return _API_STATE_OK;
}

static void FormatInformationArray(uint16_t valor, uint8_t * destiny, uint8_t pos){
	destiny[pos] = valor/1000 		+ '0';
	pos++;
	destiny[pos] = (valor%1000)/100 + '0';
	pos++;
	destiny[pos] = (valor%100)/10 	+ '0';
	pos++;
	destiny[pos] = (valor%10) 		+ '0';
}

uint8_t apiProcessInformation(uint16_t dataTemp, uint16_t dataHum, uint16_t dataWind, uint8_t * destiny) {
	uint8_t pos = 0, i;
	/*
	 * posiciones	= 0123 4 5678 9 0123 4 5  6  7
	 * string 		= XXXX ; YYYY ; ZZZZ ; \r \n \0
	 */
	for( i = 0 ; i < numberOfSensor ; i++) {
		if(sensorSetupArray[i].enable) {
			FormatInformationArray(sensorDataArray[i].rawValue, destiny, pos);
			pos += 4;
		}
		destiny[pos] = ';';
		pos++;
	}

	destiny[pos] = '\r';
	pos++;

	destiny[pos] = '\n';
	pos++;

	destiny[pos] = '\0';
	//pos++;


	return _API_STATE_OK;
}

uint8_t apiWriteSD(uint8_t * filename, uint8_t * stringData) {

	if(apiSD_Init() == _API_STATE_ERROR) {
		// error
	} else {
		if(apiSD_Write(filename, stringData) == _API_STATE_ERROR) {
			// error
			gpioWrite( LEDR, ON );
		} else {
			// ok
			gpioWrite( LEDG, ON );
		}
	}
	return _API_STATE_OK;
}



uint8_t apiSensorSetup ( void ){

	apiNumberOfSensorSetup ();
	apiSensorStateInitilize ();
    apiSamplingTimeSetup ();
    apiSensorNameSetup ();
    apiSensorEnable ();

    return _API_STATE_OK;
}



uint8_t apiSensorStateInitilize (void) {

	uint8_t sensorNumber;

	for( sensorNumber = 0 ; sensorNumber < numberOfSensor ; sensorNumber++) {
		sensorSetupArray[sensorNumber].enable = OFF;
		sensorSetupArray[sensorNumber].id = sensorNumber+1;
	}
	return _API_STATE_OK;
}


uint8_t apiSensorNameSetup (void) {
	uint8_t sensorNumber;

	consolePrintString("\n\r\n\rIngrese el nombre de los sensores (maximo 20 caracteres)");
	for( sensorNumber = 0 ; sensorNumber < numberOfSensor ; sensorNumber++) {
		consolePrintString("\n\rSensor ");
		consolePrintInt(sensorSetupArray[sensorNumber].id);
		consolePrintString(": ");

		uartReadString ( UART_USB, sensorSetupArray[sensorNumber].name, 20 );
	}
	return _API_STATE_OK;
}


uint8_t apiSamplingTimeSetup ( void ) {

	uartWriteString( UART_USB, "\n\rIngrese el tiempo de muestreo de datos: ");

	uartReadInt	( UART_USB, &samplingTime );

	return _API_STATE_OK;
}


uint8_t apiNumberOfSensorSetup ( void ) {

	uartWriteString( UART_USB, "\n\rIngrese la cantidad de sensores: ");

	uartReadInt	( UART_USB, &numberOfSensor );

	return _API_STATE_OK;
}


/* *******************************************************************
 * Función: BorrarConsola
 * Autor: Adrián Durante
 * Fecha: 2017/09/27
 * Descripción: Borra la consola y posiciona el cursor al comienzo.
 * No tiene argumentos
 * No devuelve valores
 *********************************************************************/
void BorrarConsola ( void ) {
    uartWriteByte( UART_USB, 27);
    uartWriteString( UART_USB, "[2J" );
    uartWriteByte( UART_USB, 27 );
    uartWriteString( UART_USB, "[H" );
}


/* *******************************************************************
 * Función: RutinaBienvenida
 * Autor: Adrián Durante
 * Fecha: 2017/10/12
 * Descripción: Borra la consola, escribe el mensaje de bienvenida y da
 *              una breve explicación de la utilización del programa.
 * No tiene argumentos
 * No devuelve valores
 *********************************************************************/
void RutinaBienvenida(void) {
	BorrarConsola();
	consolePrintlnString("==============================================\n\r");
	consolePrintlnString("\n\rEstacion Meteorologica - CIAA INET - V1.0");
    consolePrintlnString("\n\rAdrian Durante");
	consolePrintlnString("\n\r26/10/2017");
	consolePrintlnString("\n\r");
	consolePrintlnString("==============================================\n\r");
	delay (1);
}


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



uint8_t uartReadInt	( uartMap_t uart, uint16_t* receivedNumber ) {

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



uint8_t apiSensorEnable	( void ) {

	bool_t sensorSetupEnd = FALSE;
	uint8_t dataUart, sensorNumber;

	consolePrintString("\n\r--------------------------------------------------------------------------");
    consolePrintString("\n\r\n\rPresione la tecla indicada para activar o desactivar cada sensor: \n\r");
    consolePrintString("\n\rActivar sensores");
    consolePrintlnString("\n\rIngrese '0' para finalizar la configuración y comenzar el muestreo. \n\r");
    for( sensorNumber = 0 ; sensorNumber < numberOfSensor ; sensorNumber++) {
	    uartWriteByte( UART_USB, sensorSetupArray[sensorNumber].name[0]);
	    consolePrintString(" ");
	    consolePrintlnString(sensorSetupArray[sensorNumber].name);
    }

    while (sensorSetupEnd == FALSE){
	   //Esperar que lleguen datos por consola
	   if( uartReadByte( UART_USB, &dataUart ) ){
		   // Chequea que el valor ingresado por el usuario sea valido.
		   //if ( dataUart >= 'a' && dataUart <= 'z')
		   for( sensorNumber = 0 ; sensorNumber < numberOfSensor ; sensorNumber++) {
			   if (dataUart == sensorSetupArray[sensorNumber].name[0]) {
				   sensorSetupArray[sensorNumber].enable = !sensorSetupArray[sensorNumber].enable;
				   consolePrintString("Sensor de ");
				   consolePrintString(sensorSetupArray[sensorNumber].name);
				   consolePrintString(": ");
				   consolePrintlnInt( sensorSetupArray[sensorNumber].enable );
			   }
		   }
		   if (dataUart == '0')
			   sensorSetupEnd = TRUE;
	   }
    }
    return _API_STATE_OK;
}

bool_t isCaracter (uint8_t dataByte) {
	bool_t dataOK = FALSE;
	if ( (dataByte >= 'a' && dataByte <= 'z') || (dataByte >= 'A' && dataByte <= 'Z')) {
		dataOK = TRUE;
	}
	return dataOK;
}

bool_t  isNumber ( uint8_t dataByte ) {
	bool_t dataOK = FALSE;
	if ( dataByte >= '0' && dataByte <= '9' ) {
		dataOK = TRUE;
	}
	return dataOK;
}
