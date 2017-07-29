/*	string.c
 *
 * 	String object operations
 *
 * 	2016	K.W.E. de Lange
 */
#include "exin.h"


/*	Standard forward declarations for a typeobject
 *
 */
static StrObject *str_alloc(void);
static void str_free(StrObject *obj);
static void str_print(StrObject *obj);
static StrObject *str_set(StrObject *obj, const char *s);
static StrObject *str_vset(StrObject *obj, va_list argp);
static int_t length(StrObject *obj);


/*	String object API.
 *
 */
TypeObject strobject = {
	"str",
	(Object *(*)())str_alloc,
	(void (*)(Object *))str_free,
	(void (*)(Object *))str_print,
	(Object *(*)())str_set,
	(Object *(*)(Object *, va_list))str_vset
};


static StrObject *str_alloc(void)
{
	StrObject *obj;

	obj = calloc((size_t)1, sizeof(StrObject));

	if (obj != NULL) {
		obj->typeobj = &strobject;
		obj->type = STR_T;
		obj->refcount = 0;

		obj->sptr = strdup("");  /* initial value is empty string */

		if (obj->sptr == NULL)
			error(OutOfMemoryError);
	}
	return obj;
}


static void str_free(StrObject *obj)
{
	debug_printf(DEBUGLEVEL3, "\n%p: free %s", (void *)obj, TYPENAME(obj));

	free(obj->sptr);
	free(obj);
}


static void str_print(StrObject *obj)
{
	printf("%s", obj->sptr);
}


static StrObject *str_set(StrObject *obj, const char *s)
{
	if (obj->sptr != NULL)
		free(obj->sptr);

	obj->sptr = strdup(s);

	if (obj->sptr == NULL)
		error(OutOfMemoryError);

	return obj;
}


static StrObject *str_vset(StrObject *obj, va_list argp)
{
	return str_set(obj, va_arg(argp, char *));
}


/*	Operand op1 or op2 is a string. The other operand can be anything and
 *	must be converted to a string.
 *
 */
Object *str_concat(Object *op1, Object *op2)
{
	char *s;
	size_t bytes;
	Object *obj, *conv = NULL;

	op1 = TYPE(op1) == STR_T ? op1 : (conv = obj_to_strobj(op1));
	op2 = TYPE(op2) == STR_T ? op2 : (conv = obj_to_strobj(op2));

	bytes = strlen(obj_as_str(op1)) + strlen(obj_as_str(op2)) + 1;

	s = calloc(bytes, sizeof(char));

	if (s == NULL)
		error(OutOfMemoryError);

	strcpy(s, obj_as_str(op1));
	strcat(s, obj_as_str(op2));

	obj = obj_create(STR_T, s);

	free(s);

	if (conv)
		obj_free(conv);

	return obj;
}


Object *str_repeat(Object *op1, Object *op2)
{
	char *str;
	size_t times, bytes;
	Object *obj;

	Object *s = TYPE(op1) == STR_T ? op1 : op2;
	Object *n = TYPE(op1) == STR_T ? op2 : op1;

	times = obj_as_int(n);

	if (times < 0)
		times = 0;

	bytes = (size_t)length((StrObject *)s) * times + 1;

	str = calloc(bytes, sizeof(char));

	if (str == NULL)
		error(OutOfMemoryError);

	while (times--)
		strcat(str, obj_as_str(s));

	obj = obj_create(STR_T, str);

	free(str);

	return obj;
}


Object *str_eql(Object *op1, Object *op2)
{
	int r = strcmp(obj_as_str(op1), obj_as_str(op2)) == 0 ? 1 : 0;

	return obj_create(INT_T, (int_t)r);
}


Object *str_neq(Object *op1, Object *op2)
{
	int r = strcmp(obj_as_str(op1), obj_as_str(op2)) == 0 ? 1 : 0;

	return obj_create(INT_T, (int_t)!r);
}


static int_t length(StrObject *obj)
{
	return strlen(obj_as_str((Object *)obj));
}


Object *str_length(StrObject *obj)
{
	return obj_create(INT_T, length(obj));
}


CharObject *str_item(StrObject *str, int index)
{
	CharObject *obj;
	int len;

	len = length(str);

	if (index < 0)
		index += len;

	if (index < 0 || index >= len)
		return NULL;  /* IndexError: index out of range */

	obj = (CharObject *)obj_create(CHAR_T, *(obj_as_str((Object *)str) + index));

	return obj;
}


StrObject *str_slice(StrObject *obj, int start, int end)
{
	StrObject *slice;
	char *src, *dst;
	int len;

	len = length(obj);

	if (start < 0)
		start += len;

	if (end < 0)
		end += len;

	if (start < 0)
		start = 0;

	if (end >= len)
		end = len;

	src = obj_as_str((Object *)obj);
	dst = strndup(src + start, end - start);

	slice = (StrObject *)obj_create(STR_T, dst);

	free(dst);

	return slice;
}
