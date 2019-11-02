/* none.h
 *
 * 2016	K.W.E. de Lange
 */
#ifndef _NONE_
#define _NONE_

#include "object.h"

typedef struct {
	OBJ_HEAD;
} NoneObject;

typedef struct {
	TYPE_HEAD;
} NoneType;

extern NoneType nonetype;

#endif
