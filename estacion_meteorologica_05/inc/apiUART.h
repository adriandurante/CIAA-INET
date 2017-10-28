/*
 * apiUART.h
 *
 *  Created on: 28/10/2017
 *      Author: Adrian
 */

#ifndef APIUART_H_
#define APIUART_H_

uint8_t uartReadString	( uartMap_t uart, uint8_t* receivedString, uint8_t lengthString );
uint8_t uartReadUint16	( uartMap_t uart, uint16_t* receivedNumber );
uint8_t uartReadUint8	( uartMap_t uart, uint8_t* receivedNumber );

#endif /* APIUART_H_ */
