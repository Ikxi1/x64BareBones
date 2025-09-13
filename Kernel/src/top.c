#include <process.h>
#include <scheduler.h>
#include <top.h>
#include <libc.h>
#include <syscalls.h>
#include <types.h>

/* Obtengo la lista de procesos y la dejo en processes, si el tamanio del vector es insuficiente y no se pudo
 * completar satisfactoriamente la operacion devuelvo 0, caso contrario 1
 */
int pfTop (vecProcessInfo processes, int len)
{
	int i;
	SchedProcess * proc;
	process * procInfo;

	beginIteration();

	for (i = 0; i < len && (proc = iterateProcesses()) != NULL; i++)
	{
		procInfo = getProcess (proc->PID);
		processes[i].processPID = proc->PID;
		processes[i].state = proc->state;
		processes[i].priority = procInfo->priority;
		processes[i].vt = procInfo->vt;
		processes[i].fixedPriority = procInfo->fixedPriority;
		processes[i].processor = proc->usage;
		/* TODO Falta pedir el consumo */
	}
	
	return 1;
}

int top (vecProcessInfo processes, int len)
{
	/* Llamo al system call que me devuelve un vector con los datos del proceso que se estan ejecutando
	 * en ese momento
	 */
	int i;

	if (!getInfoProcesses (processes, len))
		return 0;

	for (i = 0; i < len && processes[i].processPID != 0 ; i++)
	{
		cprintf("PID: %d CPU: %d Status: %s ", processes[i].processPID, processes[i].processor,
				processes[i].state == SCHED_READY? "Ready" : (processes[i].state == SCHED_RUNNING? "Running" : "Waiting"));
		cprintf("VT: %d Priority: %d Fix Priority: %s\n",
				processes[i].vt, processes[i].priority, processes[i].fixedPriority? "YES":"NO");
	}

	return 1;
}
