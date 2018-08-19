/* object.c
 *
 * Operations on objects (variables, functions, ...)
 *
 * All variables and functions are modelled as objects. An object contains its
 * data but also a number of methods. Every object has mandatory set of
 * methods. This set is: alloc, free, set, vset and print. Which other methods
 * are available depends in the type of the object. See: number.c, str.c,
 * list.c, position.c and none.c.
 *
 * All operations on and between objects are found in object.c and are
 * accessed via function names like obj_... followed by the operation,
 * e.g. obj_add().
 *
 * We distinguish unary and binary operations. For unary operations only one
 * operand is required:
 *
 *          result = operator operand
 *
 * The unary operators are:
 *
 *          -   negation of the operand
 *          +   retuns the operand (so does nothing)
 *          !   logical negation of the operand (so 0 or 1)
 *
 * For binary operators the two operands as used as input for the operations:
 *
 *          result = operand1 operator operand2
 *
 *          The artihmetic operators are:   +  -  *  /  %
 *          The comparison operators are:   ==  !=  <>  <  <=  >  >=  in
 *          The logical operators are:      and  or
 *
 * Which operations are supported depends on the object type. Numerical object
 * will support almost everything, list or strings have less operations.
 *
 * Two operations are only meant for use on list or string objects:
 *
 *          item[index]
 *          slice[start:end]
 *
 * As C-functions the unary- and binary operations look like:
 *
 *      result *operator(*operand1)
 *      result *operator(*operand1, *operand2)
 *
 * Operand1 and operand2 always remain unchanged. Result is a newly created
 * object. Its type is dependent on operand1 and optionally operand2.
 * See function coerce() in number.c for the rules for determining the type
 * of the result for arithmetic operations. For locical and comparison
 * operations the result is always an INTEGER as we do not have a boolean
 * type.
 *
 * 1994 K.W.E. de Lange
 */
#include <string.h>
#include <stdlib.h>

#include "position.h"
#include "number.h"
#include "object.h"
#include "error.h"
#include "none.h"
#include "str.h"


/* Create a new object of type 'type' and assign the default initial value.
 */
Object *obj_alloc(objecttype_t type)
{
	Object *obj = NULL;

	switch (type) {
		case CHAR_T:
			obj = charobject.alloc();
			break;
		case INT_T:
			obj = intobject.alloc();
			break;
		case FLOAT_T:
			obj = floatobject.alloc();
			break;
		case STR_T:
			obj = strobject.alloc();
			break;
		case LIST_T:
			obj = listobject.alloc();
			break;
		case LISTNODE_T:
			obj = listnodeobject.alloc();
			break;
		case POSITION_T:
			obj = posobject.alloc();
			break;
		case NONE_T:
			obj = noneobject.alloc();
			break;
		default:
			error(SystemError, "cannot allocate type %d", type);
	}

	if (obj == NULL)
		error(OutOfMemoryError);

	debug_printf(DEBUGALLOC, "\n%p: alloc %-16s", (void *)obj, TYPENAME(obj));

	obj_incref(obj);  /* initial refcount = 1 */
	return obj;
}


/* Create a new object of type 'type' and assign an initial value.
 *
 * type type of the new object, also type of the initial value to assign
 * ...  value to assign (mandatory)
 */
Object *obj_create(objecttype_t type, ...)
{
	va_list argp;
	Object *obj;

	va_start(argp, type);

	obj = obj_alloc(type);

	TYPEOBJ(obj)->vset(obj, argp);

	va_end(argp);

	return obj;
}


/* Free the memory which was reserverd for an object.
 */
void obj_free(Object *obj)
{
	TYPEOBJ(obj)->free(obj);
}


/* Print value on stdout.
 */
void obj_print(Object *obj)
{
	TYPEOBJ(obj)->print(obj);
}


/* Read a value from stdin.
 */
Object *obj_scan(objecttype_t type)
{
	char buffer[LINESIZE + 1] = "";
	Object *obj = NULL;

	fgets(buffer, LINESIZE + 1, stdin);
	buffer[strcspn(buffer, "\r\n")] = 0;  /* remove trailing newline */

	switch (type) {
		case CHAR_T:
			obj = obj_create(CHAR_T, str_to_char(buffer));
			break;
		case INT_T:
			obj = obj_create(INT_T, str_to_int(buffer));
			break;
		case FLOAT_T:
			obj = obj_create(FLOAT_T, str_to_float(buffer));
			break;
		case STR_T:
			obj = obj_create(STR_T, buffer);
			break;
		default:
			error(TypeError, "unsupported type for input: %d", type);
	}
	fflush(stdin);

	return obj;
}


