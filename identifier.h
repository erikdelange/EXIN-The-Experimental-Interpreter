/* identifier.h
 *
 * 1994	K.W.E. de Lange
 */
#ifndef _IDENTIFIER_
#define _IDENTIFIER_

#include "object.h"

typedef struct identifier {
	char *name;
	struct identifier *next;
	struct object *object;
	struct identifier *(*add)(const char *name);
	struct identifier *(*search)(const char *name);
	void (*bind)(struct identifier *self, Object *o);
	void (*unbind)(struct identifier *self);
} Identifier;

extern Identifier identifier;

typedef struct scope {
	struct scope *parent;
	Identifier *first;
	int indentlevel;
	int indentation[MAXINDENT];
	void (*append_level)(void);
	void (*remove_level)(void);
} Scope;

extern Scope scope;

#define SCOPE_INIT { .parent = NULL, .first = NULL, .indentlevel = 0, .indentation[0] = 0 }

extern Scope *local;

#endif

