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


/*	API: Read the next character.
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


/*	API: Look ahead to see what the next character is, but don't read it.
 */
static int peekch(void)
{
	if (*reader.pos == 0)
		return EOF;
	else
		return (unsigned int)*reader.pos;
}


/*	API: Undo the read of a character.
 *
 *	Note: this implementation only sets the read pointer back one position
 *	and does nothing with ch. Properly this should be done via a stack.
 */
static int pushch(int ch)
{
	if (reader.pos > reader.current->code && ch != EOF) {
		reader.pos--;
		assert(*reader.pos == (char)ch);
	}
	return ch;
}


/*	API: Set the read pointer to the start of the code.
 */
static void reset(void)
{
	scanner.init(&scanner);

	reader.pos = reader.current->code;
	reader.bol = reader.current->code;
}


/*	API: Move the read pointer to the start of the current line.
 */
static void to_bol(void)
{
	reader.pos = reader.bol;
}


/*	API: Jump to location 'position' in the code. This can be in another module.
 */
static void reader_jump(PositionObject *position)
{
	reader = position->reader;
	scanner = position->scanner;
}


/*	API: Store the current position of the reader and status of the scanner.
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


/*	API: Display the code from the currently executed line.
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


/*	API: Load a module and execute the code.
 */
static void import(const char *filename)
{
	jmp_buf temp;

	if (module.search(filename) != NULL)
		return;  /* importing a module will only be done once */

	reader.current = module.new(filename);
	reader.reset();

	/* Direct struct copy by assignment (s1 = s2) does not work for jmp_buf
	 * in gcc 8.1.0 (don't know why) so memcpy is needed instead.
	 */
	memcpy(&temp, &return_address, sizeof(jmp_buf));
	if (setjmp(return_address) == 0)
		parser();
	memcpy(&return_address, &temp, sizeof(jmp_buf));
}


/*	API: Initialize reader object 'rd'.
 */
static void reader_init(struct reader *rd)
{
	/* load the function addresses from the global reader */
	*rd = reader;

	/* reset all object variables to their initial states */
	rd->current = NULL;
	rd->pos = NULL;
	rd->bol = NULL;
}


/*	Reader API and data, including the initial settings.
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
