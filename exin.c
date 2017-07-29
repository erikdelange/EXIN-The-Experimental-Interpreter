/*	exin.c
 *
 *	Experimental Interpreter EXIN (or XI)
 *
 * 	This is the interpreters main program.
 *
 *	1994	K.W.E. de Lange
 */
#include "exin.h"


int debug = DEBUGLEVEL0;


/*	Forward declarations.
 *
 */
static int execute(char *filename);
static void interpret(void);
static void evaluate(void);


int	main(int argc, char **argv)
{
	/*	Command line flags.
	 *
	 */
	int opt_evaluate = 0;
	int opt_interpret = 0;

	char *executable;
	int c;

	executable = basename(argv[0]);

	while (--argc > 0 && (*++argv)[0] == '-')
		while ((c = *++argv[0]))
			switch (c) {
				#ifdef DEBUG
				case 'd':
					if (isdigit((*argv)[1]))
						debug = *++argv[0] - '0';
					else
						debug = 1;
					break;
				#endif
				case 'e':
					opt_evaluate = 1;
					break;
				case 'i':
					opt_interpret = 1;
					break;
				default:
					fprintf(stderr, "%s: unknown option %c\n", executable, c);
					break;
			}

	if (((opt_evaluate || opt_interpret) && argc != 0) ||
		 (!(opt_evaluate || opt_interpret) && argc != 1)) {
		fprintf(stderr, "EXIN version %s\n", VERSION);
		fprintf(stderr, "usage: %s [options] [file]\n", executable);
		fprintf(stderr, "options\n");
		#ifdef DEBUG
		fprintf(stderr, "-d[detail] = show debug info\n");
		fprintf(stderr, "   detail = 1..9 (little - much)\n");
		#endif
		fprintf(stderr, "-i = interpret line as statement\n");
		fprintf(stderr, "-e = evaluate line as expression\n");
	} else {
		if (opt_evaluate)
			evaluate();
		else if (opt_interpret)
			interpret();
		else
			return execute(*argv);
	}
	return 0;
}


/*	Load program and start interpreter.
 *
 */
static int execute(char *filename)
{
	module.import(filename);

	if (return_value && isNumber(return_value))
		return obj_as_int(return_value);

	return 1;
}


/*	Read line from stdin and evaluate as expression.
 *
 */
static void evaluate(void)
{
	char line[LINESIZE];
	Object *obj;

	module.code = &line[0];
	reader.m = &module;

	printf("Enter expression followed by Enter, Ctrl-Z to stop");

	while (1) {
		printf("\n>>> ");
		if (fgets(module.code, (size_t)LINESIZE, stdin) != NULL) {
			reader.reset();
			if (scanner.next() != ENDMARKER) {
				printf("= ");
				obj = comma_expr();
				obj_print(obj);
				obj_decref(obj);
			}
		} else
			break;
	}
}


/*	Read line from stdin and execute as statement.
 *
 */
static void interpret(void)
{
	char line[LINESIZE];

	module.code = &line[0];
	reader.m = &module;

	printf("Enter statements followed by Enter, Ctrl-Z to stop");

	if (setjmp(return_address) == 0) {
		while (1) {
			printf("\n>>> ");
			if (fgets(module.code, (size_t)LINESIZE, stdin) != NULL) {
				reader.reset();
				if (scanner.next() != ENDMARKER)
					statement();
			} else
				break;
		}
	}
}
