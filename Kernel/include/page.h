#ifndef PAGE_H_
#define PAGE_H_

#define MEM_BLOCK 16384	/**< 16K */

#include <stdint.h>

/** Starts paging memory. */
void startPaging( uint32 memsize, void * memstart );

/** Returns a page. */
void * page( void );

void unpage( void * p );

#endif /*PAGE_H_*/
