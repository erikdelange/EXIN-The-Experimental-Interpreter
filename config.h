/*	config.h
 *
 * 	Configuration constants.
 *
 * 	1994	K.W.E. de Lange
 */
#if !defined (_CONFIG_)
#define _CONFIG_

#define VERSION	"1.0"

#define LINESIZE 256		/* maximum length of command line for interpreter */

#define MAXINDENT 64		/* maximum number of indents */

#define TABSIZE 8			/* spaces per tab. Also set this in your editor */

#define	BUFSIZE	256			/* maximum length of identifier name */

#define	char_t	char		/* Basic type for CHAR_T */
#define	int_t	long		/* Basic type for INT_T	*/
#define	float_t	double		/* Basic type for FLOAT_T */

typedef enum { false = 0, true } bool_t;

/* Define DEBUG in the compiler options to enable debug logging.
 *
 */
#ifdef DEBUG
	#define debug_printf(level, fmt, ...) \
				do { \
					if (debug >= level) \
						fprintf(stderr, fmt, __VA_ARGS__); } \
				while (0)
#else
	#define debug_printf(level, fmt, ...) \
			do { } while (0)
#endif

							/* debug logging detail levels: */
#define DEBUGLEVEL0	0		/* no debug output */
#define DEBUGLEVEL1	1		/* tokens only */
#define DEBUGLEVEL2	2		/* L1 + function and block entry & exit */
#define	DEBUGLEVEL3	3		/* L2 + object alloc & free */

#endif
