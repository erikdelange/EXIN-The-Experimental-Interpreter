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

	obj->typeobj = (TypeObject *)&chartype;
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

	obj->typeobj = (TypeObject *)&inttype;
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

	obj->typeobj = (TypeObject *)&floattype;
	obj->type = FLOAT_T;
	obj->refcount = 0;

	obj->fval = 0;

	return (Object *)obj;
}


static void number_free(Object *obj)
{
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
			return (Object *)char_set((CharObject *)obj, va_arg(argp, int));  /* va_arg requires at least an int */
		case INT_T:
			return (Object *)int_set((IntObject *)obj, va_arg(argp, int_t));
		case FLOAT_T:
			return (Object *)float_set((FloatObject *)obj, va_arg(argp, float_t));
		default:
			break;
	}
	return obj;
}


/* Determine the type of the result of an arithmetic operations using two
 * operands according to the following rules:
 *
 * FLOAT if at least one operand is FLOAT,
 * else INTEGER if at least one operand in INTEGER
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


static Object *number_add(Object *op1, Object *op2)
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


static Object *number_sub(Object *op1, Object *op2)
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


static Object *number_mul(Object *op1, Object *op2)
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


static Object *number_div(Object *op1, Object *op2)
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


static Object *number_mod(Object *op1, Object *op2)
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


static Object *number_inv(Object *op1)
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


static Object *number_eql(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return obj_create(INT_T, (int_t)(obj_as_float(op1) == obj_as_float(op2)));
	else if (TYPE(op1) == INT_T || TYPE(op1) == INT_T)
		return obj_create(INT_T, (int_t)(obj_as_int(op1) == obj_as_int(op2)));
	else
		return obj_create(INT_T, (int_t)(obj_as_char(op1) == obj_as_char(op2)));
}


static Object *number_neq(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return obj_create(INT_T, (int_t)(obj_as_float(op1) != obj_as_float(op2)));
	else if (TYPE(op1) == INT_T || TYPE(op1) == INT_T)
		return obj_create(INT_T, (int_t)(obj_as_int(op1) != obj_as_int(op2)));
	else
		return obj_create(INT_T, (int_t)(obj_as_char(op1) != obj_as_char(op2)));
}


static Object *number_lss(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return obj_create(INT_T, (int_t)(obj_as_float(op1) < obj_as_float(op2)));
	else if (TYPE(op1) == INT_T || TYPE(op1) == INT_T)
		return obj_create(INT_T, (int_t)(obj_as_int(op1) < obj_as_int(op2)));
	else
		return obj_create(INT_T, (int_t)(obj_as_char(op1) < obj_as_char(op2)));
}


static Object *number_leq(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return obj_create(INT_T, (int_t)(obj_as_float(op1) <= obj_as_float(op2)));
	else if (TYPE(op1) == INT_T || TYPE(op1) == INT_T)
		return obj_create(INT_T, (int_t)(obj_as_int(op1) <= obj_as_int(op2)));
	else
		return obj_create(INT_T, (int_t)(obj_as_char(op1) <= obj_as_char(op2)));
}


static Object *number_gtr(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return obj_create(INT_T, (int_t)(obj_as_float(op1) > obj_as_float(op2)));
	else if (TYPE(op1) == INT_T || TYPE(op1) == INT_T)
		return obj_create(INT_T, (int_t)(obj_as_int(op1) > obj_as_int(op2)));
	else
		return obj_create(INT_T, (int_t)(obj_as_char(op1) > obj_as_char(op2)));
}


static Object *number_geq(Object *op1, Object *op2)
{
	if (TYPE(op1) == FLOAT_T || TYPE(op2) == FLOAT_T)
		return obj_create(INT_T, (int_t)(obj_as_float(op1) >= obj_as_float(op2)));
	else if (TYPE(op1) == INT_T || TYPE(op1) == INT_T)
		return obj_create(INT_T, (int_t)(obj_as_int(op1) >= obj_as_int(op2)));
	else
		return obj_create(INT_T, (int_t)(obj_as_char(op1) >= obj_as_char(op2)));
}


static Object *number_or(Object *op1, Object *op2)
{
	return obj_create(INT_T, (int_t)(obj_as_bool(op1) || obj_as_bool(op2) ? 1 : 0));
}


static Object *number_and(Object *op1, Object *op2)
{
	return obj_create(INT_T, (int_t)(obj_as_bool(op1) && obj_as_bool(op2) ? 1 : 0));
}


static Object *number_negate(Object *op1)
{
	return obj_create(INT_T, (int_t)!obj_as_bool(op1));
}


/* Number object API (separate for char_t, int_t, float_t and number_t).
 */
CharType chartype = {
	.name = "char",
	.alloc = char_alloc,
	.free = number_free,
	.print = number_print,
	.set = (Object *(*)())char_set,
	.vset = number_vset
	};

IntType inttype = {
	.name = "int",
	.alloc = int_alloc,
	.free = number_free,
	.print = number_print,
	.set = (Object *(*)())int_set,
	.vset = number_vset
	};

FloatType floattype = {
	.name = "float",
	.alloc = float_alloc,
	.free = number_free,
	.print = number_print,
	.set = (Object *(*)())float_set,
	.vset = number_vset
	};

NumberType numbertype = {
	.name = "number",
	.alloc = int_alloc,  /* number considered INT_T */
	.free = number_free,
	.print = number_print,
	.set = (Object *(*)())int_set,  /* number considered INT_T */
	.vset = number_vset,

	.add = number_add,
	.sub = number_sub,
	.mul = number_mul,
	.div = number_div,
	.mod = number_mod,
	.inv = number_inv,
	.eql = number_eql,
	.neq = number_neq,
	.lss = number_lss,
	.leq = number_leq,
	.gtr = number_gtr,
	.geq = number_geq,
	.or = number_or,
	.and = number_and,
	.negate = number_negate
	};
