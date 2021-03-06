/* string.c
 *
 * String object operations
 *
 * 2016 K.W.E. de Lange
 */
#include <stdlib.h>
#include <string.h>

#include "strndup.h"
#include "strdup.h"
#include "error.h"
#include "str.h"


static StrObject *str_alloc(void)
{
	StrObject *obj;

	if ((obj = calloc(1, sizeof(StrObject))) == NULL)
		error(OutOfMemoryError);

	obj->typeobj = (TypeObject *)&strtype;
	obj->type = STR_T;
	obj->refcount = 0;

	if ((obj->sptr = strdup("")) == NULL)  /* initial value is empty string */
		error(OutOfMemoryError);

	return obj;
}


static void str_free(StrObject *obj)
{
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

	if ((obj->sptr = strdup(s)) == NULL)
		error(OutOfMemoryError);

	return obj;
}


static StrObject *str_vset(StrObject *obj, va_list argp)
{
	return str_set(obj, va_arg(argp, char *));
}


/* Operand op1 or op2 is a string. The other operand can be anything and
 * will be converted to a string.
 */
static Object *str_concat(Object *op1, Object *op2)
{
	char *s;
	size_t bytes;
	Object *obj, *conv = NULL;

	op1 = TYPE(op1) == STR_T ? op1 : (conv = obj_to_strobj(op1));
	op2 = TYPE(op2) == STR_T ? op2 : (conv = obj_to_strobj(op2));

	bytes = strlen(obj_as_str(op1)) + strlen(obj_as_str(op2)) + 1;

	if ((s = calloc(bytes, sizeof(char))) == NULL)
		error(OutOfMemoryError);

	strcpy(s, obj_as_str(op1));
	strcat(s, obj_as_str(op2));

	obj = obj_create(STR_T, s);

	free(s);

	if (conv)
		obj_free(conv);

	return obj;
}


static int_t length(StrObject *obj)
{
	return strlen(obj_as_str((Object *)obj));
}


static Object *str_length(StrObject *obj)
{
	return obj_create(INT_T, length(obj));
}


static Object *str_repeat(Object *op1, Object *op2)
{
	char *str;
    int_t times;
	size_t bytes;
	Object *obj;

	Object *s = TYPE(op1) == STR_T ? op1 : op2;
	Object *n = TYPE(op1) == STR_T ? op2 : op1;

	times = obj_as_int(n);

	if (times < 0)
		times = 0;

	bytes = (size_t)length((StrObject *)s) * times + 1;

	if ((str = calloc(bytes, sizeof(char))) == NULL)
		error(OutOfMemoryError);

	while (times--)
		strcat(str, obj_as_str(s));

	obj = obj_create(STR_T, str);

	free(str);

	return obj;
}


static Object *str_eql(Object *op1, Object *op2)
{
	int result = strcmp(obj_as_str(op1), obj_as_str(op2)) == 0 ? 1 : 0;

	return obj_create(INT_T, (int_t)result);
}


static Object *str_neq(Object *op1, Object *op2)
{
	int result = strcmp(obj_as_str(op1), obj_as_str(op2)) == 0 ? 1 : 0;

	return obj_create(INT_T, (int_t)!result);
}


static CharObject *str_item(StrObject *str, int index)
{
	CharObject *obj;
	int_t len;

	len = length(str);

	if (index < 0)
		index += len;

	if (index < 0 || index >= len)
		return NULL;  /* IndexError: index out of range */

	obj = (CharObject *)obj_create(CHAR_T, *(obj_as_str((Object *)str) + index));

	return obj;
}


static StrObject *str_slice(StrObject *obj, int start, int end)
{
	StrObject *slice;
	char *src, *dst;
	int_t len;

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


/* String object API.
 */
StrType strtype = {
	.name = "str",
	.alloc = (Object *(*)())str_alloc,
	.free = (void (*)(Object *))str_free,
	.print = (void (*)(Object *))str_print,
	.set = (Object *(*)())str_set,
	.vset = (Object *(*)(Object *, va_list))str_vset,

	.length = str_length,
	.item = str_item,
	.slice = str_slice,
	.concat = str_concat,
	.repeat = str_repeat,
	.eql = str_eql,
	.neq = str_neq
	};
