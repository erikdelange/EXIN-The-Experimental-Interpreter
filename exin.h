/*	exin.h
 *
 * 	Generic definitions for the experimental interpreter (EXIN).
 *
 * 	1994	K.W.E. de Lange
 */
#if !defined (_EXIN_)
#define _EXIN_

#include <libgen.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <limits.h>
#include <stdio.h>
#include <ctype.h>
#include <float.h>

#include "expression.h"
#include "identifier.h"
#include "position.h"
#include "scanner.h"
#include "parser.h"
#include "object.h"
#include "number.h"
#include "config.h"
#include "module.h"
#include "reader.h"
#include "error.h"
#include "none.h"
#include "list.h"
#include "str.h"

/* strndup.c */
extern char *strndup(const char *s, size_t n);

/* 	exin.c
 *
 * 	Struct holding all global variables.
 *
 */
typedef struct {
	int debug;
	int tabsize;
} Exin;

extern Exin exin;

#endif
