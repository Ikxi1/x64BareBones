/*
 * segmentDescriptor.h
 *
 *  Created on: 13/09/2008
 *      Author: Eduardo Casal, Luciano Mangiarotti, Rodrigo Rearden
 */

#ifndef SEGMENTDESCRIPTOR_H_
#define SEGMENTDESCRIPTOR_H_

#include "stdint.h"

typedef struct {
    uint16	limitLow;
    uint16	baseLow;
    uint8	baseHigh1;
    uint8	access;
    uint8	attrib;
    uint8	baseHigh2;
} SegmentDescriptor;

/*Access flags*/
/*Type Flags*/
enum {SD_ACCESSED = 0x1, SD_WRITABLE = 0x2, SD_EXPAND_DOWN = 0x4};
enum {/*SD_ACCESSED = 0x1,*/ SD_EXECUTE_READ = 0x2, SD_CONFORMING = 0x4};
enum {SD_DATA = 0x0, SD_CODE = 0x8};

/*S flag, descriptor type*/
enum {SD_CODE_OR_DATA = 0x10};

/*DPL's, descriptor privilege level*/
enum {SD_LV0 = 0x00, SD_LV1 = 0x20, SD_LV2 = 0x40, SD_LV3 = 0x60};

/*P, present*/
enum {SD_SEGMENT_NOT_PRESENT = 0x00, SD_SEGMENT_PRESENT = 0x80};

/*Access getters*/
enum {SD_GET_S = 0x10, SD_GET_LV = 0x60, SD_GET_P = 0x80};

/*Attribute flags*/
/*AVL, available for use by system soft.*/
enum {SD_AVL = 0x10};

///*64-bit code segment (useless here! should always be zero)*/
//enum {L_NO = 0x00, L_YES = 0x20};

/*D/B Default op. size (0=16-bit seg.1=32-bit seg)*/
enum {SD_DB_32 = 0x40};

/*Granularity*/
enum {SD_G_YES = 0x80};

/*Attribute getters*/
enum {SD_GET_AVL = 0x10, SD_GET_L = 0x20, SD_GET_DB = 0x40, SD_GET_G = 0x80};

//Sets the segment to all zero
 void segmentSetZero(SegmentDescriptor * sd);

//Sets the segment offset limit
 void segmentSetLimit(SegmentDescriptor * sd, uint32 limit);

//Gets the segment offset limit
 uint32 segmentGetLimit(SegmentDescriptor sd);

//Sets the segment base address
 void segmentSetBase(SegmentDescriptor * sd, uint32 base);

//Gets the segment base address
 uint32 segmentGetBase(SegmentDescriptor sd);

//Sets the segment type
 void segmentSetType(SegmentDescriptor * sd, uint8 type);

//Gets the segment type
 uint8 segmentGetType(SegmentDescriptor sd, uint8 typeFlag);

//Sets the value of segment access, (Prescense, Descriptor Privilege Level and
//descriptor type)
 void segmentSetAccess(SegmentDescriptor * sd, uint8 access);

//Gets the value of segment access, (Prescense, Descriptor Privilege Level and
//descriptor type)
 uint8 segmentGetAccess(SegmentDescriptor sd, uint8 accessFlag);

//Gets the value of segment attributes (mainly granularity and operation size)
 void segmentSetAttrib(SegmentDescriptor * sd, uint8 attrib);

//Sets the value of segment attributes (mainly granularity and operation size)
 uint8 segmentGetAttrib(SegmentDescriptor sd, uint8 attributeFlag);

void printSegment(SegmentDescriptor sd);

#endif /* SEGMENTDESCRIPTOR_H_ */
