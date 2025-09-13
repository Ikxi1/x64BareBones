#include <scheduler.h>
#include <interruptHandler.h>
#include <types.h>
#include <page.h>
#include <process.h>
#include <shield.h>

#ifdef SCHED_PRIORITY

#ifdef TESTING
	void _Sti(void) {}
#endif

#define PROCESS_LOWER_LIMIT	10		/**< Minimum number of processes per queue. */
#define RUN_THRESHOLD		20		/**< Number of full runs before demotion. */
#define PRIORITY_LEVELS		20		/**< Number of priority Queues. */
#define MAX_PROC			30		/**< Maximum ammount of processes per queue level. */

//static uint32 CurrentProcess = 0;

typedef struct SchedControl {
	SchedProcess queues[PRIORITY_LEVELS][MAX_PROC];	/**< One queue per level. */
	uint32 maxProc;									/**< Max ammount of processes a queue can handle. */
	uint32 currentProcPID;							/**< Current running process. */
	uint8 currentQueue;								/**< Current's process queue. */
	uint32 iteratorProc;							/**< Last Iterated Process. */
	uint8 iteratorQueue;							/**< Last Iterated Queue. */
	SchedProcess * currentProcess;					/**< Current process handle. */
} SchedControl;

static SchedControl * Controller;

static SchedProcess * getProcFromQueue(uint32 pid, uint8 queue);

static uint8 removeProcFromQueue(uint32 pid, uint8 queue);

static uint8 insertProcInQueue(uint32 pid, uint8 queue, State s);

static void shiftQueue(uint8 queue);

uint32 getCurrentRunningProc( void );

uint32 getCurrentRunningProcFromQueue( uint8 queue );


void setupScheduler()
{
	uint32 i, j;
	SchedProcess zeroP = {0, 0, SCHED_FREE, 0, 0};

	Controller = (SchedControl *)page();


	Controller->maxProc = MAX_PROC;

	/** Inicializo los datos. */
	for(i = 0; i < PRIORITY_LEVELS; i++)
		for (j=0; j<Controller->maxProc; j++)
			Controller->queues[i][j] = zeroP;

	/** Inicializo el resto de los valores. */
	Controller->currentProcPID = 0;
	Controller->currentProcess = Controller->queues[PRIORITY_LEVELS-1];
	Controller->currentQueue = PRIORITY_LEVELS-1;
	Controller->iteratorProc = Controller->iteratorQueue = 0;
}

void startScheduler()
{
	_Sti();
}

uint32 schedule()
{
	uint32 i;
	SchedProcess * current;
	SchedProcess * next = NULL;
	uint8 running;

	/** Get current process out of the scheduler. */
	current = getProcFromQueue(Controller->currentProcPID,Controller->currentQueue);

	/** Process was found. */
	if( current != NULL )
	{
		/** Check if process consumed full timeslot. */
		if( current->state != SCHED_RUNNING )
			current->runs = 0;
		else
		{
			shiftQueue(Controller->currentQueue);
			current->runs++;
			current->state = SCHED_READY;
		}

		/** Lower the process priority if limit reached. */
		if( current->runs > RUN_THRESHOLD )
		{
			process * proc = getProcess(current->PID);
			if(proc != NULL && !proc->fixedPriority && Controller->currentQueue<PRIORITY_LEVELS-1)
			{
				/** Lower. */
				uint32 currentPID = current->PID;
				State currentState = current->state;

				removeProcFromQueue(currentPID,Controller->currentQueue);
				insertProcInQueue(currentPID,Controller->currentQueue+1,currentState);
			}
		}
	}

	running = getCurrentRunningProc();

	/** Get next process. */
	for(i = 0; i < PRIORITY_LEVELS; i++)
	{
		uint32 nextProc = Controller->queues[i][0].next;

		while(Controller->queues[i][nextProc].state != SCHED_READY && nextProc != 0)
			if(Controller->queues[i][nextProc].state == SCHED_DEAD)
			{
				uint32 currentPID = Controller->queues[i][nextProc].PID;

				nextProc = Controller->queues[i][nextProc].next;
				endProcess(currentPID);
				removeProcFromQueue(currentPID,i);
			}
			else
				nextProc = Controller->queues[i][nextProc].next;

		if( nextProc == 0 )
			continue;

		next = &(Controller->queues[i][nextProc]);
		break;
	}

	/** Found the process or will run the lowest priority WAIT. */
	if( next == NULL )
	{
		/** If current process is also the WAIT process, do nothing. */
		if( Controller->currentProcPID == 0 )
			return 0;

		next = Controller->queues[PRIORITY_LEVELS-1];	//Process ZERO at last queue.
	}

	/** New process will take its place. */
	Controller->currentProcPID = next->PID;
	Controller->currentProcess = next;
	Controller->currentQueue = i;

	if(next->PID != 0)
		next->state = SCHED_RUNNING;

	next->usage = 100/running;
	return next->PID;
}

