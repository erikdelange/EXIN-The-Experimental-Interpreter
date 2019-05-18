/* main.c
 *
 * The interpreters main program. Handles command line arguments
 * and starts execution of the code in the first module specified
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

Config config = {
	.debug = NODEBUG,
	.tabsize = TABSIZE
};


/* Print the usage message.
 */
static void usage(char *executable)
{
	fprintf(stderr, "\n%s version %s\n", LANGUAGE, VERSION);
	fprintf(stderr, "usage: %s [options] file\n", executable);
	fprintf(stderr, "file: code to execute\n");
	fprintf(stderr, "options\n");
	#ifdef DEBUG
	fprintf(stderr, "-d[detail] = show debug info\n");
	fprintf(stderr, "    detail = sum of options (default = 1)\n");
	fprintf(stderr, "    option 0: no debug output\n");
	fprintf(stderr, "    option 1: show tokens during execution\n");
	fprintf(stderr, "    option 2: show block entry and exit\n");
	fprintf(stderr, "    option 4: show memory allocation\n");
	fprintf(stderr, "    option 8: show tokens during function scan\n");
	fprintf(stderr, "    option 16: dump identifier and object table to disk\n");
	#endif
	fprintf(stderr, "-h = show usage information\n");
	fprintf(stderr, "-t[tabsize] = set tab size in spaces\n");
	fprintf(stderr, "    tabsize = >= 1 (default = %d)\n", TABSIZE);
	fprintf(stderr, "-v = show version information\n");
}


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
			#endif
			case 'h':
				usage(executable);
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
				usage(executable);
				return 0;
		}
	}

	if (argc == 0) {
		fprintf(stderr, "%s: module name missing\n", executable);
		usage(executable);
	} else if (argc == 1) {
		reader.import(*argv);

		#ifdef DEBUG
		void dump_identifier(void);
		void dump_object(void);

		if (config.debug & DEBUGDUMP) {
			dump_identifier();
			dump_object();
		}
		#endif

		if (return_value && isNumber(return_value))
			return obj_as_int(return_value);
	} else {
		fprintf(stderr, "%s: to many modules\n", executable);
		usage(executable);
	}
	return 0;
}
