/* parser.h
 *
 * 2018	K.W.E. de Lange
 */
#ifndef _PARSER_
#define _PARSER_

#include "object.h"
#include "scanner.h"
#include "position.h"

extern int accept(token_t t);
extern int expect(token_t t);
extern int parser(void);
extern Object *function_call(PositionObject *pos);

#endif
