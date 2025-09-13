/*
 * shell.c
 *
 *  Created on: 30/09/2008
 *      Author: Eduardo Casal, Luciano Mangiarotti, Rodrigo Rearden
 */

/* Las funciones con r adelante son proporcionadas por row :P */

/*
 * Funciones del shell:
 * 		- Matar procesos (kill)
 * 		- Estado del proceso (top)
 * 		- Ejecucion procesos (exec)
 */

#include <stdint.h>
#include <string.h>
#include <shell.h>
#include <top.h>
#include <syscalls.h>
#include <libc.h>
#include <example.h>
#include <vt.h>

#define	PROMPT		"Termopilas $> "
#define	TRUE		1
#define	MAX_LINE	30

typedef int (*exFnc)(int argc, char ** argv);

typedef struct
{
	char * fName;
	exFnc fnc;
} tFnc;

static char * GetLine (int max_line);
static int myStrNCmp (const char * s1, const char * s2, int len);
static char ** getArgs (char * line, int * argc);
static char ** getArgsWrapp (char * line, int argc);

int shell (void)
{
	tFnc commands[] = { {"semreset", resetSem},
						{"semtestA", semtestA},
						{"semtestB", semtestB},
						{"spawner", spawner},
						{"evil", evil},
						{"shared", shared},
						{"clear", callClear},
						{"kill", callKill},
						{"top", callTop},
						{"faultonomicon", callFaultonomicon},
						{"Hello",helloWorld},
						{"", NULL} };
	char * line, ** argv;
	int i, argc, attached = 1;

	while (TRUE)
	{
		cprintf(PROMPT);
		
		line = GetLine (MAX_LINE);
		cprintf("%s\n",line);

		for (i = 0; *commands[i].fName != '\0'; i++)
			if (!myStrNCmp (commands[i].fName, line, strlen (commands[i].fName)))
			{
				if ((argv = getArgs (line, &argc)) == NULL)
					cprintf ("Comando Invalido\n");
				else
				{
					uint8 priority = 1;
					
					if(argc > 1)
					{
						if (strcmp(argv[argc-1], "&") == 0)		/* Se corre en background */
							attached = 0;
						else
							priority = atoi(argv[argc-1]);
					}
								
//					cprintf("Detalles de la ejecucion: "
//							"FUN: %x, "
//							"ATT: %d, "
//							"%d, %d, %d\n", commands[i].fnc, attached, atoi(argv[argc-1]), argc, argv);

					start (commands[i].fnc, attached, priority, argc, argv);

					for (i = 0; i < argc; i++)
						free (argv[i]);
					free (argv);
					attached = 1;
				}
				break;
			}

		if (i == sizeof(commands) / sizeof(commands[0]) - 1 && line[0])
			cprintf("Comando Invalido\n");
		
		free (line);
	}
}

static char ** getArgs (char * line, int * argc)
{
	int i;

	*argc = 0;

	/* Hago algo similar a strtok, corto los argumentos con un '\0', facilitando la tarea del wrapper.
	 * Aumenta la complejidad del algortimo al no poder utilizar realloc
	 */
	for (i = 0; line[i] != '\0'; i++)
		if (line[i] == ' ')
		{
			(*argc)++;
			line[i] = '\0';
		}

	(*argc)++;
//	cprintf("TODO OK %s, %d\n",line, *argc);
	return getArgsWrapp (line, *argc);
}

static char ** getArgsWrapp (char * line, int argc)
{
	char ** args;
	int i, longi;

	if ((args = malloc (argc * sizeof(char*))) == NULL)
		return NULL;

//	vtprintf(0,"malloc1 no falla\n");
	
	for (i = 0; i < argc; i++)
	{
		char * mal;
//		vtprintf(0,"pidiendo %d\n",strlen(line) + 1);
//		vtprintf(0,"args at %x, args[%d] at %x\n",args,i,&(args[i]));
		mal = malloc ((longi = strlen(line)) + 1);
//		vtprintf(0,"Malloc ok\n");
		if ((args[i] = mal ) == NULL)
				return NULL;
//		vtprintf(0,"hubo malloc\n");
		strcpy (args[i], line);
		line += longi + 1;
	}

	return args;
}

static char * GetLine (int max_line)
{
	int c, i = 0;
	char * resp;

//	vtprintf(0,"2\n");
	
	if ((resp = malloc (max_line)) == NULL)
	//if ((resp = page ()) == NULL)
	{
		cprintf("Memoria Insuficiente\n");
		return NULL;
	}
	
//	vtprintf(0,"3\n");

	while ((c = cgetchar()) != '\n')
	{
		if( c != 0 )
			if (i < max_line)
				resp[i++] = c;
	}
	resp[i] = '\0';
	
	return resp;
}

static int myStrNCmp (const char * s1, const char * s2, int len)
{
	int i = 0;

	while (*s1 && *s2 && i < len-1 && *s1 == *s2)
	{
		s1++;
		s2++;
		i++;
	}

	return *s1 - *s2;
}
