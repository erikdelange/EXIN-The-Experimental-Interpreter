/* scanner.c
 *
 * Token scanner (also called symbol scanner)
 *
 * A program consist of a sequence of tokens. A token is a group of one or
 * more characters which have a special meaning in the programming language.
 * The scanner reads a program character by character (using the reader
 * object) and converts them into tokens.
 *
 * Object scanner is the API to the token scanner. Only one scanner object
 * exists. See also scanner.h.
 *
 * After reading the next token scanner.token contains the token type and
 * scanner.string - if applicable - the identifier, the number the
 * character of the string.
 *
 * 1994	K.W.E. de Lange
 */
#include <string.h>
#include <ctype.h>

#include "identifier.h"
#include "scanner.h"
#include "reader.h"
#include "error.h"

/*	Table containing all keywords and corresponding token.
 */
static struct {
	char *keyword;
	token_t token;
} keywordTable[] = {  /* Note: sort keyword strings alphabetically */
	{ "and",		AND },
	{ "break",		BREAK },
	{ "char",		DEFCHAR },
	{ "continue",	CONTINUE },
	{ "def",		DEFFUNC },
	{ "do",			DO },
	{ "else",		ELSE },
	{ "float",		DEFFLOAT },
	{ "if",			IF },
	{ "import",		IMPORT },
	{ "input",		INPUT },
	{ "int",		DEFINT },
	{ "list",		DEFLIST},
	{ "or",			OR },
	{ "pass",		PASS },
	{ "print",		PRINT },
	{ "return",		RETURN },
	{ "str",		DEFSTR },
	{ "while",		WHILE }
};


/*	Forward declarations.
 */
static token_t read_next_token(char *buffer);
static token_t read_identifier(char *buffer);
static token_t read_character(char *buffer);
static token_t read_string(char *buffer);
static token_t read_number(char *buffer);
static token_t next_token(void);
static token_t peek_token(void);
static void scanner_save(struct scanner *);
static void scanner_jump(struct scanner *);
static void scanner_init(struct scanner *);


/*	Token scanner API and data.
 */
Scanner scanner = {
	.token = UNKNOWN,
	.peeked = 0,
	.at_bol = true,
	.string[0] = 0,

	.next = next_token,
	.peek = peek_token,
	.init = scanner_init,
	.save = scanner_save,
	.jump = scanner_jump
};


static void scanner_init(struct scanner *sc)
{
	*sc = scanner;

	sc->token = UNKNOWN;
	sc->peeked = 0;
	sc->at_bol = true;
	sc->string[0] = 0;
}


/*	Save the global scanner state in sc.
 */
static void scanner_save(struct scanner *sc)
{
	*sc = scanner;
}


/*	Load the global scanner state from sc.
 */
static void scanner_jump(struct scanner *sc)
{
	scanner = *sc;
}


static token_t next_token(void)
{
	if (scanner.peeked == 0)
		scanner.token = read_next_token(scanner.string);
	else {
		scanner.token = scanner.peeked;
		scanner.peeked = 0;
	}

	debug_printf(DEBUGLEVEL1, "\ntoken: %-20s %-20s", \
							  tokenName(scanner.token), scanner.string);

	return scanner.token;
}


static token_t peek_token(void)
{
	if (scanner.peeked == 0)
		scanner.peeked = read_next_token(scanner.string);

	return scanner.peeked;
}


/* Read the next token.
 *
 * After reading the buffer contains:
 *    the identifier if token == IDENTIFIER
 *    the number if token == INTEGER or FLOAT
 *    the string if token == STRING
 *    the character if token == CHAR
 *    and is empty for all other tokens
 */
