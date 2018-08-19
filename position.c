/* position.c
 *
 * Position object operations
 *
 * 2018	K.W.E. de Lange
 */
#include <stdlib.h>
#include "position.h"
#include "error.h"


static PositionObject *pos_alloc(void)
{
	PositionObject *obj;

	if ((obj = calloc(1, sizeof(PositionObject))) == NULL)
		error(OutOfMemoryError);

	obj->typeobj = &posobject;
	obj->type = POSITION_T;
	obj->refcount = 0;

	reader.init(&obj->reader);
	scanner.init(&obj->scanner);

	return obj;
}


static void pos_free(PositionObject *obj)
{
	debug_printf(DEBUGALLOC, "\n%p: free %-17s", (void *)obj, TYPENAME(obj));
	free(obj);
}


static void pos_print(PositionObject *obj)
{
	return;
}


static PositionObject *pos_set(PositionObject *dest, PositionObject *src)
{
	dest->reader = src->reader;
	dest->scanner = src->scanner;
	return dest;
}


static PositionObject *pos_vset(PositionObject *dest, va_list argp)
{
	return pos_set(dest, va_arg(argp, PositionObject *));
}


/*	Position object API.
 */
TypeObject posobject = {
	"pos",
	(Object *(*)())pos_alloc,
	(void (*)(Object *))pos_free,
	(void (*)(Object *))pos_print,
	(Object *(*)())pos_set,
	(Object *(*)(Object *, va_list))pos_vset
};
