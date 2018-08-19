/* number.c
 *
 * Number object (CHAR, INTEGER, FLOAT) operations
 *
 * 2016 K.W.E. de Lange
 */
#include <stdlib.h>

#include "number.h"
#include "error.h"


static Object *char_alloc(void)
{
	CharObject *obj;

	if ((obj = calloc(1, sizeof(CharObject))) == NULL)
		error(OutOfMemoryError);

	obj->typeobj = &charobject;
	obj->type = CHAR_T;
	obj->refcount = 0;

	obj->cval = 0;

	return (Object *)obj;
}


static Object *int_alloc(void)
{
	IntObject *obj;

	if ((obj = calloc(1, sizeof(IntObject))) == NULL)
		error(OutOfMemoryError);

	obj->typeobj = &intobject;
	obj->type = INT_T;
	obj->refcount = 0;

	obj->ival = 0;

	return (Object *)obj;
}


static Object *float_alloc(void)
{
	FloatObject *obj;

	if ((obj = calloc(1, sizeof(FloatObject))) == NULL)
		error(OutOfMemoryError);

	obj->typeobj = &floatobject;
	obj->type = FLOAT_T;
	obj->refcount = 0;

	obj->fval = 0;

	return (Object *)obj;
}


static void number_free(Object *obj)
{
	debug_printf(DEBUGALLOC, "\n%p: free %-17s", (void *)obj, TYPENAME(obj));

	free(obj);
}


static void number_print(Object *obj)
{
	switch (TYPE(obj)) {
		case CHAR_T:
			printf("%c", obj_as_char(obj));
			break;
		case INT_T:
			printf("%ld", obj_as_int(obj));
			break;
		case FLOAT_T:
//			printf("%.*g", DECIMAL_DIG, obj_as_float(obj));
//			http://stackoverflow.com/questions/16839658/printf-width-specifier-to-maintain-precision-of-floating-point-value
			printf("%.*G", 15, obj_as_float(obj));
			break;
		default:
			break;
	}
}


static CharObject *char_set(CharObject *obj, char_t c)
{
	obj->cval = c;

	return obj;
}


static IntObject *int_set(IntObject *obj, int_t i)
{
	obj->ival = i;

	return obj;
}


static FloatObject *float_set(FloatObject *obj, float_t f)
{
	obj->fval = f;

	return obj;
}


static Object *number_vset(Object *obj, va_list argp)
{
	switch (TYPE(obj)) {
		case CHAR_T:
			return (Object *)char_set((CharObject *)obj, va_arg(argp, int));
		case INT_T:
			return (Object *)int_set((IntObject *)obj, va_arg(argp, int_t));
		case FLOAT_T:
			return (Object *)float_set((FloatObject *)obj, va_arg(argp, double));
		default:
			break;
	}
	return obj;
}


/* Determine the type of the result of an arithmetic operations using two
 * operands according to the following rules:
 *
 * FLOAT is one of both operands is FLOAT,
 * else INTEGER if one of both operands in INTEGER
 * else CHAR
 */
static objecttype_t coerce(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return FLOAT_T;
	else if (TYPE(op1) == INT_T || TYPE(op2) == INT_T)
		return INT_T;
	else
		return CHAR_T;
}


Object *number_add(Object *op1, Object *op2)
{
	switch (coerce(op1, op2)) {
		case CHAR_T:
			return obj_create(CHAR_T, obj_as_char(op1) + obj_as_char(op2));
		case INT_T:
			return obj_create(INT_T, obj_as_int(op1) + obj_as_int(op2));
		case FLOAT_T:
			return obj_create(FLOAT_T, obj_as_float(op1) + obj_as_float(op2));
		default:
			return NULL;
	}
}


Object *number_sub(Object *op1, Object *op2)
{
	switch (coerce(op1, op2)) {
		case CHAR_T:
			return obj_create(CHAR_T, obj_as_char(op1) - obj_as_char(op2));
		case INT_T:
			return obj_create(INT_T, obj_as_int(op1) - obj_as_int(op2));
		case FLOAT_T:
			return obj_create(FLOAT_T, obj_as_float(op1) - obj_as_float(op2));
		default:
			return NULL;
	}
}


Object *number_mul(Object *op1, Object *op2)
{
	switch (coerce(op1, op2)) {
		case CHAR_T:
			return obj_create(CHAR_T, obj_as_char(op1) * obj_as_char(op2));
		case INT_T:
			return obj_create(INT_T, obj_as_int(op1) * obj_as_int(op2));
		case FLOAT_T:
			return obj_create(FLOAT_T, obj_as_float(op1) * obj_as_float(op2));
		default:
			return NULL;
	}
}


