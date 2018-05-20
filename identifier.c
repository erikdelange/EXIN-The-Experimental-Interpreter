/* identifier.c
 *
 * Identifier management.
 *
 * Identifiers are names which refer to variables or functions. An identifier
 * has a certain scope. When entering a function a new (lowest) scope level
 * is created.
 *
 * At any moment only two scope levels are relevant; the global scope at
 * program level, and the local scope within the currently executed function.
 * For example an identifier 'alpha' may occur in all levels in a scope
 * hierarchy. However the name is only searched at local, and if not found
 * there, at global level.
 *
 * Identifiers are stored in a singly linked list. 'Next' points to the next
 * identifier. Every list starts at a header (of type 'Scope'). These headers
 * are also stored in a singly linked list. 'Parent' always points to the next
 * higher scope level. The highest (= global) level always has a header, called
 * 'top'. Lower levels are created when needed. Global variables 'global' and
 * 'local' provide quick access to respectively the highest and lowest
 * levels in the scope hierarchy.
 *
 *	1994 K.W.E. de Lange
 */
#include <stdlib.h>
#include <string.h>

#include "identifier.h"
#include "error.h"


static Scope top = SCOPE_INIT;	/* head of global identifier list */

static Scope *global = &top;	/* initially global ... */
	   Scope *local = &top;		/* ... and local scope are the same */


/* Search an identifier in a specific scope list.
 *
 * level    list to search
 * name     identifier name
 * return   *Identifier object or NULL if not found
 */
static Identifier *searchIdentifierInScope(const Scope *level, const char *name)
{
	Identifier *id;

	for (id = level->first; id; id = id->next)
		if (strcmp(name, id->name) == 0)
			break;

	return id;
}


/* Search an identifier, first at local then at global level.
 *
 * name     identifier name
 * return   *Identifier object or NULL if not found
 */
Identifier *search(const char *name)
{
	Identifier *id;

	if ((id = searchIdentifierInScope(local, name)) == NULL)
		id = searchIdentifierInScope(global, name);

	return id;
}


/* Create a new identifier in a specific scope list.
 *
 * level    list to which to add the identifier
 * name     identifier name
 * return   *Identifier object or NULL if the identifier already existed
 */
static Identifier *addIdentifier(Scope *level, const char *name)
{
	Identifier *id = NULL;

	if ((searchIdentifierInScope(level, name)) == NULL) {
		if ((id = calloc(1, sizeof(Identifier))) == NULL)
			error(OutOfMemoryError);

		id->next = level->first;
		level->first = id;
		if ((id->name = strdup(name)) == NULL)
			error(OutOfMemoryError);
		id->object = NULL;
	}
	return id;
}


/* Add an identifier to the local scope.
 */
 Identifier *add(const char *name)
 {
	return addIdentifier(local, name);
 }


/*	Unbind an object and an identifier.
 */
static void unbind(Identifier *self)
{
	if (self->object) {
		obj_decref(self->object);
		self->object = NULL;
	}
}


/* Bind an object to an identifier. First remove an existing binding.
 */
void bind(Identifier *self, Object *obj)
{
	if (self->object)
		unbind(self);

	self->object = obj;
}


/* Remove an identifier and free the allocated memory.
 *
 * Also removes the link with the object.
 */
static void removeIdentifier(Identifier *id)
{
	unbind(id);
	free(id->name);
	free(id);
}


/* Append a new lowest level to the scope hierarchy.
 */
void appendScopeLevel(void)
{
	Scope *scope;

	if ((scope = calloc(1, sizeof(Scope))) == NULL)
		error(OutOfMemoryError);

	scope->parent = local;

	local = scope;
	local->first = NULL;
	local->level = 0;
	local->indentation[0] = 0;
}


/* Remove the lowest level from the scope hierachy.
 *
 * Also releases all identifiers and the objects linked to them.
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


/* The identifier API.
 */
Identifier identifier = {
	.name = NULL,
	.next = NULL,
	.object = NULL,
	.add = add,
	.search = search,
	.bind = bind
};


/* The scope API.
 */
Scope scope = {
	.parent = NULL,
	.first = NULL,
	.level = 0,
	.append_level = appendScopeLevel,
	.remove_level = removeScopeLevel
};