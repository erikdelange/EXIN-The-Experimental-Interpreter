/* position.h
 *
 * 2018	K.W.E. de Lange
 */
#ifndef _POSITION_
#define _POSITION_

#include "object.h"
#include "reader.h"
#include "scanner.h"

typedef struct positionobject {
	OBJ_HEAD;
	struct reader reader;		/* stores relevant parts of reader */
	struct scanner scanner;		/* stores relevant parts of scanner */
} PositionObject;

typedef struct {
	TYPE_HEAD;
} PositionType;

extern PositionType positiontype;

#endif
