/* number.h
 *
 * 2016 K.W.E. de Lange
 */
#ifndef _NUMBER_
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

typedef struct {
	OBJ_HEAD;
	objecttype_t numbertype;
	union {
		char_t cval;
		int_t ival;
		float_t fval;
	} value;
} NumberObject;

typedef struct {
	TYPE_HEAD;
} CharType;

extern CharType chartype;

typedef struct {
	TYPE_HEAD;
} IntType;

extern IntType inttype;

typedef struct {
	TYPE_HEAD;
} FloatType;

extern FloatType floattype;

typedef struct {
	TYPE_HEAD;
	Object *(*add)(Object *op1, Object *op2);
	Object *(*sub)(Object *op1, Object *op2);
	Object *(*mul)(Object *op1, Object *op2);
	Object *(*div)(Object *op1, Object *op2);
	Object *(*mod)(Object *op1, Object *op2);
	Object *(*inv)(Object *op1);
	Object *(*eql)(Object *op1, Object *op2);
	Object *(*neq)(Object *op1, Object *op2);
	Object *(*lss)(Object *op1, Object *op2);
	Object *(*leq)(Object *op1, Object *op2);
	Object *(*gtr)(Object *op1, Object *op2);
	Object *(*geq)(Object *op1, Object *op2);
	Object *(*or)(Object *op1, Object *op2);
	Object *(*and)(Object *op1, Object *op2);
	Object *(*negate)(Object *op1);
} NumberType;

extern NumberType numbertype;

#endif
