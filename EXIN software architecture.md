# EXIN Software Architecture

The interpreters design is simple to make it easy to understand its workings. The interpreter implements the language grammar as described in [EXIN EBNF Syntax Diagram](EXIN%20syntax%20diagram.pdf).
##### Interpreter code
The interpreter is written in C. To build the interpreter I used the GCC C compiler (version 8.1.0) from MinGW-w64. As IDE I used CodeLite. When setting up a project in CodeLite add DEBUG to the preprocessor constants if you want the interpreter to be able to show various levels of debug information. If you want to debug the C code using CodeLite also install the advised TDM compiler suite and set the debugger path to gdb64 in the TDM directory. Do *not* use TDM to compile as it the current version (5.1.0) does not implement C99 completely; compilation will fail.
Every source code file has an accompanying header file (e.g. *object.c*, *object.h*). All source and header files reside in the same directory. At the beginning of most files an short explanation of its function is placed in the comments.
##### Using the interpreter
When starting the interpreter with the -h argument - and if it has been compiled with the DEBUG option - the following message is printed.
```
> .\exin -h
EXIN version 1.08
usage: EXIN [options] file
file: code to execute
options
-d[detail] = show debug info
    detail = sum of options (default = 1)
    option 0: no debug output
    option 1: show tokens during execution
    option 2: show block entry and exit
    option 4: show memory allocation
    option 8: show tokens during function scan
    option 16: dump identifier and object table to disk
-h show usage information
-t[tabsize] = set tab size in spaces
    tabsize = >= 0
 ```
By specifying a filename this file is loaded and the code is executed. The filename must include its extension (if any), the interpreter does not guess.
##### Code structure
The core function of the interpreter is *parser()* which can be found in *parser.c*. The parser decodes the most recently read token from the program code and starts executing it. A token is a group of characters which have a special meaning in the language. For example the *while* statement or floating point constant *5.1E3*. The scanner translates groups of characters in the program code into tokens so the parser can digest them. The scanner is accessed via struct *scanner* in file *scanner.c* which contains both the scanners' variables and all functions which may be called by other functions. Using a struct like this is a very crude way to mimic what in other languages would be an object.
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
EXIN program code is stored in modules which are loaded via the *import* statement. See *module.c* and struct *module* for the details.
The scanner uses the reader (struct *reader* in file *reader.c*) to read individual characters from the program code. Similar to the *scanner* struct only a single *reader* struct containing variables and function calls is used. The reader is able move the read pointer to other places in the same module when executing loops, or even between modules in case of function calls.

![EXIN-software-structure.png](https://github.com/erikdelange/EXIN-The-Experimental-Interpreter/blob/master/EXIN-software-structure.png)

###### Efficiency
When reading code the interpreter evaluates the characters which are read over and over. So long variables names are searched in the list with identifiers every time again. This is not very efficient. Some interpreters first translate these names and/or keywords in shorter (e.g. one- or two-byte) versions before starting interpretation which speeds up things. However the aim for this interpreter was simplicity and not speed.
##### Variables
Function names and variables are stored in lists with identifiers. Two global variables *global* and *local* in *identifier.c* point to the relevant lists with identifiers.
An identifier is just a name. The value which belongs to a variable is stored separately in an object. This allows an identifier to point to any type of value. This is used in the *for .. in* statement. Using a uniform way to store values makes operations on variables easy. Because all values are objects they are also used during expression evaluation (see *expression.c*). The generic functions to do unary and binary operations on objects can be found in *object.c*. The *obj_...* functions are actually wrappers. For each type of variable a separate file with the supported operations is created. See *number.c*, *string.c* and *list.c* for the detals and note that not all objects support the same operations. Again the obj_... wrapper calls functions in these files.
Two special objects are *position* and *none*. The first one is used to store the location of function calls and loops in the source code. None is used as a return value when a function cannot return a value.
