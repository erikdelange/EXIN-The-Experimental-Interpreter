/* config.h
 *
 * Configuration constants.
 *
 * 2018	K.W.E. de Lange
 */
#ifndef _CONFIG_
#define _CONFIG_

#include <stdbool.h>
#include <setjmp.h>
#include <stdio.h>

#define VERSION		"1.07"
#define	BUFSIZE		256		/* maximum length of identifier name excl '\0' */
#define LINESIZE 	256		/* maximum length of input line excl '\0' */
#define MAXINDENT 	132		/* maximum number of indents */
#define TABSIZE 	4		/* default spaces per tab */

#define	char_t	char		/* Basic type for CHAR_T */
#define	int_t	long		/* Basic type for INT_T */
#define	float_t	double		/* Basic type for FLOAT_T */

/* 	Struct containing all global configuration variables.
 */
typedef struct {
	int debug;
	int tabsize;
} Config;

extern Config config;

extern jmp_buf return_address;

/*	Define preprocessor macro DEBUG in the compiler options to enable
 *	debug logging.
 */
#ifdef DEBUG
	#define debug_printf(level, fmt, ...) \
				do { \
					if (config.debug >= level) \
						fprintf(stderr, fmt, __VA_ARGS__); } \
				while (0)
#else
	#define debug_printf(level, fmt, ...) \
			do { } while (0)
#endif

/* debug logging detail levels:
 */
#define DEBUGLEVEL0	0		/* no debug output */
#define DEBUGLEVEL1	1		/* tokens only */
#define DEBUGLEVEL2	2		/* LEVEL1 + function and block entry & exit */
#define	DEBUGLEVEL3	3		/* LEVEL2 + object alloc() & free() * (un)bind() */

#endif
