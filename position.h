/*	position.h
 *
 *	1994	K.W.E. de Lange
 */
#if !defined (_POSITION_)
#define _POSITION_

#include "object.h"
#include "reader.h"
#include "scanner.h"

typedef struct positionobject {
	OBJ_HEAD;
	struct reader reader;		/* stores relevant parts of reader */
	struct scanner scanner;		/* stores relevant parts of scanner */
} PositionObject;

extern TypeObject posobject;

#endif
