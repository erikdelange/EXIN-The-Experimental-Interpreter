/* none.h
 *
 * 2016	K.W.E. de Lange
 */
#ifndef _NONE_
#define NONE

#include "object.h"

typedef struct {
	OBJ_HEAD;
} NoneObject;

extern TypeObject noneobject;

extern NoneObject none;

#endif
