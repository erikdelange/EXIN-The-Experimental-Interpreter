/* main.c
 *
 * This is the interpreters main program.
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


jmp_buf	return_address;			/* Return adress at end of function */
Object *return_value = NULL;	/* Result of the last function call */

Config config = {
	.debug = DEBUGLEVEL0,
	.tabsize = TABSIZE
};


/* Print a usage message.
 */
static void usage(char *executable)
{
	fprintf(stderr, "\n%s version %s\n", executable, VERSION);
	fprintf(stderr, "usage: %s [options] file\n", executable);
	fprintf(stderr, "options\n");
	#ifdef DEBUG
	fprintf(stderr, "-d[detail] = show debug info\n");
	fprintf(stderr, "    detail = 1..9 (little - much)\n");
	#endif
	fprintf(stderr, "-h = show usage information\n");
	fprintf(stderr, "-t[tabsize] = set tab size in spaces\n");
	fprintf(stderr, "    tabsize = >= 1\n");
}


int	main(int argc, char **argv)
{
	int ch;
	char *executable = basename(argv[0]);

	while (--argc > 0 && (*++argv)[0] == '-')
		while ((ch = *++argv[0]))
			switch (ch) {
				#ifdef DEBUG
				case 'd':
					if (isdigit((*argv)[1]))
						config.debug = *++argv[0] - '0';
					else
						config.debug = DEBUGLEVEL1;
					break;
				#endif
				case 'h':
					usage(executable);
					return 0;
				case 't':
					if (isdigit((*argv)[1]))
						config.tabsize = atoi(*argv);
					else
						config.tabsize = TABSIZE;
					break;
				default:
					fprintf(stderr, "%s: unknown option %c\n", executable, ch);
					usage(executable);
					return 0;
			}

	if (argc == 0) {
		fprintf(stderr, "%s: module name missing\n", executable);
		usage(executable);
	} else if (argc == 1) {
		reader.import(*argv);
		if (return_value && isNumber(return_value))
			return obj_as_int(return_value);
	} else {
		fprintf(stderr, "%s: to many modules\n", executable);
		usage(executable);
	}
	return 0;
}
