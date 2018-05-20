/* module.c
 *
 * Code is grouped in files called modules. Modules are loaded via the
 * module.new() function. Every module object contains a reference to
 * the loaded code of that module. Module objects are stored in a
 * singly linked list starting at 'modulehead'.
 *
 * 1995	K.W.E. de Lange
 */
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include "module.h"
#include "error.h"


/* Pointer to the list of loaded modules
 */
static Module *modulehead = NULL;


/* Search a module in the list of loaded modules.
 *
 * name		filename of module (may include path)
 * return	module object or NULL if not found
 */
static Module *search(const char *name)
{
	Module *m;

	for (m = modulehead; m; m = m->next)
		if (strcmp(name, m->name) == 0)
			break;

	return m;

}


/* Load the code for a module.
 *
 * self		pointer to module object
 * name		filename (may include path)
 * return	1 if succesfull else 0
 */
static int load(Module *self, const char *name)
{
	FILE *fp;
	struct _stat stat_buffer;

	assert(self != NULL);

	if (_stat(name, &stat_buffer) == 0) {
		self->size = stat_buffer.st_size;
		if ((self->code = calloc(self->size + 1, sizeof(char))) != NULL) {
			if ((fp = fopen(name, "r")) != NULL) {
				self->size = fread(self->code, sizeof(char), self->size, fp);
				fclose(fp);
				self->code[self->size] = 0;
				return 1;
			} else {
				free(self->code);
				self->code = NULL;
			}
		}
	}
	return 0;
}


/* Create a new module and load the code.
 *
 * name		module's filename (may include path)
 * return	module object (else program fails)
 */
static Module *new(const char *name)
{
	Module *m;

	if ((m = calloc(1, sizeof(Module))) == NULL)
		error(OutOfMemoryError);
	else
		*m = module;

	if (load(m, name) == 0)
		error(SystemError, "error importing %s: %s (%d)", name, strerror(errno), errno);

	if ((m->name = strdup(name)) == NULL)
		error(OutOfMemoryError);

	m->next = modulehead;
	modulehead = m;

	return m;
}


/*	The module API.
 */
Module module = {
	.next = NULL,
	.name = NULL,
	.code = NULL,
	.size = 0,

	.new = new,
	.search = search
};
