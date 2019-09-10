/* function.c
 *
 * Builtin (aka intrinsic) functions.
 *
 * 2019	K.W.E. de Lange
 */
#include <string.h>
#include "function.h"


/* Builtin: determine the type of an expression
 *
 * in:	token = LPAR of argument list
 * out:	token = token after RPAR of function call argument list
 *
 * Syntax: type(expression)
 */
 static Object *type(void)
 {
	Object *obj, *result;

	expect(LPAR);
	obj = assignment_expr();
	expect(RPAR);

	result = isListNode(obj) ? obj_type(obj_from_listnode(obj)) : obj_type(obj);

	obj_decref(obj);

	return result;
}


/* Builtin: return string representation of integer
 *
 * in:	token = LPAR of argument list
 * out:	token = token after RPAR of function call argument list
 *
 * Syntax: chr(integer expression)
 */
 static Object *chr(void)
 {
	Object *obj, *result;
	char buffer[BUFSIZE+1];

	expect(LPAR);
	obj = assignment_expr();
	expect(RPAR);

	snprintf(buffer, BUFSIZE, "%c", obj_as_char(obj));
	result = obj_create(STR_T, buffer);  // VERBETER: 10 als \n printen

	obj_decref(obj);

	return result;
}


/*	Table containing all builtin function names and their addresses.
 */
static struct {
	char *functionname;
	Object *(*functionaddr)();
} builtinTable[] = { /* Note: functionnames must be sorted alphabetically */
	{"chr", chr},
	{"type", type}
};


/* Check if functionname is an builtin function, and if so execute it.
 *
 * functionname	identifier to check for builtin function
 * return		Object* with function results if functionname
 * 				was a builtin else NULL
 */
Object *builtin(char *functionname)
{
	int l, h, m, d;

	l = 0, h = (int)(sizeof builtinTable / sizeof builtinTable[0]) - 1;

	while (l <= h) {
		m = (l + h) / 2;
		d = strcmp(&functionname[0], builtinTable[m].functionname);
		if (d < 0)
			h = m - 1;
		if (d > 0)
			l = m + 1;
		if (d == 0)
			break;
	};

	if (d == 0) {
		expect(IDENTIFIER);
		return builtinTable[m].functionaddr();
	} else
		return NULL;
}