static void shiftQueue(uint8 queue)
{
	uint32 last, current, next;

	if(queue >= PRIORITY_LEVELS)
		return;

	/**
	 * 1- Set first as current first's next.
	 * 2- Search for last.
	 * 3- Set current first as last's next.
	 */
	last = Controller->queues[queue][0].next;

	current = next = Controller->queues[queue][last].next;

	if(current == 0)
		/** There is only one process in the queue, do nothing. */
		return;

	Controller->queues[queue][0].next = current;

	do
	{
		current = next;
		next = Controller->queues[queue][current].next;
	}while(next != 0);

	Controller->queues[queue][current].next = last;
	Controller->queues[queue][last].next = 0;
}


static SchedProcess * getProcFromQueue(uint32 pid, uint8 queue)
{
	int i;

	if(queue >= PRIORITY_LEVELS)
		return NULL;

	if(pid == 0)
		return Controller->queues[PRIORITY_LEVELS-1];

	for(i=0; i < Controller->maxProc; i++)
		if(Controller->queues[queue][i].PID == pid)
			return &(Controller->queues[queue][i]);


	return NULL;
}


static uint8 removeProcFromQueue(uint32 pid, uint8 queue)
{
	int prev = 0, next=Controller->queues[queue][0].next;

	while(next != 0)
		if(Controller->queues[queue][next].PID == pid)
		{
			Controller->queues[queue][prev].next = Controller->queues[queue][next].next;
			Controller->queues[queue][next].PID = Controller->queues[queue][next].runs =
				Controller->queues[queue][next].next = 0;
			Controller->queues[queue][next].state = SCHED_FREE;
			return TRUE;
		}
		else
		{
			prev = next;
			next = Controller->queues[queue][next].next;
		}

	return FALSE;
}


static uint8 insertProcInQueue(uint32 pid, uint8 queue, State s)
{
	int prev = 0, i=Controller->queues[queue][0].next, jumps = 1, j;

	while(i != 0)
	{
		prev = i;
		i = Controller->queues[queue][i].next;
		jumps++;
	}


	if(jumps < Controller->maxProc-1)
	{
		/** There's space available. Look for first slot. */
		j = 1 ;
		for( ; ; j++)
		{
			if(Controller->queues[queue][j].state == SCHED_FREE)
			{
				Controller->queues[queue][j].PID = pid;
				Controller->queues[queue][j].state = s;
				Controller->queues[queue][j].next = 0;
				Controller->queues[queue][j].runs = 0;
				Controller->queues[queue][prev].next = j;

				return TRUE;
			}
		}
	}

	return FALSE;
}

void schedAddProcess(uint32 PID)
{
	process * proc = getProcess(PID);

	if( PID == 0 )
		return;

	if( proc != NULL )
		insertProcInQueue(PID,PRIORITY_LEVELS-proc->priority-1, SCHED_READY);
}

uint32 schedRemoveProcess(uint32 PID)
{
	int i;

	for(i=0; i < PRIORITY_LEVELS; i++)
		if(removeProcFromQueue(PID,i))
			return TRUE;

	return FALSE;
}

SchedProcess * schedGetCurrentProcess()
{
	return Controller->currentProcess;
}

SchedProcess * schedGetProcess(uint32 PID)
{
	int i;
	SchedProcess * ret;

	for(i=0; i < PRIORITY_LEVELS; i++)
		if((ret = getProcFromQueue(PID,i)) != NULL)
			return ret;

	return NULL;
}


void beginIteration (void)
{
	/* Salto el proceso 0 (Kernel) */
	int i,j;

	for(i = 0; i < PRIORITY_LEVELS; i++)
	{
		j = Controller->queues[i][0].next;

		if(j == 0)
			continue;

		break;
	}

	if( i == PRIORITY_LEVELS )
		i--;

	Controller->iteratorProc = j;
	Controller->iteratorQueue = i;
}

SchedProcess * iterateProcesses (void)
{
	SchedProcess * ans;

	if (Controller->iteratorProc == 0)
		return NULL;

	ans = &(Controller->queues[Controller->iteratorQueue][Controller->iteratorProc]);

	/** Look for next proc. */
	Controller->iteratorProc = Controller->queues[Controller->iteratorQueue][Controller->iteratorProc].next;

	while(Controller->iteratorProc == 0)
	{
		Controller->iteratorQueue++;
		if(Controller->iteratorQueue == PRIORITY_LEVELS)
			return NULL;

		Controller->iteratorProc = Controller->queues[Controller->iteratorQueue][0].next;
	}

	return ans;
}

uint32 countProcesses (void)
{
	uint32 count = 0;

	beginIteration();

	while (iterateProcesses() != NULL)
		count++;

	return count;
}

uint32 getCurrentRunningProc( void )
{
	uint32 i, total = 0;

	for( i = 0; i < PRIORITY_LEVELS; i++)
		total+=getCurrentRunningProcFromQueue(i);

	return total;
}

uint32 getCurrentRunningProcFromQueue( uint8 queue )
{
	uint32 current = Controller->queues[queue][0].next, total = 0;

	while( current != 0 )
	{
		if( Controller->queues[queue][current].state == SCHED_READY )
			total++;

		current = Controller->queues[queue][current].next;
	}

	return total;
}

#endif /** ifdef SCHED_PRIORITY */
