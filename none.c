/* none.c
 *
 * 2016	K.W.E. de Lange
 */
#include "none.h"


static NoneObject none = {
	.refcount = 0,
	.type = NONE_T,
	.typeobj = (TypeObject *)&nonetype
	};


static NoneObject *none_alloc(void)
{
	return &none;
}


static void none_free(NoneObject *obj)
{
	;
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
NoneType nonetype = {
	.name = "none",
	.alloc = (Object *(*)())none_alloc,
	.free = (void (*)(Object *))none_free,
	.print = (void (*)(Object *))none_print,
	.set = (Object *(*)())none_set,
	.vset = (Object *(*)(Object *, va_list))none_vset
	};
