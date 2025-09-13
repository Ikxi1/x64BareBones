#include "string.h"

char * strcpy(char * dest, char * orig)
{
	while(*orig != 0)
		*dest++ = *orig++;

	*dest = 0;
	return dest;
}

int strlen(char * str)
{
	int c=0;

	while(*(str++))
		c++;

	return c;
}

int strcmp(char * s1, char * s2)
{
	while(*s1 == *s2 && *s1 != '\0')
		s1++,s2++;

	if (*s1=='\0' && *s2=='\0')
		return 0;

	if (*s1<*s2)
		return -1;
	else
		return 1;
}

int strncmp(char * s1, char * s2, int size)
{
	while(*s1 == *s2 && *s1 != '\0' && size)
		s1++,s2++,size--;

	if (size == 0 || (*s1 == '\0' && *s2 == '\0'))
		return 0;

	if (*s1 < *s2)
		return -1;
	else
		return 1;
}

int iisalpha(int c)
{
	return ((c<'a' || c>'z') && (c<'A' || c>'Z'))? 0 : 1;
}

void * memcpy (void * dst, void const * src, int len)
{
	char *cdst = dst;
	char const *csrc = src;

	while (len-- > 0)
		*cdst++ = *csrc++;

	return dst;
}

