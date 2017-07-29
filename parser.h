/*	parser.h
 *
 *	1995	K.W.E. de Lange
 */
#if !defined(_PARSER_)
#define _PARSER_

#include <setjmp.h>
#include "object.h"
#include "scanner.h"
#include "position.h"

extern jmp_buf return_address;
extern Object *return_value;
extern int accept(token_t t);
extern int expect(token_t t);
extern void parser(void);
extern void statement(void);
extern Object *function_call(PositionObject *pos);

#endif