/* (type op1)result = op1
 */
Object *obj_copy(Object *op1)
{
	switch (TYPE(op1)) {
		case CHAR_T:
			return obj_create(CHAR_T, obj_as_char(op1));
		case INT_T:
			return obj_create(INT_T, obj_as_int(op1));
		case FLOAT_T:
			return obj_create(FLOAT_T, obj_as_float(op1));
		case STR_T:
			return obj_create(STR_T, obj_as_str(op1));
		case LIST_T:
			return obj_create(LIST_T, obj_as_list(op1));
		case LISTNODE_T:
			return obj_copy(obj_from_listnode(op1));
		default:
			error(TypeError, "cannot copy type %s", TYPENAME(op1));
	}
	return NULL;
}


/* op1 = (type op1) op2
 */
void obj_assign(Object *op1, Object *op2)
{
	switch (TYPE(op1)) {
		case CHAR_T:
			TYPEOBJ(op1)->set(op1, obj_as_char(op2));
			break;
		case INT_T:
			TYPEOBJ(op1)->set(op1, obj_as_int(op2));
			break;
		case FLOAT_T:
			TYPEOBJ(op1)->set(op1, obj_as_float(op2));
			break;
		case STR_T:
			TYPEOBJ(op1)->set(op1, obj_as_str(op2));
			break;
		case LIST_T:
			TYPEOBJ(op1)->set(op1, obj_as_list(op2));
			break;
		case LISTNODE_T:
			TYPEOBJ(op1)->set(op1, obj_copy(op2));
			break;
		default:
			error(TypeError, "unsupported operand type(s) for operation =: %s and %s", \
							  TYPENAME(op1), TYPENAME(op2));
	}
}


/* result = op1 + op2
 */
