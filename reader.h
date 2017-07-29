/*	reader.h
 *
 *	2017	K.W.E. de Lange
 */
#if !defined (_READER_)
#define _READER_

#include "position.h"
#include "module.h"

typedef struct reader {
	struct module *m;		/* now reading from this module */
	char *rp;				/* read pointer = next character to read */

	int (*nextch)(void);
	int (*peekch)(void);
	int (*pushch)(int);

	void (*reset)(void);
	void (*to_bol)(void);

	struct positionobject *(*save)(void);
	void (*jump)(struct positionobject *);

	void (*print_current_line)(void);
} Reader;

extern Reader reader;

#endif
