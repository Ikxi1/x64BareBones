/*
 * bits.h
 *
 *  Created on: 13/09/2008
 *      Author: arkanrow
 */

#ifndef BITS_H_
#define BITS_H_

#include "stdint.h"

/*Obtiene n bits de x a partir de p, contando de derecha a izquierda*/
uint8 getbits8(uint8 x, int8 p, int8 n);
uint16 getbits16(uint16 x, int16 p, int16 n);
uint32 getbits32(uint32 x, int32 p, int32 n);
uint64 getbits64(uint64 x, int64 p, int64 n);

/*Pone n bits desde p en x con el valor de los primeros n bits de y, contando
 * derecha a izquierda*/
void setbits8(uint8 * x, int8 p, int8 n, uint8 y);
void setbits16(uint16 * x, uint16 p, uint16 n, uint16 y);
void setbits32(uint32 * x, uint32 p, uint32 n, uint32 y);
void setbits64(uint64 * x, uint64 p, uint64 n, uint64 y);

/*Prints bits to an array allocated by the user*/
void printbits8(int8 dst[8], uint8 x);
void printbits16(int8 dst[16], uint16 x);
void printbits32(int8 dst[32], uint32 x);
void printbits64(int8 dst[64], uint64 x);

#endif /* BITS_H_ */
