/*	scanner.h
 *
 * 	1994	K.W.E. de Lange
 */
#if !defined (_SCANNER_)
#define _SCANNER_

typedef enum { 	UNKNOWN=0, CHAR, INT, FLOAT, STR, STAR, SLASH,
				PLUS, MINUS, EQEQUAL, NOTEQUAL, LESS, LESSEQUAL, GREATER,
				GREATEREQUAL, COMMA, RPAR, ELSE, DO, LPAR, EQUAL, NUMBER,
				IDENTIFIER, IF, WHILE, INPUT, PRINT, DEFCHAR, DEFINT,
				DEFFLOAT, DEFSTR, DEFFUNC, DOT, ENDMARKER, RETURN, PERCENT,
				AND, OR, PLUSEQUAL, MINUSEQUAL, STAREQUAL, SLASHEQUAL,
				PERCENTEQUAL, NOT, LSQB, RSQB, NEWLINE, INDENT, DEDENT,
				PASS, BREAK, CONTINUE, DEFLIST, COLON, IMPORT } token_t;


static inline char *tokenName(token_t t)  /* inline requires at least C99 */
{
	static char *string[] = {
	"UNKNOWN TOKEN", "CHARACTER LITERAL", "INTEGER LITERAL", "FLOAT LITERAL",
	"STRING LITERAL", "STAR", "DIV", "PLUS", "MINUS", "EQEQUAL", "NOTEQUAL",
	"LESS",	"LESSEQUAL", "GREATER", "GREATEREQUAL", "COMMA", "RPAR", "ELSE",
	"DO", "LPAR", "EQUAL", "NUMBER", "IDENTIFIER", "IF", "WHILE", "INPUT",
	"PRINT", "DEFCHAR", "DEFINT", "DEFFLOAT", "DEFSTR", "DEFFUNC", "DOT",
	"ENDMARKER", "RETURN", "PERCENT", "AND", "OR", "PLUSEQUAL", "MINUSEQUAL",
	"STAREQUAL", "SLASHEQUAL", "PERCENTEQUAL", "NOT", "LSQB", "RSQB",
	"NEWLINE", "INDENT", "DEDENT", "PASS", "BREAK", "CONTINUE", "DEFLIST",
	"COLON", "IMPORT" };
	return string[t];
}


/*	This struct is the API to the Scanner object, containing both data and
 *	function adresses.
 *
 *	Function next() reads the next token, and places it in variable 'token'.
 *	If the token is an identifier or literal then 'string' points to the text.
 *	Function peek() looks one token ahead without actually reading it.
 *	Variable 'peeked' is used to temporarily store the peeked token, and is for
 *	internal use only.
 *
 */
typedef struct scanner {
	token_t	token;
	token_t peeked;  	/* private */
	bool_t atbol;
	char *string;
	token_t (*next)(void);
	token_t (*peek)(void);
	void (*init)(void);
	void (*save)(struct scanner *);
	void (*jump)(struct scanner *);

} Scanner;

extern Scanner scanner;

#endif
