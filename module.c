/*	module.c
 *
 *	EXIN code is grouped in files called modules. Modules are loaded via the
 *	module.import() function. Every module object contains a reference to the
 *	loaded code of that module.
 *
 *	1995	K.W.E. de Lange
 */
#include <sys/types.h>
#include <sys/stat.h>
#include "exin.h"


/*	Forward declarations.
 *
 */
static struct module *import(char *filename);
static struct module *searchModule(char *name);


/*	An empty module object, used as 1. the starting point for the list of
 *	loaded modules and 2. the module API.
 *
 */
Module module = {
	NULL,
	"shell",
	"",
	import
};


/*	Determine file size.
 *
 *	filename	filename optionally including path
 *	return		file size in bytes or 0 in case of error
 *
 */
static size_t fileSize(char *filename)
{
	struct _stat stat_buffer;

	if (_stat(filename, &stat_buffer) == 0)
		return stat_buffer.st_size;
	return 0;
}


/*	Allocate buffer and read file.
 *
 *	filename	filename optionally including path
 *	return		buffer containing code or NULL in case of error
 *
 */
static char *loadFile(char *filename)
{
	FILE *fp;
	char *buffer;
	size_t bytes;

	if ((bytes = fileSize(filename)) != 0)
		if ((buffer = (char *)calloc(bytes + 1, sizeof(char))) != NULL)
			if ((fp = fopen(filename, "r")) != NULL) {
				fread(buffer, sizeof(char), bytes, fp);
				fclose(fp);
				buffer[bytes] = 0;
				return buffer;
			}

	return NULL;
}


/*	Search a module in the list of loaded modules.
 *
 *	filename	filename optionally including path
 * 	return		module-object or NULL if not found
 *
 */
static struct module *searchModule(char *filename)
{
	Module *m;

	for (m = module.next; m; m = m->next)
		if (strcmp(filename, m->name) == 0)
			break;

	return m;

}


/*	Import a module ans start interpreting the module's code.
 *
 * 	filename	filename optionally including path
 * 	return		new module-object (interpreter stops in case of error)
 *
 */
static Module *import(char *filename)
{
	Module *m;

	if ((m = searchModule(filename)) == NULL) {  /* check if already loaded */

		m = calloc((size_t)1, sizeof(Module));
		if (m == NULL)
			error(OutOfMemoryError);

		m->code = loadFile(filename);
		if (m->code == NULL)
			error(SystemError, "error importing %s", filename);

		m->name = strdup(filename);
		if (m->name == NULL)
			error(OutOfMemoryError);

		m->next = module.next;
		module.next = m;
	}

	reader.m = m;
	reader.reset();

	if (setjmp(return_address) == 0)
		parser();

	return m;
}
