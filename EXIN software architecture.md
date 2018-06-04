# EXIN Software Architecture

The interpreter is intended to be simple to make it easy to understand its workings. Starting point for the interpreter is the language grammar as described in [EXIN EBNF Syntax Diagram](EXIN%20syntax%20diagram.pdf). The interpreter works according to the rules depicted in this diagram.
##### Interpreter code
The interpreter itself is written in C. To build the interpreter I used the GCC C compiler from MinGW-w64 and the free CodeLite IDE for Windows. When setting up a project in CodeLite add DEBUG to the preprocessor constants if you want the interpreter to be able to show debug information.
Every source code file has an accompanying header file (e.g. *object.c*, *object.h*). All source and header files reside in the same directory. At the beginning of most files an extensive explanation of its function is available.
##### Using the interpreter
When starting the interpreter with the -h argument - and if it has been compiled with the DEBUG option - the following message is printed.
```
> .\exin -h
EXIN version 1.04
usage: EXIN [options] file
options
-d[detail] = show debug info
    detail = 1..9 (little - much)
-h show usage information
-t[tabsize] = set tab size in spaces
    tabsize = >= 0
 ```
By specifying a filename this file is loaded and the code is executed.
##### Code structure
The core function of the interpreter is *parser()* which can be found in *parser.c*. The parser decodes the last read token from the program code and starts executing it. A token is a group of characters which have a special meaning in the language. For example the *while* statement or floating point constant *5.1E3*. The scanner translates groups of characters in the program code into tokens so the parser can digest them. The scanner is accessed via struct *scanner* in file *scanner.c* which contains both the scanners' variables and all functions which may be called by other functions. Using a struct like this is a very crude way to mimic what in other languages would be an object.
``` C
typedef struct scanner {
	token_t	token;
	token_t peeked;  	/* private */
	bool at_bol;
	char string[BUFSIZE + 1];
	token_t (*next)(void);
	token_t (*peek)(void);
	void (*init)(struct scanner *);
	void (*save)(struct scanner *);
	void (*jump)(struct scanner *);

} Scanner;

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

token = scanner.next();
printf("%s", token.string);
```
Program code is stored modules and accessed via the *module* struct. Modules must be loaded via the *import* statement. See *reader.c* and *module.c* and structs *reader* and *module* for the details.
The scanner uses a reader (in file *reader.c*) to read individual characters from the program code. Similar to the *scanner* struct only a single *reader* struct containing variables and function calls is used. The reader is able move the read pointer to other places in the same module or between modules when it needs to do so because of function calls or loops.

![EXIN-software-structure.png](https://github.com/erikdelange/EXIN-The-Experimental-Interpreter/blob/master/EXIN-software-structure.png)

###### Not efficient
When reading code the interpreter evaluates the characters which are read over and over. So long variables names are searched in the list with identifiers every time again. This is not very efficient. Some interpreters first translate these names and/or keywords in shorter (e.g. one- or two-byte) versions before starting interpretation which speeds up things. However the aim for this interpreter was simplicity and not speed.
##### Variables
Function names and variables are stored in lists with identifiers. Two global variables *global* and *local* in *identifier.c* point to the relevant lists with identifiers.
An identifier is just a name. The value which belongs to a variable is stored separately in an object. This allows an identifier to point to any type of value. Using a uniform way to store values also makes operations on variables easy. Because all values are objects they are also used during expression evaluation (see *expression.c*). The generic functions to do unary and binary operations on objects can be found in *object.c*. The *obj_...* functions are actually wrappers. For each type of variable a separate file with the supported operations is created. See *number.c*, *string.c* and *list.c* for the detals and note that not all objects support the same operations. Again the obj_... wrapper calls functions in these files.
Two special objects are *position* and *none*. The first one is used to store the location of function calls and loops in the source code. None is used as a return value when a function cannot return a value.
