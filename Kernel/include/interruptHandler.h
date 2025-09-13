/*
 * interruptionHandler.h
 *
 *  Created on: 25/09/2008
 *      Author: arkanrow
 */

#ifndef INTERRUPTIONHANDLER_H_
#define INTERRUPTIONHANDLER_H_

void setupInterrupts(void);
void int_08_hand(void);
void int_09_hand(void);
void _Sti(void);
void _Cli(void);
void PIC1mask(uint8 mask);
void PIC2mask(uint8 mask);

#endif /* INTERRUPTIONHANDLER_H_ */
