/*	number.h
 *
 * 	2016	K.W.E. de Lange
 */
#if !defined (_NUMBER_)
#define _NUMBER_

#include "object.h"

typedef struct {
	OBJ_HEAD;
	char_t cval;
} CharObject;

typedef struct {
	OBJ_HEAD;
	int_t ival;
} IntObject;

typedef struct {
	OBJ_HEAD;
	float_t fval;
} FloatObject;

extern TypeObject charobject;
extern TypeObject intobject;
extern TypeObject floatobject;

extern Object *number_add(Object *op1, Object *op2);
extern Object *number_sub(Object *op1, Object *op2);
extern Object *number_mul(Object *op1, Object *op2);
extern Object *number_div(Object *op1, Object *op2);
extern Object *number_mod(Object *op1, Object *op2);
extern Object *number_inv(Object *op1);

extern Object *number_eql(Object *op1, Object *op2);
extern Object *number_neq(Object *op1, Object *op2);
extern Object *number_lss(Object *op1, Object *op2);
extern Object *number_leq(Object *op1, Object *op2);
extern Object *number_gtr(Object *op1, Object *op2);
extern Object *number_geq(Object *op1, Object *op2);

extern Object *number_or(Object *op1, Object *op2);
extern Object *number_and(Object *op1, Object *op2);

extern Object *number_negate(Object *op1);

#endif
