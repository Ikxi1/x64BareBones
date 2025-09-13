/*
 * libc.h
 *
 *  Created on: 29/09/2008
 *      Author: Eduardo Casal, Luciano Mangiarotti, Rodrigo Rearden
 */

#ifndef LIBC_H_
#define LIBC_H_

int cprintf(const char * fmtStr, ...);
void free(void *ap);
void * malloc(unsigned nbytes);
char cgetchar (void);
int atoi (char * s);

#endif /*LIBC_H_*/
