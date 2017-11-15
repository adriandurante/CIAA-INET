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


/* configuraciÃ³n de mi aplicaciÃ³n */
#define _SYS_CFG_DATALOG_FILENAME 	"datalog.txt"
//#define _SYS_CFG_SAMPLINGTIME		(1000)
#define UART_BAUDRATE				(115200)


uint8_t meteoStationMEF_Itialize 	( meteoStationMEF_Status_t *estado );
uint8_t meteoStationMEF_Actualize	( meteoStationMEF_Status_t *estado );

extern uint16_t samplingTime;


/************* APLICACION **************/
int main( void ){

	meteoStationMEF_Status_t meteoStationMEF_ActualState;
	bool_t samplingEnd = FALSE;
	uint8_t dataUart;

	boardConfig();
	uartConfig(UART_USB, UART_BAUDRATE);
	adcConfig(ADC_ENABLE);
	spiConfig(SPI0);
	tickConfig(10, diskTickHook);



	meteoStationMEF_Itialize( &meteoStationMEF_ActualState );




	uartWriteString( UART_USB, "\n\r------------------------------------------" );
	uartWriteString( UART_USB, "\n\rComenzo el muestreo. Ingrese 'S' para finalizar" );



	while( samplingEnd == FALSE ) {

		meteoStationMEF_Actualize ( &meteoStationMEF_ActualState );
		//if(ON == flagTick) {
		//	flagTick = OFF;

			delay(samplingTime);
			if( uartReadByte( UART_USB, &dataUart ) ){
				if ( dataUart == 'S' || dataUart == 's') {
					samplingEnd = TRUE;
				}
			}
		//}
	}
	uartWriteString( UART_USB, "\n\rFinalizo el muestreo." );
	//Espera que haya una interrupcion y pone el procesador en bajo consumo.
	sleepUntilNextInterrupt();

	return 0;
}



uint8_t meteoStationMEF_Itialize ( meteoStationMEF_Status_t *actualState ) {

	*actualState = INICIAL_STATE;
}


uint8_t meteoStationMEF_Actualize ( meteoStationMEF_Status_t *actualState ) {
	uint16_t sensorTempValue, sensorHumValue, sensorWindValue;
	uint8_t bufferDataLog[128];


	switch ( *actualState ) {
		case INICIAL_STATE:			RutinaBienvenida();
									apiRtcInicialize ();
									apiSensorSetup ();
									*actualState = RUN_STATE;
									break;

		case RUN_STATE:				apiReadSensor(&sensorTempValue, &sensorHumValue, &sensorWindValue);
									apiProcessInformation( bufferDataLog );
									apiWriteSD(_SYS_CFG_DATALOG_FILENAME, bufferDataLog);

									break;

		case CONFIGURATION_STATE:	break;
		default: 					*actualState = INICIAL_STATE;
	}
}
