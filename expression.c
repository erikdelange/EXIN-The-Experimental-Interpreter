/* expression.c
 *
 * Expression evaluator
 *
 * Evaluates an expression recursively in the following order:
 *
 * - first variables (including subscripts and slices), constants,
 *   then function calls, object methods and parenthesized expressions,
 * - then the unary operators + - and !
 * - then multiplication and division (normal and modulo)
 * - then addition and subtraction
 * - then the comparisons < <= > and >=
 * - then the comparisons == and !=
 * - then logical and
 * - then logical or
 * - then assignment of values (normal and compound)
 * - and finally comma separated statements
 *
 * The outcome is an object which contains the result.
 *
 * 1995 K.W.E. de Lange
 */
#include <limits.h>
#include <string.h>

#include "expression.h"
#include "identifier.h"
#include "position.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"
#include "str.h"


static Object *logical_or_expr(void);


/* Decode the next expression and convert to integer.
 *
 * To be used when subscript indices must be read.
 */
static int int_expression()
{
	Object *obj;
	int i;

	obj = logical_or_expr();
	i = obj_as_int(obj);
	obj_decref(obj);

	return i;
}


/* Decode subscripts [index] and [start:end] for sequences.
 *
 * Index is mandatory, start en end are optional. The result can be another
 * sequence with subscripts. Decoding continues until no subscripts are left.
 *
 * The opening LSQB of the subscript has already been read.
 *
 * Return: new reference (count = 1)
 *         for LIST: LISTNODE for index or LIST for slice
 *         for STR: CHAR for index or STR for slice
 */
static Object *subscript(Object *sequence)
{
	Object *lvalue, *rvalue, *original = sequence;
	enum { INDEX, SLICE } type = INDEX;
	int index = 0, start = 0, end = 0;

	if (!isSequence(sequence))
		error(TypeError, "%s is not subscriptable", TYPENAME(sequence));

	rvalue = sequence;

	while (1) {
		sequence = isListNode(rvalue) ? obj_from_listnode(rvalue) : rvalue;

		type = INDEX;

		if (accept(COLON)) {
			start = 0;
			type = SLICE;
		} else {
			start = index = int_expression();
		}
		if (accept(COLON)) {
			type = SLICE;
		}
		if (accept(RSQB)) {
			if (type == SLICE)
				end = INT_MAX;
		} else {
			end = int_expression();
			expect(RSQB);
		}
		if (type == INDEX)
			lvalue = obj_item((Object *)sequence, index);
		else
			lvalue = obj_slice((Object *)sequence, start, end);

		if (lvalue == NULL)
			error(IndexError);

		if (accept(LSQB)) {
			if (rvalue != original)
				obj_decref(rvalue);
			rvalue = lvalue;
		} else
			break;
	}
	return lvalue;
}


/* Call methods: seq.len, seq.append, seq.remove, seq.insert, *.type
 *
 * The DOT which indicates a method will follow has already been read.
 *
 * Return: new reference (with count = 1)
 */
static Object *method(Object *object)
{
	int index;
	Object *obj = NULL;

	object = isListNode(object) ? obj_from_listnode(object) : object;

	/* If an object has many methods then the approach used below is not
	 * very efficient and must be rewritten. */
	if (scanner.token == IDENTIFIER) {
		if (TYPE(object) == LIST_T && strcmp("insert", scanner.string) == 0) {
			expect(IDENTIFIER);
			expect(LPAR);
			index = int_expression();
			expect(COMMA);
			obj = logical_or_expr();
			listnode_insert((ListObject *)object, index, obj_copy(obj));
			obj_decref(obj);
			obj = obj_alloc(NONE_T);
			expect(RPAR);
		} else if (TYPE(object) == LIST_T && strcmp("append", scanner.string) == 0) {
			expect(IDENTIFIER);
			expect(LPAR);
			obj = logical_or_expr();
			listnode_append((ListObject *)object, obj_copy(obj));
			obj_decref(obj);
			obj = obj_alloc(NONE_T);
			expect(RPAR);
		} else if (TYPE(object) == LIST_T && strcmp("remove", scanner.string) == 0) {
			expect(IDENTIFIER);
			expect(LPAR);
			index = int_expression();
			if ((obj = listnode_remove((ListObject *)object, index)) == NULL)
				error(IndexError);
			expect(RPAR);
		} else if (TYPE(object) == LIST_T && strcmp("len", scanner.string) == 0) {
			expect(IDENTIFIER);
			obj = list_length((ListObject *)object);
		} else if (TYPE(object) == STR_T && strcmp("len", scanner.string) == 0) {
			expect(IDENTIFIER);
			obj = str_length((StrObject *)object);
		} else if (strcmp("type", scanner.string) == 0) {
			expect(IDENTIFIER);
			obj = obj_type(object);
		} else
			error(SyntaxError, "unknown method %s for type %s", scanner.string, TYPENAME(object));
	} else
		error(SyntaxError, "expected method for type %s", TYPENAME(object));

	return obj;
}


