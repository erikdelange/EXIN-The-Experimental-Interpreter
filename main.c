/* main.c
 *
 * The interpreters main program. Handles command line arguments
 * and starts execution of the code in the (first) module specified
 * on the command line.
 *
 * 2018	K.W.E. de Lange
 */
#include <ctype.h>
#include <stdio.h>
#include <libgen.h>
#include <stdlib.h>

#include "parser.h"
#include "object.h"
#include "reader.h"
#include "config.h"


jmp_buf	return_address;			/* Return address at end of function */
Object	*return_value = NULL;	/* Result of the last function call */

Config config = {				/* global configuration variables */
	.debug = NODEBUG,
	.tabsize = TABSIZE
};


/* Print the usage message to stream (normally stdout or stderr).
 */
static void usage(char *executable, FILE *stream)
{
	fprintf(stream, "\n%s version %s\n", LANGUAGE, VERSION);
	fprintf(stream, "usage: %s [options] module\n", executable);
	fprintf(stream, "module: name of file containing code to execute\n");
	fprintf(stream, "options\n");
	#ifdef DEBUG
	fprintf(stream, "-d[detail] = show debug info\n");
	fprintf(stream, "    detail = sum of options (default = 1)\n");
	fprintf(stream, "    option 0: no debug output\n");
	fprintf(stream, "    option 1: show tokens during execution\n");
	fprintf(stream, "    option 2: show block entry and exit\n");
	fprintf(stream, "    option 4: show memory allocation\n");
	fprintf(stream, "    option 8: show tokens during function scan\n");
	fprintf(stream, "    option 16: dump identifier and object table to disk after program end\n");
	#endif  /* DEBUG */
	fprintf(stream, "-h = show usage information\n");
	fprintf(stream, "-t[tabsize] = set tab size in spaces\n");
	fprintf(stream, "    tabsize = >= 1 (default = %d)\n", TABSIZE);
	fprintf(stream, "-v = show version information\n");
}


/* The interpreter starts here.
 *
 * For command line options see function usage().
 */
int	main(int argc, char **argv)
{
	char ch;
	char *executable = basename(*argv);

	/* decode flags on the command line */
	while (--argc > 0 && (*++argv)[0] == '-') {
		ch = *++argv[0];
		switch (ch) {
			#ifdef DEBUG
			case 'd':
				if (isdigit(*++argv[0]))
					config.debug = (int)str_to_int(&(*argv[0]));
				else
					config.debug = DEBUGTOKEN;
				break;
			#endif  /* DEBUG */
			case 'h':
				usage(executable, stdout);
				return 0;
			case 't':
				if (isdigit(*++argv[0])) {
					config.tabsize = (int)str_to_int(&(*argv[0]));
					if (config.tabsize < 1) {
						fprintf(stderr, "%s: invalid tabsize %d\n", \
										executable, config.tabsize);
						config.tabsize = TABSIZE;
					}
				} else
					config.tabsize = TABSIZE;
				break;
			case 'v':
				fprintf(stdout, "%s version %s\n", LANGUAGE, VERSION);
				return 0;
			default:
				fprintf(stderr, "%s: unknown option -%c\n", executable, ch);
				usage(executable, stderr);
				return 0;
		}
	}

	if (argc == 0) {
		fprintf(stderr, "%s: module name missing\n", executable);
		usage(executable, stderr);
	} else if (argc == 1) {
		reader.import(*argv);

		#ifdef DEBUG
		void dump_identifier(void);
		void dump_object(void);

		if (config.debug & DEBUGDUMP) {
			dump_identifier();
			dump_object();
		}
		#endif  /* DEBUG */

		if (return_value && isNumber(return_value))
			return obj_as_int(return_value);
	} else {  /* more then 1 argument */
		fprintf(stderr, "%s: to many modules\n", executable);
		usage(executable, stderr);
	}
	return 0;
}
