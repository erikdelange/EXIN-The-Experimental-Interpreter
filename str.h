/* str.h
 *
 * 2016 K.W.E. de Lange
 */
#ifndef _STR_
#define _STR_
#include "object.h"
#include "number.h"

typedef struct {
	OBJ_HEAD;
	char *sptr;
} StrObject;

typedef struct {
	TYPE_HEAD;
	Object *(*length)(StrObject *obj);
	CharObject *(*item)(StrObject *str, int index);
	StrObject *(*slice)(StrObject *obj, int start, int end);
	Object *(*concat)(Object *op1, Object *op2);
	Object *(*repeat)(Object *op1, Object *op2);
	Object *(*eql)(Object *op1, Object *op2);
	Object *(*neq)(Object *op1, Object *op2);
} StrType;

extern StrType strtype;

#endif
