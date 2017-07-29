/*	expression.h
 *
 *	1995	K.W.E. de Lange
 */
#if !defined(_EXPRESSION_)
#define _EXPRESSION_

#include "object.h"

extern Object *comma_expr();
extern Object *assignment_expr(void);

#endif
