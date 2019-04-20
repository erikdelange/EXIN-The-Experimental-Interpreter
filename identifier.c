/* identifier.c
 *
 * Identifier management.
 *
 * Identifiers are names which refer to variables or functions. An identifier
 * has a certain scope. At any moment only two scope levels are relevant; the
 * global scope at program level, and the local scope within the currently
 * executed function. When entering a function a new (lowest) scope level
 * is created.
 *
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
static Identifier *search(const char *name)
{
	Identifier *id;

	if ((id = searchIdentifierInScope(local, name)) == NULL)
		id = searchIdentifierInScope(global, name);

	return id;
}


/* Create a new identifier in a specific scope list.
 *
 * level    list in which to add the identifier
 * name     identifier name
 * return   *Identifier object or NULL if the identifier already exists
 */
static Identifier *addIdentifier(Scope *level, const char *name)
{
	Identifier *id = NULL;

	if ((searchIdentifierInScope(level, name)) == NULL) {
		if ((id = calloc(1, sizeof(Identifier))) == NULL)
			error(OutOfMemoryError);

		*id = identifier;

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
 static Identifier *add(const char *name)
 {
	return addIdentifier(local, name);
 }


/*	Unbind an object and an identifier.
 */
static void unbind(Identifier *self)
{
	debug_printf(DEBUGALLOC, "\nunbind: %s, %p", self->name, (void *)self->object);

	if (self->object) {
		obj_decref(self->object);
		self->object = NULL;
	}
}


/* Bind an object to an identifier. First remove an existing binding (if any).
 */
static void bind(Identifier *self, Object *obj)
{
	if (self->object)
		unbind(self);

	debug_printf(DEBUGALLOC, "\nbind  : %s, %p", self->name, (void *)obj);

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
static void appendScopeLevel(void)
{
	Scope *level;

	if ((level = calloc(1, sizeof(Scope))) == NULL)
		error(OutOfMemoryError);

	*level = scope;

	level->parent = local;

	local = level;
	local->first = NULL;
	local->indentlevel = 0;
	local->indentation[0] = 0;
}


/* Remove the lowest level from the scope hierachy.
 *
 * Also releases all identifiers and the objects linked to them.
 */
static void removeScopeLevel(void)
{
	Identifier *id, *next;
	Scope *level;

	if (local != global) {
		level = local;
		local = level->parent;
		for (id = level->first; id; ) {
			next = id->next;
			removeIdentifier(id);
			id = next;
		}
		free(level);
	}
}


#ifdef DEBUG
/*  Print identifiers per level to a semi-colon separated file.
 *  Note: changes previous stdout
 */
void dump_identifier(void)
{
	int n;
	FILE *fp;
	Scope *level;
	Identifier *id;

	for (level = local, n = 0; level; level = level->parent, n++)
		;

	if ((fp = freopen("identifier.dsv", "w", stdout)) != NULL) {
		printf("level;name;object\n");
		for (level = local; level; level = level->parent) {
			for (id = level->first; id; id = id->next) {
				printf("%d;%s;", n, id->name);
				if (id->object != NULL)
					printf("%p", (void *)id->object);
				printf("\n");
			}
		}
		fclose(fp);
	}
}
#endif


/* The identifier API.
 */
Identifier identifier = {
	.name = NULL,
	.next = NULL,
	.object = NULL,

	.add = add,
	.search = search,
	.bind = bind,
	.unbind = unbind
};


/* The scope API.
 */
Scope scope = {
	.parent = NULL,
	.first = NULL,
	.indentlevel = 0,
	.indentation[0] = 0,

	.append_level = appendScopeLevel,
	.remove_level = removeScopeLevel
};