Object *number_div(Object *op1, Object *op2)
{
	if (obj_as_int(op2) == 0)
		error(DivisionByZeroError);

	switch (coerce(op1, op2)) {
		case CHAR_T:
			return obj_create(CHAR_T, obj_as_char(op1) / obj_as_char(op2));
		case INT_T:
			return obj_create(INT_T, obj_as_int(op1) / obj_as_int(op2));
		case FLOAT_T:
			return obj_create(FLOAT_T, obj_as_float(op1) / obj_as_float(op2));
		default:
			return NULL;
	}
}


Object *number_mod(Object *op1, Object *op2)
{
	if (obj_as_int(op2) == 0)
		error(DivisionByZeroError);

	switch (coerce(op1, op2)) {
		case CHAR_T:
			return obj_create(CHAR_T, obj_as_char(op1) % obj_as_char(op2));
		case INT_T:
			return obj_create(INT_T, obj_as_int(op1) % obj_as_int(op2));
		case FLOAT_T:
			error(ModNotAllowedError, "%% operator only allowed on integers");
		default:
			return NULL;
	}
}


Object *number_inv(Object *op1)
{
	Object *op2, *result;

	switch (TYPE(op1)) {
		case CHAR_T:
			op2 = obj_create(CHAR_T, (char_t)0);
			break;
		case INT_T:
			op2 = obj_create(INT_T, (int_t)0);
			break;
		case FLOAT_T:
			op2 = obj_create(FLOAT_T, (float_t)0);
			break;
		default:
			return NULL;
	}
	result = obj_sub(op2, op1);

	obj_decref(op2);

	return result;
}


Object *number_eql(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return obj_create(INT_T, (int_t)(obj_as_float(op1) == obj_as_float(op2)));
	else if (TYPE(op1) == INT_T || TYPE(op1) == INT_T)
		return obj_create(INT_T, (int_t)(obj_as_int(op1) == obj_as_int(op2)));
	else
		return obj_create(INT_T, (int_t)(obj_as_char(op1) == obj_as_char(op2)));
}


Object *number_neq(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return obj_create(INT_T, (int_t)(obj_as_float(op1) != obj_as_float(op2)));
	else if (TYPE(op1) == INT_T || TYPE(op1) == INT_T)
		return obj_create(INT_T, (int_t)(obj_as_int(op1) != obj_as_int(op2)));
	else
		return obj_create(INT_T, (int_t)(obj_as_char(op1) != obj_as_char(op2)));
}


Object *number_lss(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return obj_create(INT_T, (int_t)(obj_as_float(op1) < obj_as_float(op2)));
	else if (TYPE(op1) == INT_T || TYPE(op1) == INT_T)
		return obj_create(INT_T, (int_t)(obj_as_int(op1) < obj_as_int(op2)));
	else
		return obj_create(INT_T, (int_t)(obj_as_char(op1) < obj_as_char(op2)));
}


Object *number_leq(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return obj_create(INT_T, (int_t)(obj_as_float(op1) <= obj_as_float(op2)));
	else if (TYPE(op1) == INT_T || TYPE(op1) == INT_T)
		return obj_create(INT_T, (int_t)(obj_as_int(op1) <= obj_as_int(op2)));
	else
		return obj_create(INT_T, (int_t)(obj_as_char(op1) <= obj_as_char(op2)));
}


Object *number_gtr(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return obj_create(INT_T, (int_t)(obj_as_float(op1) > obj_as_float(op2)));
	else if (TYPE(op1) == INT_T || TYPE(op1) == INT_T)
		return obj_create(INT_T, (int_t)(obj_as_int(op1) > obj_as_int(op2)));
	else
		return obj_create(INT_T, (int_t)(obj_as_char(op1) > obj_as_char(op2)));
}


Object *number_geq(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return obj_create(INT_T, (int_t)(obj_as_float(op1) >= obj_as_float(op2)));
	else if (TYPE(op1) == INT_T || TYPE(op1) == INT_T)
		return obj_create(INT_T, (int_t)(obj_as_int(op1) >= obj_as_int(op2)));
	else
		return obj_create(INT_T, (int_t)(obj_as_char(op1) >= obj_as_char(op2)));
}


Object *number_or(Object *op1, Object *op2)
{
	return obj_create(INT_T, (int_t)(obj_as_bool(op1) || obj_as_bool(op2) ? 1 : 0));
}


Object *number_and(Object *op1, Object *op2)
{
	return obj_create(INT_T, (int_t)(obj_as_bool(op1) && obj_as_bool(op2) ? 1 : 0));
}


Object *number_negate(Object *op1)
{
	return obj_create(INT_T, (int_t)!obj_as_bool(op1));
}


/* Number object API (separate for char_t, int_t and float_t).
 */
TypeObject charobject = {
	"char",
	char_alloc,
	number_free,
	number_print,
	(Object *(*)())char_set,
	number_vset
};

TypeObject intobject = {
	"int",
	int_alloc,
	number_free,
	number_print,
	(Object *(*)())int_set,
	number_vset
};

TypeObject floatobject = {
	"float",
	float_alloc,
	number_free,
	number_print,
	(Object *(*)())float_set,
	number_vset
};
