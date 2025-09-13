#include <stdint.h>
#include <types.h>
#include <syscalls.h>
#include <vt.h>
#include <libc.h>

#include <vtprintf.h>

#define	MAX_BUF	200
#define NALLOC  1024   /* minimum #units to request */

typedef long Align;     			/* for alignment to long boundary */

union header
{          							/* block header */
    struct {
         union header * ptr; 		/* next block if on free list */
         unsigned size;     		/* size of this block */
    } s;
    Align x;            			/* force alignment of blocks */
};

typedef union header Header;

typedef struct
{
	Header base;        	/* empty list to get started */
	Header * freep;      	/* start of free list */
	uint8 a;			/**/
	char buffer[MAX_BUF];	/* Buffer de entrada */
	int index;
	int sizeBuffer;
	void * freeZone;
} heapHead;

static void sIntPrintf (void * vaArgs, char * buffer, int * buffIndex);
static void sStrPrintf (void * vaArgs, char * buffer, int * buffIndex);
static void sHexPrintf (void * vaArgs, char * buffer, int * buffIndex);
static void sIntPrintfWrapp (int arg, char * buffer, int * buffIndex);
static void sHexPrintfWrapp (int arg, char * buffer, int * buffIndex);

char cgetchar (void)
{
	char rta;
	heapHead * hh = (heapHead*)getHeap();
	if (hh->index < hh->sizeBuffer)
	{
		rta = hh->buffer[hh->index];
		hh->index++;
	}
	else	/* index == sizeBuffer */
	{
		hh->sizeBuffer = vtRead((void*)hh->buffer, MAX_BUF);	/* Leo caracteres de entrada */
		if(hh->sizeBuffer > 0)
		{
			hh->index = 1;
			rta = hh->buffer[0];
		}
		else
			return 0;
	}

	return rta;
}

/* free: put block ap in free list */
void free(void *ap)
{
//    Header *bp, *p;
//    bp = (Header *)ap - 1;    /* point to block header */
//    for (p = ((heapHead*)getHeap())->freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
//         if (p >= p->s.ptr && (bp > p || bp < p->s.ptr))
//             break; /* freed block at start or end of arena */
//    if (bp + bp->s.size == p->s.ptr) {    /* join to upper nbr */
//        bp->s.size += p->s.ptr->s.size;
//        bp->s.ptr = p->s.ptr->s.ptr;
//    } else
//        bp->s.ptr = p->s.ptr;
//    if (p + p->s.size == bp) {            /* join to lower nbr */
//        p->s.size += bp->s.size;
//        p->s.ptr = bp->s.ptr;
//    } else
//        p->s.ptr = bp;
//    ((heapHead *)getHeap())->freep = p;
}

void * malloc(unsigned nbytes)
{
//    Header * p, *prevp, *freeptr, *prevptr;
//    unsigned nunits;
//
//    nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
//    if ((prevp = ((heapHead*)getHeap())->freep) == NULL) {    /* no free list yet */
//        ((heapHead*)getHeap())->base.s.ptr = freeptr = prevptr = &(((heapHead*)getHeap())->base);
//        ((heapHead*)getHeap())->base.s.size = 0;
//    }
//    for (p = prevp->s.ptr; ; prevp = p, p = p->s.ptr) {
//        if (p->s.size >= nunits) { /* big enough */
//            if (p->s.size == nunits) /* exactly */
//                prevp->s.ptr = p->s.ptr;
//            else {               /* allocate tail end */
//                p->s.size -= nunits;
//                p += p->s.size;
//                p->s.size = nunits;
//            }
//            ((heapHead*)getHeap())->freep = prevp;
//            return (void *)(p+1);
//        }
//	    if (p == ((heapHead*)getHeap())->freep) /* wrapped around free list */
//	    {
//		    if (((heapHead*)getHeap())->a == 0)
//		    {
//		    	((heapHead*)getHeap())->a++;
//		    	p = (Header*)(((heapHead*)getHeap()) + 1);
//		    }
//		    else
//		    	return NULL;
//	    }
//    }
	void * ret;
	heapHead * hh = (heapHead*)getHeap();
	if (hh->freeZone == NULL)
		hh->freeZone = (char*)hh + sizeof(heapHead);

	ret = hh->freeZone;
	hh->freeZone = (char*)hh->freeZone + nbytes;

	return ret;
}

int cprintf(const char * fmtStr, ...)
{
	void * vaArgs = ((uint32*)&fmtStr + 1);
	int i, buffIndex = 0, n = 0;
	char buffer[MAX_BUF];

	for (i = 0; fmtStr[i] != '\0'; i++)
	{
		if (fmtStr[i] == '%')
		{
			switch (fmtStr[++i])
			{
				case 'd':
					sIntPrintf(vaArgs, buffer, &buffIndex);
					break;
				case 's':
					sStrPrintf(vaArgs, buffer, &buffIndex);
					break;
				case 'x':
					sHexPrintf(vaArgs, buffer, &buffIndex);
					break;
				default:
					vtWrite (buffer, buffIndex);
					return n;
			}
			vaArgs += sizeof(int32);
			n++;		/* Cantidad de argumentos leidos */
		}
		else
			buffer[buffIndex++] = fmtStr[i];

		if (buffIndex == MAX_BUF)
		{
			vtWrite (buffer, buffIndex);
			buffIndex = 0;
		}
	}

	if (buffIndex > 0)
		vtWrite (buffer, buffIndex);

	return n;
}

static void sIntPrintf (void * vaArgs, char * buffer, int * buffIndex)
{
	if (vaArgs != NULL)
	{
		int arg = *(int32*)vaArgs;

		sIntPrintfWrapp (arg, buffer, buffIndex);
	}
}

static void sIntPrintfWrapp (int arg, char * buffer, int * buffIndex)
{
	if (arg >= 10)
		sIntPrintfWrapp (arg / 10, buffer, buffIndex);

	if (*buffIndex == MAX_BUF)
	{
		vtWrite (buffer, *buffIndex);
		buffIndex = 0;
	}

	buffer[(*buffIndex)++] = arg % 10 + '0';
}

static void sStrPrintf (void * vaArgs, char * buffer, int * buffIndex)
{
	if (vaArgs != NULL)
	{
		char * arg = *(char**)vaArgs;
		int i;

		for (i = 0; arg[i] != '\0'; i++)
		{
			if (*buffIndex == MAX_BUF)
			{
				vtWrite (buffer, *buffIndex);
				buffIndex = 0;
			}
			buffer[(*buffIndex)++] = arg[i];
		}
	}
}

static void sHexPrintf (void * vaArgs, char * buffer, int * buffIndex)
{
	if (vaArgs != NULL)
	{
		int arg = *(int32*)vaArgs;

		sHexPrintfWrapp (arg, buffer, buffIndex);
	}
}

static void sHexPrintfWrapp (int arg, char * buffer, int * buffIndex)
{
	if (arg >= 16)
		sIntPrintfWrapp (arg / 16, buffer, buffIndex);

	if (*buffIndex == MAX_BUF)
	{
		vtWrite (buffer, *buffIndex);
		buffIndex = 0;
	}

	if (arg % 16 > 9)
		buffer[(*buffIndex)++] = arg % 16 - 10 + 'A';
	else
		buffer[(*buffIndex)++] = arg % 16 + '0';
}

int atoi (char * s)
{
	int rta = 0;
	
	while (*s)
	{
		rta *= 10;
		rta += *s - '0';
		s++;
	}
	
	return rta;
}
