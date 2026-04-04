#ifndef STRING_H
#define STRING_H

/*Identico a isalpha de la biblioteca estandar*/
int iisalpha(int c);

/*Identico a strlen de la biblioteca estandar*/
int strlen(char * str);

/*Identico a strcmp de la biblioteca estandar*/
int strcmp(char * s1, char * s2);

/*Identico a strncmp de la biblioteca estandar*/
int strncmp(char * s1, char * s2, int size);

/*Identico a strcpy de la biblioteca estandar*/
char * strcpy(char * dest , char * orig);

// void * memcpy (void * dst, void const * src, int len);

#endif

