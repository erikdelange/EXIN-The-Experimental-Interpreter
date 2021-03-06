/* scanner.h
 *
 * 2018	K.W.E. de Lange
 */
#ifndef _SCANNER_
#define _SCANNER_

#include <stdbool.h>
#include "config.h"

typedef enum { 	UNKNOWN=0, CHAR, INT, FLOAT, STR, STAR, SLASH,
				PLUS, MINUS, EQEQUAL, NOTEQUAL, LESS, LESSEQUAL, GREATER,
				GREATEREQUAL, COMMA, RPAR, ELSE, DO, LPAR, EQUAL, NUMBER,
				IDENTIFIER, IF, WHILE, INPUT, PRINT, DEFCHAR, DEFINT,
				DEFFLOAT, DEFSTR, DEFFUNC, DOT, ENDMARKER, RETURN, PERCENT,
				AND, OR, PLUSEQUAL, MINUSEQUAL, STAREQUAL, SLASHEQUAL,
				PERCENTEQUAL, NOT, LSQB, RSQB, NEWLINE, INDENT, DEDENT,
				PASS, BREAK, CONTINUE, DEFLIST, COLON, IMPORT, FOR, IN } token_t;

static inline char *tokenName(token_t t)  /* 'inline' requires at least C99 */
{
	static char *string[] = {
	"UNKNOWN TOKEN", "CHARACTER LITERAL", "INTEGER LITERAL", "FLOAT LITERAL",
	"STRING LITERAL", "STAR", "DIV", "PLUS", "MINUS", "EQEQUAL", "NOTEQUAL",
	"LESS", "LESSEQUAL", "GREATER", "GREATEREQUAL", "COMMA", "RPAR", "ELSE",
	"DO", "LPAR", "EQUAL", "NUMBER", "IDENTIFIER", "IF", "WHILE", "INPUT",
	"PRINT", "DEFCHAR", "DEFINT", "DEFFLOAT", "DEFSTR", "DEFFUNC", "DOT",
	"ENDMARKER", "RETURN", "PERCENT", "AND", "OR", "PLUSEQUAL", "MINUSEQUAL",
	"STAREQUAL", "SLASHEQUAL", "PERCENTEQUAL", "NOT", "LSQB", "RSQB",
	"NEWLINE", "INDENT", "DEDENT", "PASS", "BREAK", "CONTINUE", "DEFLIST",
	"COLON", "IMPORT", "FOR", "IN" };
	return string[t];
}


/* This struct is the API to the scanner object, containing both data and
 * function adresses.
 *
 * Function next() reads the next token, and places it in variable 'token'.
 * If the token is an identifier or literal then 'string' points to the text.
 * Function peek() looks one token ahead without actually reading it.
 * Variable 'peeked' is used to temporarily store the peeked token, and is for
 * internal use only.
 */
typedef struct scanner {
	token_t token;
	token_t peeked;		/* private */
	bool at_bol;
	char string[BUFSIZE + 1];

	token_t (*next)(void);
	token_t (*peek)(void);
	void (*init)(struct scanner *);
	void (*save)(struct scanner *);
	void (*jump)(struct scanner *);
} Scanner;

extern Scanner scanner;

#endif
