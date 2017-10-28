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

extern uint16_t samplingTime;


/************* APLICACION **************/
int main( void ){

	uint16_t sensorTempValue, sensorHumValue, sensorWindValue;
	uint8_t bufferDataLog[128], dataUart;
	bool_t samplingEnd = FALSE;

	boardConfig();
	uartConfig(UART_USB, UART_BAUDRATE);
	adcConfig(ADC_ENABLE);
	spiConfig(SPI0);
	tickConfig(10, diskTickHook);

	RutinaBienvenida();
	apiSensorSetup ();


	uartWriteString( UART_USB, "\n\r------------------------------------------" );
	uartWriteString( UART_USB, "\n\rComenzo el muestreo. Ingrese 'S' para finalizar" );
	while( samplingEnd == FALSE ) {
		delay(samplingTime);

		apiReadSensor(&sensorTempValue, &sensorHumValue, &sensorWindValue);

		apiProcessInformation(sensorTempValue,
				sensorHumValue,
				sensorWindValue,
				bufferDataLog);

		apiWriteSD(_SYS_CFG_DATALOG_FILENAME, bufferDataLog);

		if( uartReadByte( UART_USB, &dataUart ) ){
			if ( dataUart == 'S' || dataUart == 's') {
				samplingEnd = TRUE;
			}
		}
	}
	uartWriteString( UART_USB, "\n\rFinalizo el muestreo." );
	//Espera que haya una interrupcion y pone el procesador en bajo consumo.
	sleepUntilNextInterrupt();

	return 0;
}
