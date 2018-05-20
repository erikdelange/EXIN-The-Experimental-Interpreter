/* module.h
 *
 * 2018	K.W.E. de Lange
 */
#ifndef _MODULE_
#define _MODULE_

#include <stddef.h>

/* This struct is the API to a module object, containing both data and
 * function adresses.
 *
 * Function new() loads a new module. Function search() looks for a module
 * in the list of loaded modules.
 */
typedef struct module {
	struct module *next;	/* next module in list with loaded modules */
	char *name;				/* module name */
	char *code;  			/* buffer containing module code */
	size_t size;			/* number of bytes in code buffer */

	struct module *(*new)(const char *name);	/* load new module */
	struct module *(*search)(const char *name);	/* search for loaded module */
} Module;

extern Module module;

#endif
