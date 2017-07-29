/*	error.c
 *
 *	Error handling.
 *
 *	1995	K.W.E. de Lange
 */
#include "exin.h"


/*	Table containing all error messages.
 *
 */
static struct {
	int number;
	char *description;
	int print_extra_info;
} errors[] = {  /* sort this list by number (see error.h) */
	{ NameError, "NameError", 1 },
	{ TypeError, "TypeError", 1 },
	{ SyntaxError, "SyntaxError", 1 },
	{ ValueError, "ValueError", 1 },
	{ SystemError, "SystemError", 1 },
	{ IndexError, "IndexError: index out of range", 0 },
	{ OutOfMemoryError, "Out of memory", 0 },
	{ ModNotAllowedError, "ModNotAllowedError", 1 },
	{ DivisionByZeroError, "DivisionByZeroError: division by zero", 0 },
};


/*	Display an error message and stop the interpreter.
 *
 * 	number	error number (see error.h)
 * 	...		optional printf style format string, optionally followed by arguments
 *
 * 	error(TypeError, "%s is not subscriptable", TYPENAME(sequence));
 *
 */
void error(int number, ...)
{
	char *format;
	va_list argp;

	va_start(argp, number);

	if (reader.m && reader.m->name)
		fprintf(stderr, "File %s", reader.m->name);

	reader.print_current_line();

	if (number < 0 || number > (int)(sizeof errors / sizeof errors[0]) - 1)
		error(SystemError, "unknown error number %d", number);

	fprintf(stderr, "%s", errors[number].description);

	if (errors[number].print_extra_info == 1) {
		fprintf(stderr, ": ");
		format = va_arg(argp, char *);
		if (format)
			vfprintf(stderr, format, argp);
		fprintf(stderr, "\n");
	}

	va_end(argp);

	exit(number);
}
