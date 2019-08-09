/* strdup.c
 *
 * Not every C compiler has this function in its standard library so here
 * is a private version.
 *
 * 1994	K.W.E. de Lange
 */
#include <string.h>
#include <stdlib.h>


char *strdup(const char *s)
{
    void *new;
    size_t len = strlen(s) + 1;

    new = malloc(len);

    if (new == NULL)
        return NULL;

    return (char *)memcpy(new, s, len);
}
