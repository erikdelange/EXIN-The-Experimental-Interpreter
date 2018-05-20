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

extern TypeObject strobject;

extern Object *str_length(StrObject *obj);
extern CharObject *str_item(StrObject *str, int index);
extern StrObject *str_slice(StrObject *obj, int start, int end);
extern Object *str_concat(Object *op1, Object *op2);
extern Object *str_repeat(Object *op1, Object *op2);
extern Object *str_eql(Object *op1, Object *op2);
extern Object *str_neq(Object *op1, Object *op2);

#endif