static token_t read_next_token(char *buffer)
{
	char ch;

	buffer[0] = 0;

	/*	Determine the level of indentation. If it has increased compared to the
	 * 	previous line then token is INDENT. Has it decreased then check if it
	 * 	was equal to the previous (smaller) indentation. If so then the token
	 * 	is DEDENT, else there is an indentation error.
	 * 	If the indentation has not changed then continue reading the next token.
	 */
	while (scanner.at_bol == true) {
		int col = 0;
		scanner.at_bol = false;

		/* determine the indentation */
		while (1) {
			ch = reader.nextch();
			if (ch == ' ')
				col++;
			else if (ch == '\t')
				col = (col / config.tabsize + 1) * config.tabsize;
			else
				break;
		}  /* col = column-nr of first character which is not tab or space */

		/* ignore empty lines or comment only lines */
		if (ch == '#')
			while (ch != '\n' && ch != EOF)
				ch = reader.nextch();
		if (ch == '\n') {
			scanner.at_bol = true;
			continue;
		} else if (ch == EOF) {
			col = 0;  /* do we need more DEDENTs? */
			if (col == local->indentation[local->level])
				return ENDMARKER;
		} else
			reader.pushch(ch);

		if (col == local->indentation[local->level])
			break;  /* indentation has not changed */
		else if (col > local->indentation[local->level]) {
			if (local->level == MAXINDENT)
				error(SyntaxError, "max indentation level reached");
			local->indentation[++local->level] = col;
			return INDENT;
		} else {  /* col < local->indentation[local->level] */
			if (--local->level < 0)
				error(SyntaxError, "inconsistent use of TAB and space in identation");
			if (col != local->indentation[local->level]) {
				scanner.at_bol = true;  /* not yet at old indentation level */
				reader.to_bol();
			}
			return DEDENT;
		}
	}

	/* skip spaces */
	do {
		ch = reader.nextch();
	} while (ch == ' ' || ch == '\t');

	/* skip comments */
	if (ch == '#')
		while (ch != '\n' && ch != EOF)
			ch = reader.nextch();

	/* check for end of line or end of file */
	if (ch == '\n') {
		scanner.at_bol = true;
		return NEWLINE;
	} else if (ch == EOF)
		return ENDMARKER;

	if (isdigit(ch)) {
		reader.pushch(ch);
		return read_number(buffer);
	} else if (isalpha(ch)) {
		reader.pushch(ch);
		return read_identifier(buffer);
	} else {
		switch (ch) {
			case '\'':	return read_character(buffer);
			case '\"': 	return read_string(buffer);
			case EOF :	return ENDMARKER;
			case '(' :	return LPAR;
			case ')' :	return RPAR;
			case '[' :	return LSQB;
			case ']' :	return RSQB;
			case ',' :	return COMMA;
			case '.' :	return DOT;
			case ':' :	return COLON;
			case '*' :	if (reader.peekch() == '=') {
							reader.nextch();
							return STAREQUAL;
						} else
							return STAR;
			case '%' :	if (reader.peekch() == '=') {
							reader.nextch();
							return PERCENTEQUAL;
						} else
							return PERCENT;
			case '+' :	if (reader.peekch() == '=') {
							reader.nextch();
							return PLUSEQUAL;
						} else
							return PLUS;
			case '-' :	if (reader.peekch() == '=') {
							reader.nextch();
							return MINUSEQUAL;
						} else
							return MINUS;
			case '/' :	if (reader.peekch() == '=') {
							reader.nextch();
							return SLASHEQUAL;
						} else
							return SLASH;
			case '!' :	if (reader.peekch() == '=') {
							reader.nextch();
							return NOTEQUAL;
						} else
							return NOT;
			case '=' :	if (reader.peekch() == '=') {
							reader.nextch();
							return EQEQUAL;
						} else
							return EQUAL;
			case '<' :	if (reader.peekch() == '=') {
							reader.nextch();
							return LESSEQUAL;
						} else if (reader.peekch() == '>') {
							reader.nextch();
							return NOTEQUAL;
						} else
							return LESS;
			case '>' :	if (reader.peekch() == '=') {
							reader.nextch();
							return GREATEREQUAL;
						} else
							return GREATER;
			default  :	return UNKNOWN;
		}
	}
}


/* Read a string.
 *
 * Strings are surrounded by double quotes. Escape sequences are recognized.
 * Examples: "abc"  ""  "xyz\n"
 */
