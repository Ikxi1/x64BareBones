#include <page.h>
#include <stdint.h>
#include <bits.h>
#include <types.h>

#define MAP_SIZE 32768 /**< Size of the memory (4GB/(16KB*8))*/

typedef uint64 int_t;  /**< WORD is 16bits long. */

static uint8 MemMap[MAP_SIZE];
static uint64 MemSize;
static int_t MemStart;

void startPaging( uint64 memsize, void * memstart )
{
    MemSize = memsize;
    MemStart = (uint64) memstart;
}

void * page(void)
{
    static uint64 i;

    for( i = 0; i < (MemSize/MEM_BLOCK); i++ )
    {
        if( !getbits8(MemMap[i/8],i%8,1) )
        {
            setbits8(&(MemMap[i/8]),i%8,1,1);
            //TODO
            return (void*)(MemStart+i*MEM_BLOCK);
        }
    }

    return NULL;
}

void unpage( void * p )
{
    static int_t mempos;
    static uint64 i;

    //TODO
    mempos = (int_t) p;

    mempos -= MemStart;
    i = mempos/MEM_BLOCK;

    if(mempos % MEM_BLOCK != 0)
    {
        /** ACA SE ROMPE TODO!!! */
        /** Se esta liberando una direccion no valida. */
    }

    if( !getbits8(MemMap[i/8], i % 8, 1) )
    {
        /** DOUBLE FREE EXCEPTION. **/
    }

    /** Todo ok, dejo libre la zona de memoria. */
    setbits8(&(MemMap[i/8]), i % 8, 1, 0);
}
