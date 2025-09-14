/*
 * segmentSelector.c
 *
 *  Created on: 13/09/2008
 *      Author: Eduardo Casal, Luciano Mangiarotti, Rodrigo Rearden
 */

#include "segmentSelector.h"
#include "bits.h"

/*Sets all segment selector fields to zero*/
void segSelZero(SegmentSelector * ss) {
    *ss = 0;
}

void segSelSet(SegmentSelector * ss, RPL_T RPL, TI_T TI, uint16 index)
{
    segSelSetRPL(ss, RPL);
    segSelSetTI(ss, TI);
    segSelSetIndex(ss, index);
}

/*Sets the segment selector requested privilege level*/
void segSelSetRPL(SegmentSelector * ss, RPL_T rpl) {
    setbits16(ss, 0, 2, rpl);
}

/*Gets the segment selector requested privilege level*/
RPL_T segSelGetRPL(SegmentSelector ss) {
    return getbits16(ss, 0, 2);
}

/*Sets the segment selector table indicator*/
void segSelSetTI(SegmentSelector * ss, TI_T ti) {
    setbits16(ss, 2, 1, ti);
}

/*Sets the segment selector table indicator*/
TI_T segSelGetTI(SegmentSelector ss) {
    return getbits16(ss, 2, 1);
}

/*Sets the segment selector index*/
void segSelSetIndex(SegmentSelector * ss, uint16 index) {
    setbits16(ss, 3, 13, index);
}

/*Gets the segment selector index*/
uint16 segSelGetIndex(SegmentSelector ss) {
    return getbits16(ss, 3, 13);
}
