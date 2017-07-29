/*	strndup.c
 *
 *	Not every C compiler has this function in its standard library, so here
 * 	is a private version.
 * 
 *	1994	K.W.E. de Lange
 */
#include <string.h>
#include <stdlib.h>


char *strndup(const char *s, size_t n)
{
	char *result;
	size_t len = strlen(s);

	if (n < len)
		len = n;

	result = (char *)malloc(len + 1);

	if (!result)
		return 0;

	result[len] = '\0';

	return (char *)memcpy(result, s, len);
}
