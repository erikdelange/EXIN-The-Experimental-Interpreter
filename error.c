/* error.c
 *
 * Error handling.
 *
 * 1995	K.W.E. de Lange
 */
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "reader.h"
#include "error.h"


/* Table containing all error messages. No sorting needed.
 */
static struct {
	int number;
	char *description;
	int print_extra_info;
} errors[] = {
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


/* Display an error message and stop the interpreter.
 *
 * number	error number (see error.h)
 * ...		optional printf style format string, optionally followed by arguments
 *
 * error(TypeError, "%s is not subscriptable", TYPENAME(sequence));
 */
void error(int number, ...)
{
	int i = 0;
	char *format;
	va_list argp;

	va_start(argp, number);

	while (1) {
		if (errors[i].number == number)
			break;
		if (i++ <= (int)(sizeof errors / sizeof errors[0]) - 1)
			continue;
		error(SystemError, "unknown error number %d", number);
	}

	if (reader.current) {
		if (reader.current->name)
			fprintf(stderr, "File %s", reader.current->name);
		reader.print_current_line();
	}

	fprintf(stderr, "%s", errors[i].description);

	if (errors[i].print_extra_info == 1) {
		fprintf(stderr, ": ");
		format = va_arg(argp, char *);
		if (format)
			vfprintf(stderr, format, argp);
	}
	fprintf(stderr, "\n");

	va_end(argp);

	exit(number);
}
