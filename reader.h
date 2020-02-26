/* reader.h
 *
 * 2018	K.W.E. de Lange
 */
#ifndef _READER_
#define _READER_

#include "module.h"

typedef struct reader {
	struct module *current;	/* currently reading from this module */
	char *pos;				/* position of next character to read */
	char *bol;				/* position of beginning of current line */

	int (*nextch)(void);	/* read the next character */
	int (*peekch)(void);	/* peek the next character */
	int (*pushch)(int);		/* push character back in the input stream */
	void (*to_bol)(void);	/* move to beginning of current line */
	void (*reset)(void);	/* reset reader to line 1, character 1 */

	void (*init)(struct reader *);			/* initialize reader struct */
	struct positionobject *(*save)(void);	/* save current reader */
	void (*jump)(struct positionobject *);	/* load current reader */
	int (*import)(const char *filename);	/* import new module */
	void (*print_current_line)(void);		/* print current line */
} Reader;

extern Reader reader;

#endif
