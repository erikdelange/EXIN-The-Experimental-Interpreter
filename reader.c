/*	reader.c
 *
 * 	The reader read character from the source code. The reader can also jump to
 * 	other places in the code. The reader contains a pointer to the module
 * 	object from which it is currently reading. (See also reader.h).
 *	Only one global reader object existst.
 *
 *	2017	K.W.E. de Lange
 */
#include "exin.h"


/*	Forward declarations.
 *
 */
static int nextch(void);
static int peekch(void);
static int pushch(int ch);
static void reset(void);
static void to_bol(void);
static PositionObject *save(void);
static void	jump(PositionObject *pos);
static void	print_current_line(void);


/*	Reader API
 *
 */
Reader reader = {
	NULL,
	NULL,
	nextch,
	peekch,
	pushch,
	reset,
	to_bol,
	save,
	jump,
	print_current_line
};


/*	Read the next character.
 *
 */
static int nextch(void)
{
	if (*reader.rp == 0)
		return EOF;
	else
		return (unsigned int)*reader.rp++;
}


/*	See what the next character is, but don't read it.
 *
 */
static int peekch(void)
{
	if (*reader.rp == 0)
		return EOF;
	else
		return (unsigned int)*reader.rp;
}


/*	Undo the read of a character.
 *
 *	Note: this implementation only set the read pointer back one position and
 *	does nothing with ch
 *
 */
static int pushch(int ch)
{
	if (reader.rp > reader.m->code)
		reader.rp--;
	return ch;
}


/*	Set the read pointer at the start of the code.
 *
 */
static void reset(void)
{
	scanner.init();
	reader.rp = reader.m->code;
}


/*	Move the read pointer ro the start of the current line.
 *
 */
static void to_bol(void)
{
	/* Note: this actually reverses rp to the previous NEWLINE */
	while (*reader.rp != '\n' && reader.rp > reader.m->code)
		reader.rp--;
}


/*	Jump to location 'position' in the code.
 *
 */
static void jump(PositionObject *position)
{
	reader.m = position->reader.m;
	reader.rp = position->reader.rp;

	scanner.jump(&position->scanner);
}


/*	Store the current postion of the reader
 *	(and the current status of the scanner).
 *
 */
static PositionObject *save(void)
{
	PositionObject *pos;

	pos = (PositionObject *)obj_alloc(POSITION_T);

	/* save relevant reader variabeles */
	pos->reader.m = reader.m;
	pos->reader.rp = reader.rp;

	/* save relevant scanner variabeles */
	scanner.save(&pos->scanner);

	return pos;
}


/*	Display the current line in the code.
 *
 */
static void print_current_line(void)
{
	char *p;
	int	line;

	if (reader.rp != NULL) {  /* is NULL before first module is loaded */
		to_bol();

		for (p = reader.m->code, line = 1; p <= reader.rp; p++)
			if (*p == '\n')
				line++;

		fprintf(stderr, ", line %d\n", line);

		while (*p && *p != '\n')
			fprintf(stderr, "%c", *p++);  /* print line */
	}
	fprintf(stderr, "\n");
}
