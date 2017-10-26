/************* APLICACION **************/
#include "main.h"
#include "sapi.h"
#include "api.h"

/* configuración de mi aplicación */
#define _SYS_CFG_DATALOG_FILENAME 	"hola.txt"
#define _SYS_CFG_SAMPLINGTIME		(1000)

/************* APLICACION **************/
int main( void ){
	uint16_t sensorTempValue, sensorHumValue, sensorWindValue = 0;
	uint8_t bufferDataLog[128];

	boardConfig();
	uartConfig(UART_USB, 115200);
	adcConfig(ADC_ENABLE);
	spiConfig(SPI0);
	tickConfig(10, diskTickHook);

	while(TRUE) {
		delay(_SYS_CFG_SAMPLINGTIME);

		apiReadSensor(&sensorTempValue, &sensorHumValue, &sensorWindValue);

		apiProcessInformation(sensorTempValue,
				sensorHumValue,
				sensorWindValue,
				bufferDataLog);

		apiWriteSD(_SYS_CFG_DATALOG_FILENAME, bufferDataLog);
	}

	return 0;
}
