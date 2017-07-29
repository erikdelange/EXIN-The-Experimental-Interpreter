/*	module.h
 *
 *	1995	K.W.E. de Lange
 */
#if !defined (_MODULE_)
#define _MODULE_

/*	This struct is the API to a module object, containing both data and
 *	function adresses.
 *
 * 	Function import() loads a module and starts parsing the code.
 *
 */
typedef struct module {
	struct module *next;	/* private - list with loaded modules */
	char *name;				/* module name */
	char *code;  			/* buffer containing module code */

	struct module *(*import)(char *filename);	/* actually a class function */
} Module;

extern Module module;

#endif
