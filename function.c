/* function.c
 *
 * Builtin functions.
 *
 * 2019	K.W.E. de Lange
 */
#include <string.h>
#include "function.h"


/* Builtin: determine the type of an expression
 *
 * in:	token is LPAR of argument list
 * out:	token = token after RPAR of function call argument list
 *
 * Syntax: type(expression)
 */
 static Object *type(void) {
	Object *obj, *result;

	expect(LPAR);
	obj = comma_expr();
	expect(RPAR);
	result = obj_type(obj);
	obj_decref(obj);

	return result;
}


/*	Table containing all builtin function names and their addresses.
 */
static struct {
	char *functionname;
	Object *(*functionaddr)();
} builtinTable[] = { /* Note: sort functionname strings alphabetically */
	{"type", type}
};


/* Check if functionname is an builtin function, and if so execute it.
 *
 * functionname		identifier to check for builtin
 * return			Object* if functionname was a builtin else NULL
 */
Object *builtin(char *functioname) {
	int l, h, m, d;

	l = 0, h = (int)(sizeof builtinTable / sizeof builtinTable[0]) - 1;

	while (l <= h) {
		m = (l + h) / 2;
		d = strcmp(&functioname[0], builtinTable[m].functionname);
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