Object *obj_add(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_add(op1, op2);
	else if (isString(op1) || isString(op2))
		return str_concat(op1, op2);
	else if (isList(op1) && isList(op2))
		return list_concat((ListObject *)op1, (ListObject *)op2);
	else
		error(TypeError, "unsupported operand type(s) for operation +: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));
	return NULL;
}


/* result = op1 - op2
 */
Object *obj_sub(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_sub(op1, op2);
	else
		error(TypeError, "unsupported operand type(s) for operation -: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));
	return NULL;
}


/* result = op1 * op2
 */
Object *obj_mult(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_mul(op1, op2);
	else if ((isNumber(op1) || isNumber(op2)) && (isString(op1) || isString(op2)))
		return str_repeat(op1, op2);
	else if ((isNumber(op1) || isNumber(op2)) && (isList(op1) || isList(op2)))
		return list_repeat(op1, op2);
	else
		error(TypeError, "unsupported operand type(s) for operation *: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));
	return NULL;
}



/* result = op1 / op2
 */
Object *obj_divs(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_div(op1, op2);
	else
		error(TypeError, "unsupported operand type(s) for operation /: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));
	return NULL;
}



/* result = op1 % op2
 */
Object *obj_mod(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_mod(op1, op2);
	else
		error(TypeError, "unsupported operand type(s) for operation %%: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));

	return NULL;
}


/* result = 0 - op1
 */
Object *obj_invert(Object *op1)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;

	if (isNumber(op1))
		return number_inv(op1);
	else
		error(TypeError, "unsupported operand type for operation -: %s", \
						  TYPENAME(op1));
	return NULL;
}


/* result = (int_t)(op1 == op2)
 */
Object *obj_eql(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_eql(op1, op2);
	else if (isString(op1) && isString(op2))
		return str_eql(op1, op2);
	else if (isList(op1) && isList(op2))
		return list_eql((ListObject *)op1, (ListObject *)op2);
	else
		error(TypeError, "unsupported operand type(s) for operation ==: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));
	return NULL;
}


/* result = (int_t)(op1 != op2)
 */
Object *obj_neq(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_neq(op1, op2);
	else if (isString(op1) && isString(op2))
		return str_neq(op1, op2);
	else if (isList(op1) && isList(op2))
		return list_neq((ListObject *)op1, (ListObject *)op2);
	else
		error(TypeError, "unsupported operand type(s) for operation !=: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));
	return NULL;
}


/* result = (int_t)(op1 < op2)
 */
Object *obj_lss(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_lss(op1, op2);
	else
		error(TypeError, "unsupported operand type(s) for operation <: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));
	return NULL;
}


/* result = (int_t)(op1 <= op2)
 */
Object *obj_leq(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_leq(op1, op2);
	else
		error(TypeError, "unsupported operand type(s) for operation <=: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));
	return NULL;
}


/* result = (int_t)(op1 > op2)
 */
Object *obj_gtr(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_gtr(op1, op2);
	else
		error(TypeError, "unsupported operand type(s) for operation >: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));
	return NULL;
}


/* result = (int_t)(op1 >= op2)
 */
Object *obj_geq(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_geq(op1, op2);
	else
		error(TypeError, "unsupported operand type(s) for operation >=: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));
	return NULL;
}


/* result = (int_t)(op1 or op2)
 */
Object *obj_or(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_or(op1, op2);
	else
		error(TypeError, "unsupported operand type(s) for operation or: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));
	return NULL;
}


/* result = (int_t)(op1 and op2)
 */
Object *obj_and(Object *op1, Object *op2)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isNumber(op1) && isNumber(op2))
		return number_and(op1, op2);
	else
		error(TypeError, "unsupported operand type(s) for operation and: %s and %s", \
						  TYPENAME(op1), TYPENAME(op2));
	return NULL;
}


/* result = (int_t)(op1 in (sequence)op2)
 */
Object *obj_in(Object *op1, Object *op2)
{
	Object *result = NULL;
	int_t len;

	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;
	op2 = isListNode(op2) ? obj_from_listnode(op2) : op2;

	if (isSequence(op2) == 0)
		error(TypeError, "%s is not subscriptable", TYPENAME(op2));

	len = obj_length(op2);

	for (int_t i = 0; i < len; i++) {
		if (result != NULL)
			obj_decref(result);
		result = obj_eql(op1, obj_item(op2, i));
		if (obj_as_int(result) == 1)
			break;
	}
	return result;
}


/* result = (int_t)!op1
 */
Object *obj_negate(Object *op1)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;

	if (isNumber(op1))
		return number_negate(op1);
	else
		error(TypeError, "unsupported operand type for operation !: %s", TYPENAME(op1));

	return NULL;
}


/* item = list[index]
 * item = string[index]
 */
Object *obj_item(Object *sequence, int index)
{
	sequence = isListNode(sequence) ? obj_from_listnode(sequence) : sequence;

	if (TYPE(sequence) == STR_T)
		return (Object *)str_item((StrObject *)sequence, index);
	else if (TYPE(sequence) == LIST_T)
		return (Object *)list_item((ListObject *)sequence, index);
	else
		error(TypeError, "type %s is not subscriptable", TYPENAME(sequence));

	return NULL;
}


/* slice = list[start:end]
 * slice = string[start:end]
 */
Object *obj_slice(Object *sequence, int start, int end)
{
	sequence = isListNode(sequence) ? obj_from_listnode(sequence) : sequence;

	if (TYPE(sequence) == STR_T)
		return (Object *)str_slice((StrObject *)sequence, start, end);
	else if (TYPE(sequence) == LIST_T)
		return (Object *)list_slice((ListObject *)sequence, start, end);
	else
		error(TypeError, "type %s is not subscriptable", TYPENAME(sequence));

	return NULL;
}


/* Return number of items in a sequence.
 */
int_t obj_length(Object *sequence)
{
	sequence = isListNode(sequence) ? obj_from_listnode(sequence) : sequence;

	if (TYPE(sequence) == STR_T)
		return obj_as_int(str_length((StrObject *)sequence));
	else if (TYPE(sequence) == LIST_T)
		return obj_as_int(list_length((ListObject *)sequence));
	else
		error(TypeError, "type %s is not subscriptable", TYPENAME(sequence));

	return 0;
}


/* Return object type as string.
 */
Object *obj_type(Object *op1)
{
	return obj_create(STR_T, TYPENAME(op1));
}


/* result = (char_t)op1
 */
char_t obj_as_char(Object *op1)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;

	switch (TYPE(op1)) {
		case CHAR_T:
			return (char_t)((CharObject *)op1)->cval;
		case INT_T:
			return (char_t)((IntObject *)op1)->ival;
		case FLOAT_T:
			return (char_t)((FloatObject *)op1)->fval;
		default:
			error(ValueError, "cannot convert %s to char", TYPENAME(op1));
	}
	return 0;
}


/* result = (int_t)op1
 */
int_t obj_as_int(Object *op1)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;

	switch (TYPE(op1)) {
		case CHAR_T:
			return (int_t)((CharObject *)op1)->cval;
		case INT_T:
			return (int_t)((IntObject *)op1)->ival;
		case FLOAT_T:
			return (int_t)((FloatObject *)op1)->fval;
		default:
			error(ValueError, "cannot convert %s to integer", TYPENAME(op1));
	}
	return 0;
}


/* result = (float_t)op1
 */
float_t obj_as_float(Object *op1)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;

	switch (TYPE(op1)) {
		case CHAR_T:
			return (float_t)((CharObject *)op1)->cval;
		case INT_T:
			return (float_t)((IntObject *)op1)->ival;
		case FLOAT_T:
			return (float_t)((FloatObject *)op1)->fval;
		default:
			error(ValueError, "cannot convert %s to float", TYPENAME(op1));
	}
	return 0;
}


/* result = (str_t)op1
 */
char *obj_as_str(Object *op1)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;

	switch (TYPE(op1)) {
		case STR_T:
			return ((StrObject *)op1)->sptr;
		default:
			error(ValueError, "cannot convert %s to string", TYPENAME(op1));
	}
	return NULL;
}


/* result = (list_t)op1
 */
ListObject *obj_as_list(Object *op1)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;

	switch(TYPE(op1)) {
		case LIST_T:
			return (ListObject *)op1;
		default:
			error(ValueError, "cannot convert %s to list", TYPENAME(op1));
	}
	return NULL;
}


/* result = (bool_t)op1
 */
int obj_as_bool(Object *op1)
{
	op1 = isListNode(op1) ? obj_from_listnode(op1) : op1;

	switch (TYPE(op1)) {
		case CHAR_T:
			return obj_as_char(op1) ? 1 : 0;
		case INT_T:
			return obj_as_int(op1) ? 1 : 0;
		case FLOAT_T:
			return obj_as_float(op1) ? 1 : 0;
		default:
			error(ValueError, "cannot convert %s to bool", TYPENAME(op1));
	}
	return 0;
}


/* Convert string to a char_t
 */
char_t str_to_char(char *s)
{
	char_t c = 0;

	if (s[0] == '\\') {  /* is escape sequence */
		switch (s[1]) {
			case 'b' :	c = '\b'; break;
			case 'f' :	c = '\f'; break;
			case 'n' :	c = '\n'; break;
			case 'r' :	c = '\r'; break;
			case 't' :	c = '\t'; break;
			case 'v' :	c = '\v'; break;
			case '\\':	c = '\\'; break;
			case '\'':	c = '\''; break;
			case '\"':	c = '\"'; break;
			case '0' :	c = '\0'; break;
			default  :	error(ValueError, "unknown escape sequence: %c", s[1]);
		}
	} else  /* not an escape sequence */
		c = s[0];

	return c;
}


/* Convert string to an int_t
 */
int_t str_to_int(char *s)
{
	char *e;
	int_t i;

	errno = 0;

	i = strtol(s, &e, 10);

	if (*e != 0 || errno != 0) {
		if (errno != 0)
			error(ValueError, "cannot convert %s to int; %s", \
							   s, strerror(errno));
		else
			error(ValueError, "cannot convert %s to int", s);
	}
	return i;
}


/* Convert string to a float_t
 */
float_t str_to_float(char *s)
{
	char *e;
	float_t f;

	errno = 0;

	f = strtod(s, &e);

	if (*e != 0 || errno != 0) {
		if (errno != 0)
			error(ValueError, "cannot convert %s to float; %s", \
							   s, strerror(errno));
		else
			error(ValueError, "cannot convert %s to float", s);
	}
	return f;
}


/* Convert object obj to a string object
 */
Object *obj_to_strobj(Object *obj)
{
	char buffer[BUFSIZE+1];

	switch(TYPE(obj)) {
		case STR_T:
			obj_incref(obj);
			return obj;
		case CHAR_T:
			snprintf(buffer, BUFSIZE, "%c", obj_as_char(obj));
			return obj_create(STR_T, buffer);
		case INT_T:
			snprintf(buffer, BUFSIZE, "%ld", obj_as_int(obj));
			return obj_create(STR_T, buffer);
		case FLOAT_T:
			snprintf(buffer, BUFSIZE, "%.16lG", obj_as_float(obj));
			return obj_create(STR_T, buffer);
		case NONE_T:
			return obj_create(STR_T, "None");
		case POSITION_T:
			return obj_create(STR_T, "");
		default:
			return obj_create(STR_T, "");
	}
}


#if 0
/* Place string representation of f in buffer.
 */
char *float_to_str(float_t f, char *buffer, size_t buffersize)
{
	int count;

	count = snprintf(buffer, buffersize, "%.16lG", f);

	if (count == -1 || count == buffersize)
		error(ValueError, "error while printing");

	return buffer;
}
#endif