static token_t read_string(char *string)
{
	char ch;
	int	count = 0;

	while (1) {
		ch = reader.nextch();
		if (ch != EOF && ch != '\"') {
			if (ch == '\\')
				switch (reader.peekch()) {
					case 'b' :	reader.nextch(); ch = '\b'; break;
					case 'f' :	reader.nextch(); ch = '\f'; break;
					case 'n' :	reader.nextch(); ch = '\n'; break;
					case 'r' :	reader.nextch(); ch = '\r'; break;
					case 't' :	reader.nextch(); ch = '\t'; break;
					case 'v' :	reader.nextch(); ch = '\v'; break;
					case '\\':	reader.nextch(); ch = '\\'; break;
					case '\'':	reader.nextch(); ch = '\''; break;
					case '\"':	reader.nextch(); ch = '\"'; break;
					case '0' :	reader.nextch(); ch = '\0'; break;
				}
			if (count < BUFSIZE)
				string[count++]= ch;
		} else {
			string[count] = 0;
			break;
		}
	}
	return STR_T;
}


/* Read an integer or a floating point number.
 *
 * Scientific notation (e, E) is recognized.
 * Examples: 2  0.2  2.0  1E+2  1E2  1E-2  0.1E+2
 */
static token_t read_number(char *number)
{
	char ch;
	int dot = 0;
	int exp = 0;
	int	count = 0;

	while (1) {
		ch = reader.nextch();
		if (ch != EOF && (isdigit(ch) || ch == '.')) {
			if (ch == '.') {
				if (++dot > 1)
					error(ValueError, "multiple decimal points");
			}
			if (count < BUFSIZE)
				number[count++] = ch;
		} else {  /* check for scientific notation */
			if (ch == 'e' || ch == 'E') {
				exp = 1;
				if (count < BUFSIZE)
					number[count++] = ch;
				ch = reader.nextch();

				if (ch == '-' || ch == '+') {
					if (count < BUFSIZE)
						number[count++] = ch;
					ch = reader.nextch();
				}
				if (!isdigit(ch))
					error(ValueError, "missing exponent");
				while (ch != EOF && isdigit(ch)) {
					if (count < BUFSIZE)
						number[count++] = ch;
					ch = reader.nextch();
				}
			}
			number[count] = 0;
			reader.pushch(ch);
			break;
		}
	}

	if (dot == 1 || exp == 1)
		return FLOAT_T;

	return INT_T;
}


/* Read a name and check whether it is a keyword or an identifier.
 *
 * A name consist of digits, letters and underscores, and must start with
 * a letter.
 */
static token_t read_identifier(char *name)
{
	char ch;
	int	count = 0, l, h, m, d;

	while (1) {
		ch = reader.nextch();
		if (ch != EOF && (isalnum(ch) || ch == '_')) {
			if (count < BUFSIZE)
				name[count++] = ch;
		} else {
			name[count] = 0;
			reader.pushch(ch);
			break;
		}
	}

	l = 0, h = (int)(sizeof keywordTable / sizeof keywordTable[0]) - 1;

	while (l <= h) {
		m = (l + h) / 2;
		d = strcmp(&name[0], keywordTable[m].keyword);
		if (d < 0)
			h = m - 1;
		if (d > 0)
			l = m + 1;
		if (d == 0)
			break;
	};

	if (d == 0) {
		name[0] = 0;
		return keywordTable[m].token;
	} else
		return IDENTIFIER;
}


/* Read a character constant. This can a single letter or an escape sequence.
 *
 * A character constant is surrounded by single quotes.
 */
static token_t read_character(char *c)
{
	char ch;

	ch = reader.nextch();

	if (ch == '\\') {  /* is an escape sequence */
		ch = reader.nextch();
		switch (ch) {
			case 'b' :	c[0] = '\b'; break;
			case 'f' :	c[0] = '\f'; break;
			case 'n' :	c[0] = '\n'; break;
			case 'r' :	c[0] = '\r'; break;
			case 't' :	c[0] = '\t'; break;
			case 'v' :	c[0] = '\v'; break;
			case '\\':	c[0] = '\\'; break;
			case '\'':	c[0] = '\''; break;
			case '\"':	c[0] = '\"'; break;
			case '0' :	c[0] = '\0'; break;
			default  :	error(SyntaxError, "unknown escape sequence: %c", ch);
		}
	} else {  /* not an escape sequence */
		if (ch == '\'' || ch == EOF)
			error(SyntaxError, "empty character constant");
		else
			c[0] = ch;
	}
	ch = reader.nextch();
	if (ch != '\'')
		error(SyntaxError, "to many characters in character constant");

	c[1] = 0;

	return CHAR_T;
}
