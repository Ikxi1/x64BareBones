#include <example.h>
#include <syscalls.h>
#include <string.h>
#include <libc.h>
#include <types.h>

#define	NPROC		24

int resetSem(int argc, char ** argv)
{
	if (argc != 2)
	{
		cprintf("Invalid arguments for semreset\n");
		return 1;
	}
	
	cprintf("Resting semaphore %d\n", atoi(argv[1]));
	semReset(atoi(argv[1]));
	return 0;
}

int semtestA(int argc, char ** argv)
{
	char * msg1 = "b";
	//int i=0;

	while(1)
	//for (i=0; i<4; i++)
	{
		//yield();
		//while(!semNBDec(1));

		semDec(9);
		vtWrite(msg1, strlen(msg1));
		semInc(10);
	}
	while(1);
}

int semtestB(int argc, char ** argv)
{
	char * msg2 = "a";
	//int i=0;
	semInc(10);

	while(1)
	//for (i=0; i<4; i++)
	{
		//yield();
		//while(!semNBDec(0));

		semDec(10);
		vtWrite(msg2, strlen(msg2));
		semInc(9);
	}
	while(1);
}

int spawner(int argc, char ** argv)
{
	char * msg1 = "Init... ";
	char * msg2 = "Started 4 process!\n";
	int i;

	vtWrite(msg1, strlen(msg1));
	start(spawned, TRUE, 1, 0, NULL);
	for (i=0; i<4; i++)
		start(spawned, TRUE, 1, 0, NULL);
	vtWrite(msg2, strlen(msg2));
	while(1);
}

int spawned(int argc, char ** argv)
{
	char * msg1 = "Init... ";
	char * msg2 = "Started 4 more evil process!!\n";
	int i;

	vtWrite(msg1, strlen(msg1));
	for (i=0; i<4; i++)
		start(evil, FALSE, 1, 0, NULL);
	vtWrite(msg2, strlen(msg2));
	while(1);
}

int evil(int argc, char ** argv)
{
	char * msg1 = "Hello I'm evil e!\n";
	vtWrite(msg1, strlen(msg1));
	
	char * f = (char*)0xFFFFFFF0;
	f[0] = 0;
	while(1);
	return 0;
}

int shared(int argc, char ** argv)
{
	uint32 childPID = start(sharedChild,FALSE, 1, 0, NULL);
	char * shmem = getShmem(getId(childPID));
	strcpy(shmem, "HOLA SOY UNA COSA COMPARTIDA :D:D:D:D:D\n");
	cprintf("Hola\n");
	semInc(getId(childPID));
	while(1);
	shmem[0] = 4;
}

int sharedChild(int argc, char ** argv)
{
	char * shmem = getShmem(getId(getpid()));
	semDec(getId(getpid()));
	cprintf("Hola\n");
	vtWrite(shmem, strlen(shmem));
	while(1);
}

int helloWorld(int argc, char ** argv)
{
	cprintf("Hello World!!\n");
	while(1);
	return 0;
}

int callhelloWorld(int argc, char ** argv)
{
	return 0;
}

int callKill (int argc, char ** argv)
{
	if (argc > 4)
		return 0;

	return kill (atoi (argv[1]));
}

int callTop (int argc, char ** argv)
{
	vecProcessInfo mem;
	
	if (argc > 3)
		return 0;
	
	if ((mem = malloc (NPROC * sizeof(tProcessInfo))) == NULL)
		return 0;
	
	top (mem, NPROC);
	free (mem);
	return 1;
}

int callFaultonomicon (int argc, char ** argv)
{
	return 0;
}

int callClear (int argc, char ** argv)
{
	int i;

	for (i=0; i<80*24; i++)
		cprintf("\b");
	return 0;
}

