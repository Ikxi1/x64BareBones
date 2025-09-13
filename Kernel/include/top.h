#ifndef TOP_H_
#define TOP_H_

#define	MAX_PROC	50

typedef struct
{
	uint32 processPID;				/* PID del proceso */ 
	uint32 processor;				/* Porcentaje de procesador */
	uint32 state;					/* Estado del proceso */
	uint32 vt;						/* Virtual Terminal en la que se encuentra el proceso */
	uint8 priority;					/* Prioridad del proceso */
	uint8 fixedPriority;			/* Valor booleano que indica si el proceso tiene o no prioridad fija */
} tProcessInfo;

typedef tProcessInfo * vecProcessInfo;

int pfTop (vecProcessInfo processes, int len);
int top (vecProcessInfo processes, int len);

#endif /*TOP_H_*/
