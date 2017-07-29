/*	position.c
 *
 * 	Position object operations
 *
 * 	1994	K.W.E. de Lange
 */
#include "exin.h"


/*	Standard forward declarations for a typeobject
 *
 */
static PositionObject *pos_alloc(void);
static void pos_free(PositionObject *obj);
static void pos_print(PositionObject *obj);
static PositionObject *pos_set(PositionObject *dest, PositionObject *src);
static PositionObject *pos_vset(PositionObject *obj, va_list argp);


/*	Position object API.
 *
 */
TypeObject posobject = {
	"pos",
	(Object *(*)())pos_alloc,
	(void (*)(Object *))pos_free,
	(void (*)(Object *))pos_print,
	(Object *(*)())pos_set,
	(Object *(*)(Object *, va_list))pos_vset
};


static PositionObject *pos_alloc(void)
{
	PositionObject *obj;

	obj = calloc((size_t)1, sizeof(PositionObject));

	if (obj != NULL) {
		obj->typeobj = &posobject;
		obj->type = POSITION_T;
		obj->refcount = 0;

		obj->reader.rp = NULL;
		obj->scanner.token = UNKNOWN;
		obj->scanner.string = strdup("");

		if (obj->scanner.string == NULL)
			error(OutOfMemoryError);
	}
	return obj;
}


static void pos_free(PositionObject *obj)
{
	debug_printf(DEBUGLEVEL3, "\n%p: free %s", (void *)obj, TYPENAME(obj));

	free(obj->scanner.string);
	free(obj);
}


static void pos_print(PositionObject *obj)
{
	return;
}


static PositionObject *pos_set(PositionObject *dest, PositionObject *src)
{
	dest->reader.rp = src->reader.rp;
	dest->scanner.token = src->scanner.token;

	if (dest->scanner.string)
		free(dest->scanner.string);

	dest->scanner.string = strdup(src->scanner.string);

	if (dest->scanner.string == NULL)
		error(OutOfMemoryError);

	return dest;
}


static PositionObject *pos_vset(PositionObject *dest, va_list argp)
{
	return pos_set(dest, va_arg(argp, PositionObject *));
}
