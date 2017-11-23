/*Se debe definir una interfaz de usuario para interactuar con la estación metereológica que
permita:
1. Habilitar/deshabilitar individualmente los sensores.
2. [opcional] Configurar los datos del RTC
3. [opcional] Configurar el período de muestreo de datos.
Cuando el sistema se inicializa (luego del reset) se debe limpiar la pantalla de la consola con
los siguientes comandos:
 27 -> ESC command
 "[2J" -> Clear screen command
 27 -> ESC command
 "[H" -> Cursor to home command
 Un retardo de 1ms para que permita mostrar nuevos mensajes.
Código de ejemplo para limpiar la pantalla de la consola serie:
uartWriteByte( UART_USB, 27 ); // ESC command
uartWriteString( UART_USB, "[2J" ); // Clear screen command
uartWriteByte( UART_USB, 27 ); // ESC command
uartWriteString( UART_USB, "[H" ); // Cursor to home command
delay(1); // Pequeño delay para que permita mostrar nuevos mensajes
Luego de limpiar la pantalla, el sistema debe mostrar un mensaje de bienvenida que incluya
nombre del proyecto, número de versión, nombre del alumno/desarrollador y las opciones de
configuración que fueron listadas
*/

/************* APLICACION **************/
#include "sapi.h"
#include "api.h"
#include "main.h"


/* configuracion de mi aplicacion */
#define _SYS_CFG_DATALOG_FILENAME 	"datalog.txt"
#define _SYS_CFG_DATALOG_MAX_SIZE	1048576

#define UART_BAUDRATE				(115200)


uint8_t meteoStationMEF_Itialize 	( meteoStationMEF_Status_t *estado );
uint8_t meteoStationMEF_Actualize	( meteoStationMEF_Status_t *estado );

extern uint16_t samplingTime;
delay_t samplingDelay;
volatile bool_t flagTick;

/************* APLICACION **************/
int main( void ){

	meteoStationMEF_Status_t meteoStationMEF_ActualState;
	bool_t samplingEnd = FALSE;
	uint16_t sensorTempValue, sensorHumValue, sensorWindValue;

	boardConfig();
	uartConfig(UART_USB, UART_BAUDRATE);
	adcConfig(ADC_ENABLE);
	spiConfig(SPI0);
	// Configuración SysTick
	tickConfig( 10, diskTickHook );



	meteoStationMEF_Itialize( &meteoStationMEF_ActualState );




	uartWriteString( UART_USB, "\n\r------------------------------------------" );
	uartWriteString( UART_USB, "\n\rComenzo el muestreo. Ingrese 'S' para finalizar" );

	while( samplingEnd == FALSE ) {
		meteoStationMEF_Actualize ( &meteoStationMEF_ActualState );

		if ( esp8266ReadHttpServer() ) {
			gpioWrite (LED2, ON);
			apiReadSensor ( &sensorTempValue, &sensorHumValue, &sensorWindValue );
			apiWifiTransmission ( sensorTempValue, sensorHumValue, sensorWindValue );
			gpioWrite (LED2, OFF);
		}

	}

	return 0;
}



uint8_t meteoStationMEF_Itialize ( meteoStationMEF_Status_t *actualState ) {

	*actualState = INICIAL_STATE;
	return 1;
}


uint8_t meteoStationMEF_Actualize ( meteoStationMEF_Status_t *actualState ) {
	uint16_t sensorTempValue, sensorHumValue, sensorWindValue;
	uint8_t bufferDataLog[128];
	uint32_t fileSize;

	switch ( *actualState ) {
		case INICIAL_STATE:			RutinaBienvenida();
									apiRtcInicialize ();
									apiSensorSetup ();
									apiWifiInitialize ();
									delayConfig(&samplingDelay, samplingTime);
									*actualState = RUN_STATE;
									break;

		case RUN_STATE:				if ( delayRead(&samplingDelay) ) {
										apiReadSensor(&sensorTempValue, &sensorHumValue, &sensorWindValue);
										apiProcessInformation( bufferDataLog );
										apiWriteSD(_SYS_CFG_DATALOG_FILENAME, bufferDataLog, &fileSize);
										gpioToggle (LED1);

										if ( _SYS_CFG_DATALOG_MAX_SIZE < fileSize )
											*actualState = TRANSMISSION_STATE;
									}

									break;
		case TRANSMISSION_STATE:	gpioWrite (LED3, ON);
									// Acá debería ir la rutina de transmision
									// que envía los datos almacenados al servidor.
									gpioWrite (LED3, OFF);
									break;
		case CONFIGURATION_STATE:	break;
		default: 					*actualState = INICIAL_STATE;
	}
	return 1;
}
