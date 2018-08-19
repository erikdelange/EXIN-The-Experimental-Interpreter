/* none.c
 *
 * 2016	K.W.E. de Lange
 */
#include "none.h"


NoneObject none = {1, NONE_T, &noneobject};


static NoneObject *none_alloc(void)
{
	return &none;
}


static void none_free(NoneObject *obj)
{
	debug_printf(DEBUGALLOC, "\n%p: free %-17s", (void *)obj, TYPENAME(obj));
}


static void none_print(NoneObject *obj)
{
	printf("none");
}


static NoneObject *none_set(NoneObject *obj)
{
	return &none;
}

static NoneObject *none_vset(NoneObject *obj, va_list argp)
{
	return &none;
}


/*	None object API.
 */
TypeObject noneobject = {
	"none",
	(Object *(*)())none_alloc,
	(void (*)(Object *))none_free,
	(void (*)(Object *))none_print,
	(Object *(*)())none_set,
	(Object *(*)(Object *, va_list))none_vset
};