/* Evaluate the part of an expression which comes after the identifier,
 * function call or constant.
 *
 * Contains: subscripts and methods. Methods may follow subscripts:
 * e.g. "abc"[:].len returns 3
 *
 * Return: obj (reference count not increased) if there is no trailer, or
 *         new reference (count = 1)
 */
static Object *trailer(Object *obj)
{
	Object *rv = NULL;

	if (accept(LSQB)) {  /* is subscript */
		rv = subscript(obj);
		obj_decref(obj);
		obj = rv;
	}
	if (accept(DOT)) {  /* is method */
		rv = method(obj);
		obj_decref(obj);
	}
	if (rv == NULL)  /* not a subscript and not a method */
		rv = obj;

	return rv;
}


/* Evaluate variables, function calls, constants, (expression)
 *
 * Return: object with reference count +1
 */
static Object *primary_expr(void)
{
	Identifier *id;
	Object *obj = NULL, *tmp;

	switch (scanner.token) {
		case CHAR:  /* CHAR constant */
			obj = obj_create(CHAR_T, str_to_char(scanner.string));
			expect(CHAR);
			break;
		case INT:   /* INT constant */
			obj = obj_create(INT_T, str_to_int(scanner.string));
			expect(INT);
			break;
		case FLOAT:  /* FLOAT constant */
			obj = obj_create(FLOAT_T, str_to_float(scanner.string));
			expect(FLOAT);
			break;
		case STR:   /* STR constant */
			obj = obj_create(STR_T, scanner.string);
			expect(STR);
			break;
		case LSQB:  /* LIST constant */
			obj = obj_alloc(LIST_T);
			expect(LSQB);
			while (accept(RSQB) == 0) {
				do {
					tmp = assignment_expr();
					listnode_append((ListObject *)obj, obj_copy(tmp));
					obj_decref(tmp);
				} while (accept(COMMA));
			}
			break;
		case IDENTIFIER:  /* variabele or function identifier */
			if ((id = identifier.search(scanner.string)) == NULL)
				error(NameError, "identifier %s is not defined", scanner.string);
			expect(IDENTIFIER);
			if (TYPE(id->object) == POSITION_T) {
				obj = function_call((PositionObject *)id->object);
			} else {
				obj = id->object;
				obj_incref(obj);
			}
			break;
		case LPAR:  /* parenthesized expression */
			expect(LPAR);
			obj = comma_expr();
			expect(RPAR);
			break;
		default:
			error(SyntaxError, "expression expected");
	}
	return trailer(obj);
}


/* Operators: (unary)-  (unary)+  ! (logical negation, NOT)
 */
static Object *unary_expr(void)
{
	Object *lvalue, *result;

	if (accept(NOT)) {
		result = primary_expr();
		lvalue = obj_negate(result);
		obj_decref(result);
	} else if (accept(MINUS)) {
		result = primary_expr();
		lvalue = obj_invert(result);
		obj_decref(result);
	} else if (accept(PLUS)) {
		lvalue = primary_expr();
	} else
		lvalue = primary_expr();

	return lvalue;
}


/* Operators: *  /  %
 */
