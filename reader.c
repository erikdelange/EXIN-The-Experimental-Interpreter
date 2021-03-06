/* reader.c
 *
 * The reader object reads characters from the source code. It can also
 * jump to other places in the code. The reader contains a pointer to the
 * module object from which it is currently reading. (See also reader.h).
 * Only one (and thus global) reader object exists.
 *
 * 2018	K.W.E. de Lange
 */
#include <assert.h>
#include <setjmp.h>
#include <string.h>
#include <stdio.h>

#include "scanner.h"
#include "parser.h"
#include "reader.h"
#include "error.h"


/* API: Read the next character.
 *
 * return   the character read
 */
static int nextch(void)
{
	if (*reader.pos == 0) {
		reader.bol = reader.pos;
		return EOF;
	} else {
		if (reader.pos > reader.current->code && *(reader.pos - 1) == '\n')
			reader.bol = reader.pos;
		return (unsigned int)*reader.pos++;
	}
}


/* API: Look ahead to see what the next character is, but don't read it.
 *
 * return   the next character to read
 */
static int peekch(void)
{
	if (*reader.pos == 0)
		return EOF;
	else
		return (unsigned int)*reader.pos;
}


/* API: Undo the read of a character.
 *
 * ch       the character to push back into the input stream
 * return   the character which was pushed back
 *
 * Note: this implementation only puts the read pointer back one position
 * and does nothing with ch. Properly this should be done via a stack.
 */
static int pushch(const int ch)
{
	if (reader.pos > reader.current->code && ch != EOF) {
		reader.pos--;
		assert(*reader.pos == (char)ch);
	}
	return ch;
}


/* API: Set the read pointer to the start of the code.
 */
static void reset(void)
{
	scanner.init(&scanner);

	reader.pos = reader.current->code;
	reader.bol = reader.current->code;
}


/* API: Move the read pointer to the start of the current line.
 */
static void to_bol(void)
{
	reader.pos = reader.bol;
}


/* API: Jump to location 'position' in the code. This can be in another module.
 *
 * position location where to jump to
 */
static void reader_jump(PositionObject *position)
{
	assert(position != NULL);

	reader = position->reader;
	scanner = position->scanner;
}


/* API: Store the current position of the reader and status of the scanner.
 *
 * return   pointer to position object containing current read position
 */
static PositionObject *reader_save(void)
{
	PositionObject *pos;

	if ((pos = (PositionObject *)obj_alloc(POSITION_T)) == NULL)
		error(OutOfMemoryError);

	pos->reader = reader;
	pos->scanner = scanner;

	return pos;
}


/* API: Display the code from the currently executed line.
 */
static void print_current_line(void)
{
	char *p;
	int	line;

	if (reader.pos != NULL) {
		to_bol();

		/* determine line number */
		for (p = reader.current->code, line = 1; *p && p != reader.bol; p++)
			if (*p == '\n')
				line++;

		fprintf(stderr, ", line %d\n", line);

		/* print line */
		while (*p && *p != '\n')
			fprintf(stderr, "%c", *p++);
	}
	fprintf(stderr, "\n");
}


/* API: Load a module and execute the code.
 *
 * filename     filename of module to load and execute
 * return		0 or integer return value argument from return statement
 */
static int import(const char *filename)
{
	assert(filename != NULL);
	assert(*filename != '\0');

	if (module.search(filename) != NULL)
		return 0;  /* importing a module will only be done once */

	reader.current = module.new(filename);
	reader.reset();

	return parser();
}


/* API: Initialize reader object 'rd'.
 *
 * rd       pointer to reader object
 */
static void reader_init(struct reader *rd)
{
	assert(rd != NULL);

	/* load the function addresses from the global reader */
	*rd = reader;

	/* reset all object variables to their initial states */
	rd->current = NULL;
	rd->pos = NULL;
	rd->bol = NULL;
}


/* Reader API and data, including the initial settings.
 */
Reader reader = {
	.current = NULL,
	.pos = NULL,
	.bol = NULL,

	.nextch = nextch,
	.peekch = peekch,
	.pushch = pushch,
	.to_bol = to_bol,
	.reset = reset,
	.import = import,
	.init = reader_init,
	.save = reader_save,
	.jump = reader_jump,
	.print_current_line = print_current_line
	};
