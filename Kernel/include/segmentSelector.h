/*
 * segmentSelector.h
 *
 *  Created on: 13/09/2008
 *      Author: Eduardo Casal, Luciano Mangiarotti, Rodrigo Rearden
 */

#ifndef SEGMENTSELECTOR_H_
#define SEGMENTSELECTOR_H_

#include "stdint.h"

/*The segmentSelector type definition*/
typedef uint16 SegmentSelector;

/*Privilege levels*/
typedef enum {SS_LV0=0x0, SS_LV1, SS_LV2, SS_LV3} RPL_T;
#define RPL_SMASK 0xFFFC
#define RPL_GMASK 0x0003
#define RPL_OFFS 0

/*Table indicator posible values*/
typedef enum {SS_GDT=0, SS_LDT} TI_T;
#define TI_MASK 0xFFFB
#define TI_OFFS 2

/*Index*/
#define INDEX_MASK 0x7
#define INDEX_OFFS 3

/*Sets all segment selector fields to zero*/
 void segSelZero(SegmentSelector * ss);

/*Sets every segment selector fields*/
 void segSelSet(SegmentSelector * ss, RPL_T RPL, TI_T TI, uint16 index);

/*Sets the segment selector requested privilege level*/
 void segSelSetRPL(SegmentSelector * ss, RPL_T rpl);

/*Gets the segment selector requested privilege level*/
 RPL_T segSelGetRPL(SegmentSelector ss);

/*Sets the segment selector table indicator*/
 void segSelSetTI(SegmentSelector * ss, TI_T ti);

/*Sets the segment selector table indicator*/
 TI_T segSelGetTI(SegmentSelector ss);

/*Sets the segment selector index*/
 void segSelSetIndex(SegmentSelector * ss, uint16 index);

/*Gets the segment selector index*/
 uint16 segSelGetIndex(SegmentSelector ss);

/*Prints the segment on screen*/
void printSegmentSelector(SegmentSelector ss);

#endif /* SEGMENTSELECTOR_H_ */