static Object *mult_expr(void)
{
	Object *lvalue, *rvalue, *result;

	lvalue = unary_expr();

	while (1)
		if (accept(STAR)) {
			rvalue = mult_expr();
			result = obj_mult(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else if (accept(SLASH)) {
			rvalue = mult_expr();
			result = obj_divs(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else if (accept(PERCENT)) {
			rvalue = mult_expr();
			result = obj_mod(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else
			return lvalue;
}


/* Operators: +  -
 */
static Object *additive_expr(void)
{
	Object *lvalue, *rvalue, *result;

	lvalue = mult_expr();

	while (1)
		if (accept(PLUS)) {
			rvalue = additive_expr();
			result = obj_add(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else if (accept(MINUS)) {
			rvalue = additive_expr();
			result = obj_sub(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else
			return lvalue;
}


/* Operators: <  <=  >  >=
 */
static Object *relational_expr(void)
{
	Object *lvalue, *rvalue, *result;

	lvalue = additive_expr();

	while (1)
		if (accept(LESS)) {
			rvalue = relational_expr();
			result = obj_lss(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else if (accept(LESSEQUAL)) {
			rvalue = relational_expr();
			result = obj_leq(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else if (accept(GREATER)) {
			rvalue = relational_expr();
			result = obj_gtr(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else if (accept(GREATEREQUAL)) {
			rvalue = relational_expr();
			result = obj_geq(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else
			return lvalue;
}


/* Operators: ==  !=  <>  in 
 */
static Object *equality_expr(void)
{
	Object *lvalue, *rvalue, *result;

	lvalue = relational_expr();

	while (1)
		if (accept(EQEQUAL)) {
			rvalue = equality_expr();
			result = obj_eql(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else if (accept(NOTEQUAL)) {
			rvalue = equality_expr();
			result = obj_neq(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else if (accept(IN)) {
			rvalue = equality_expr();
			result = obj_in(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else
			return lvalue;
}


/* Operators: logical and
 */
static Object *logical_and_expr(void)
{
	Object *lvalue, *rvalue, *result;

	lvalue = equality_expr();

	while (1)
		if (accept(AND)) {
			rvalue = logical_and_expr();
			result= obj_and(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else
			return lvalue;
}


/* Operators: logical or
 */
static Object *logical_or_expr(void)
{
	Object *lvalue, *rvalue, *result;

	lvalue = logical_and_expr();

	while (1)
		if (accept(OR)) {
			rvalue = logical_or_expr();
			result = obj_or(lvalue, rvalue);
			obj_decref(lvalue);
			obj_decref(rvalue);
			lvalue = result;
		} else
			return lvalue;
}


/* Operators: =  +=  -=  *=  /=  %=
 */
Object *assignment_expr(void)
{
	Object *lvalue, *rvalue, *result;

	lvalue = logical_or_expr();

	while (1)
		if (accept(EQUAL)) {
			rvalue = assignment_expr();
			obj_assign(lvalue, rvalue);
			obj_decref(rvalue);
		} else if (accept(PLUSEQUAL)) {
			rvalue = logical_or_expr();
			result = obj_add(lvalue, rvalue);
			obj_assign(lvalue, result);
			obj_decref(rvalue);
			obj_decref(result);
		} else if (accept(MINUSEQUAL)) {
			rvalue = logical_or_expr();
			result = obj_sub(lvalue, rvalue);
			obj_assign(lvalue, result);
			obj_decref(rvalue);
			obj_decref(result);
		} else if (accept(STAREQUAL)) {
			rvalue = logical_or_expr();
			result = obj_mult(lvalue, rvalue);
			obj_assign(lvalue, result);
			obj_decref(rvalue);
			obj_decref(result);
		} else if (accept(SLASHEQUAL)) {
			rvalue = logical_or_expr();
			result = obj_divs(lvalue, rvalue);
			obj_assign(lvalue, result);
			obj_decref(rvalue);
			obj_decref(result);
		} else if (accept(PERCENTEQUAL)) {
			rvalue = logical_or_expr();
			result = obj_mod(lvalue, rvalue);
			obj_assign(lvalue, result);
			obj_decref(rvalue);
			obj_decref(result);
		} else
			return lvalue;
}


/* Operators: ,
 *
 * Multiple expressions separated by comma's. Returns an object with the
 * result from the last expression.
 */
Object *comma_expr(void)
{
	Object *lvalue;

	lvalue = assignment_expr();

	while (1)
		if (accept(COMMA)) {
			obj_decref(lvalue);
			lvalue = comma_expr();
		} else
			return lvalue;

}
