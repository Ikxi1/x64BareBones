/*
 * segmentDescriptor.c
 *
 *  Created on: 17/09/2008
 *      Author: arkanrow
 */
#include <types.h>
#include <stdint.h>
#include <bits.h>
#include <segmentDescriptor.h>


void segmentSetZero(SegmentDescriptor * sd) {
    sd->access = 0;
    sd->attrib = 0;
    sd->baseHigh1 = 0;
    sd->baseHigh2 = 0;
    sd->baseLow = 0;
    sd->limitLow = 0;
}

void segmentSetLimit(SegmentDescriptor * sd, uint32 limit) {
    setbits16(&sd->limitLow, 0, 16, limit);
    setbits8(&sd->attrib, 0, 4, getbits32(limit, 16, 4));
}

uint32 segmentGetLimit(SegmentDescriptor sd) {
    uint32 ret = 0;

    setbits32(&ret, 0, 16, sd.limitLow);
    setbits32(&ret, 16, 4, sd.attrib);
    return ret;
}

void segmentSetBase(SegmentDescriptor * sd, uint32 base) {
    setbits16(&sd->baseLow, 0, 16, base);
    setbits8(&sd->baseHigh1, 0, 8, getbits32(base, 16, 8));
    setbits8(&sd->baseHigh2, 0, 8, getbits32(base, 24, 8));
}

uint32 segmentGetBase(SegmentDescriptor sd) {
    uint32 ret = 0;

    setbits32(&ret, 0, 16, getbits16(sd.baseLow, 0, 16));
    setbits32(&ret, 16, 24, getbits8(sd.baseHigh1, 0, 8));
    setbits32(&ret, 24, 8, getbits8(sd.baseHigh2, 0, 8));
    return ret;
}

void segmentSetType(SegmentDescriptor * sd, uint8 type) {
    sd->access = (sd->access & 0xF0) | (type & 0x0F);
}

uint8 segmentGetType(SegmentDescriptor sd, uint8 typeFlag) {
    return (typeFlag & sd.access) & 0x0F;
}

void segmentSetAccess(SegmentDescriptor * sd, uint8 access) {
    sd->access = (sd->access & 0x0F) | (access & 0xF0);
}

uint8 segmentGetAccess(SegmentDescriptor sd, uint8 accessFlag) {
    return (accessFlag & sd.access) & 0xF0;
}

void segmentSetAttrib(SegmentDescriptor * sd, uint8 attrib) {
    sd->attrib = (sd->attrib & 0x0F) | (attrib & 0xF0);
}

uint8 segmentGetAttrib(SegmentDescriptor sd, uint8 attributeFlag) {
    return (attributeFlag & sd.attrib) & 0xF0;
}
