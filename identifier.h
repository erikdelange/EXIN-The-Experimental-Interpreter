/*	identifier.h
 *
 * 	1994	K.W.E. de Lange
 */
#if !defined (_IDENTIFIER_)
#define _IDENTIFIER_

#include "object.h"

typedef struct identifier {
	char *name;
	struct identifier *next;
	struct object *object;
} Identifier;

typedef struct scope {
	struct scope *parent;
	Identifier *first;
	int level;
	int indentation[MAXINDENT];
} Scope;

#define SCOPE_INIT	{ NULL, NULL, 0, { 0 } }

extern Scope *global;
extern Scope *local;

extern void appendScopeLevel(void);
extern void removeScopeLevel(void);
extern Identifier *addIdentifier(char *name, Scope *level);
extern Identifier *searchIdentifier(char *name);
extern void bind(Identifier *id, Object *obj);

#endif

