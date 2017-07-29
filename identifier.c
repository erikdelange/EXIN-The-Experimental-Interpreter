/*	identifier.c
 *
 *	Identifier management.
 *
 * 	Identifiers are names which refer to variables or functions. An identifier
 * 	has a certain scope. When entering a function a new (lowest) scope level
 * 	is created.
 *
 * 	At any moment only two scope levels are relevant; the global scope at
 * 	program level, and the local scope within the currently executed function.
 * 	For example an identifier 'alpha' may occur in all levels in a scope
 * 	hierarchy. However the name is only searched at local, and if not found
 * 	there, at global level.
 *
 * 	Identifiers are stored in a singly linked list. 'Next' points to the next
 * 	identifier. Every list starts at a header (of type 'Scope'). These headers
 * 	are also stored in a singly linked list. 'Parent' always points to the next
 * 	higher scope level. The highest (= global) level always has a header, called
 * 	'top'. Lower levels are created when needed. Global variables 'global' and
 * 	'local' provide quick access to - respectively - the highest and lowest
 * 	levels in the scope hierarchy.
 *
 *	1994	K.W.E. de Lange
 */
#include "exin.h"


static Scope top = SCOPE_INIT;	/* head of global identifier list */
Scope *global = &top;			/* initially global ... */
Scope *local = &top;			/* ... and local scope are the same */


static Identifier *searchIdentifierInScope(char *name, Scope *level);
static void removeIdentifier(Identifier *id);
static void unbind(Identifier *id);


/*	Append a new lowest level to the scope hierarchy.
 *
 */
void appendScopeLevel(void)
{
	Scope *scope;

	scope = calloc((size_t)1, sizeof(Scope));

	if (scope == NULL)
		error(OutOfMemoryError);

	scope->parent = local;

	local = scope;
	local->first = NULL;
	local->level = 0;
	local->indentation[0] = 0;
}


/*	Remove the lowest level from the scope hierachy.
 *
 *	Also releases all identifiers and objects linked to them.
 *
 */
void removeScopeLevel(void)
{
	Identifier *id, *next;
	Scope *scope;

	if (local != global) {
		scope = local;
		local = scope->parent;
		for (id = scope->first; id; ) {
			next = id->next;
			removeIdentifier(id);
			id = next;
		}
		free(scope);
	}
}


/*	Search an identifier, first at local then at global level.
 *
 *	name	identifier name
 *	return	*Identifier or NULL if not found
 *
 */
Identifier *searchIdentifier(char *name)
{
	Identifier *id;

	if ((id = searchIdentifierInScope(name, local)) == NULL)
		id = searchIdentifierInScope(name, global);

	return id;
}


/*	Search an identifier in a specific scope list.
 *
 *	name	identifier name
 *	level	list to search
 *	return	*Identifier or NULL if not found
 *
 */
static Identifier *searchIdentifierInScope(char *name, Scope *level)
{
	Identifier *id;

	for (id = level->first; id; id = id->next)
		if (strcmp(name, id->name) == 0)
			break;

	return id;
}


/*	Create a new identifier in a specific scope list.
 *
 *	name	identifier name
 * 	level	list to which to add the identifier
 *	return	*Identifier or NULL if the identifier already existed
 *
 */
Identifier *addIdentifier(char *name, Scope *level)
{
	Identifier *id = NULL;

	if ((searchIdentifierInScope(name, level)) == NULL) {
		id = calloc((size_t)1, sizeof(Identifier));
		if (id != NULL) {
			id->next = level->first;
			level->first = id;
			if ((id->name = strdup(name)) == NULL)
				error(OutOfMemoryError);
			id->object = NULL;
		} else
			error(OutOfMemoryError);
	}
	return id;
}


/*	Remove an identifier and free the allocated memory.
 *
 *	Als remove the link with the object.
 *
 */
static void removeIdentifier(Identifier *id)
{
	unbind(id);
	free(id->name);
	free(id);
}


/*	Bind an object to an identifier. First remove an existing binding.
 *
 */
void bind(Identifier *id, Object *obj)
{
	if (id->object)
		unbind(id);

	id->object = obj;
}


/*	Unbind an object and an identifier.
 *
 */
static void unbind(Identifier *id)
{
	if (id->object) {
		obj_decref(id->object);
		id->object = NULL;
	}
}
