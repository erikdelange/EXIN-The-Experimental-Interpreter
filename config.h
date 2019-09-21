/* config.h
 *
 * Configuration constants.
 *
 * 2018	K.W.E. de Lange
 */
#ifndef _CONFIG_
#define _CONFIG_

#include <setjmp.h>
#include <stdio.h>

#define LANGUAGE	"EXIN"
#define VERSION		"1.11"
#define BUFSIZE		256		/* maximum length of identifier name excl '\0' */
#define LINESIZE	256		/* maximum length of input line excl '\0' */
#define MAXINDENT	132		/* maximum number of indents */
#define TABSIZE		4		/* default spaces per tab */

#define char_t	char		/* basic type for CHAR_T */
#define int_t	long		/* basic type for INT_T */
#define float_t	double		/* basic type for FLOAT_T */

/* 	All global configuration variables.
 */
typedef struct {
	int debug;      /* debug logging level */
	int tabsize;    /* spaces per tab */
} Config;

extern Config config;

extern jmp_buf return_address;

/*	Define preprocessor macro DEBUG in the compiler options to enable
 *	debug logging.
 */
#ifdef DEBUG
	#define debug_printf(level, fmt, ...) \
				do { \
					if (config.debug & (level)) { \
						fprintf(stdout, fmt, __VA_ARGS__); \
						fflush(stdout); \
					} \
				} while (0)
#else  /* not DEBUG */
	#define debug_printf(level, fmt, ...) \
				do { } while (0)
#endif

/* debug logging detail levels
 */
#define NODEBUG         0	/* no debug output */
#define DEBUGTOKEN      1	/* show tokens during execution */
#define DEBUGBLOCK      2	/* show function and block entry & exit */
#define DEBUGALLOC      4	/* show object alloc() & free() * (un)bind() */
#define DEBUGSCANONLY   8	/* show tokens during function scan */
#define DEBUGDUMP       16	/* dump identifiers and objects at end of program */

#endif
