# EXIN Software Architecture

The interpreters design is as simple as possible so its workings can be easily understood. The interpreter implements the language grammar as described in [EXIN EBNF Syntax Diagram](EXIN%20syntax%20diagram.pdf).

##### Running the interpreter
When starting the interpreter with the -h argument - and if it has been compiled with the DEBUG option - the following message is printed.
```
> .\exin -h
EXIN version 1.10
usage: exin [options] file
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
-h = show usage information
-t[tabsize] = set tab size in spaces
    tabsize = >= 1
-v = show version information
```
By specifying a file this is loaded and the code is executed. The filename must include its extension (if any), the interpreter does not guess.

##### C version
The interpreter is written in C99. For development I used MinGW's GCC C compiler (then version 6.3.0) and CodeLite as IDE.

##### Debugging
There are two ways to debug the interpreter;
1. the most low level approach is debugging the actual C code
2. have the interpreter print debugging messages when running

###### Debugging messages
The interpreter can produce extensive debugging output. For this add DEBUG to the preprocessor constants. Search for `debug_printf()` in the code to see where the messages are generated. As an example, when running the following program with -d7 as debug level ...
``` python
int n = 3
print n
```
... this debug output is printed.
```
------: Start execution
token : DEFINT
token : IDENTIFIER n
alloc : 0000000000BA6F90 int
bind  : n, 0000000000BA6F90
token : EQUAL
token : INTEGER LITERAL 3
alloc : 0000000000BA1620 int
token : NEWLINE
free  : 0000000000BA1620 int
token : PRINT
token : IDENTIFIER n
token : NEWLINE
print : 3
token : ENDMARKER
token : ENDMARKER
```
Printing debugging messages can obscure a programs output. Therefor this is printed using a green background (provided preprocessor constant VT100 is defined). Unfortunately this is not visible here as MarkDown does not allow manually setting colors in code blocks. The background color is set using ANSI/VT100 codes. On Windows these are not automatically recognized by PowerShell and cmd.exe, so the debugging output will just show the actual escape sequences. To enable ANSI/VT100 codes execute the following command.
``` shell
reg add HKCU\Console /v VirtualTerminalLevel /t REG_DWORD /d 1
```
Restart the shell afterwards. In Windows' bash shell the colors work out of the box. If the shell you are using does not support VT100 code remove *VT100* from the preprocessor constants and recompile.

##### Code structure
Every source code file has an accompanying header file (e.g. *object.c*, *object.h*) with a guard. All source and header files reside in the same directory. Most files have an short explanation in their header.

The core function of the interpreter is *parser()* in file *parser.c*. The parser decodes the most recently read token from the program code and starts executing it. A token is a group of characters which have a special meaning in the language. For example the *while* statement or floating point constant *5.1E3*. The scanner translates groups of characters in the program code into tokens which the parser can digest. The scanner is accessed via struct *scanner* in file *scanner.c* which contains both the scanners' variables and all functions which may be called by other functions. Using a struct like this is a very crude way to mimic what in other languages would be an object.
``` C
/* definition of the object */
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

/* creating an instance of the object */
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

/* accessing methods and variables */
token = scanner.next();
printf("%s", token.string);
```
EXIN program code is stored in modules which are loaded via the *import* statement. See *module.c* and struct *module* for details.
The scanner uses the reader (struct *reader* in file *reader.c*) to read individual characters from the program code. Similar to the *scanner* struct only a single *reader* struct containing variables and function calls is used. The reader is able move the read pointer to other places in the same module when executing loops, or even between modules in case of function calls.

![EXIN-software-structure.png](https://github.com/erikdelange/EXIN-The-Experimental-Interpreter/blob/master/EXIN-software-structure.png)

###### Efficiency
When reading code the interpreter evaluates the characters which are read over and over. So long variables names are searched in the identifier lists every time again. This is not very efficient. Some interpreters first translate names and/or keywords in shorter (e.g. one- or two-byte) versions before starting interpretation to speeds up things. However the aim for this interpreter was simplicity and not speed.
##### Variables
Function names and variables are stored in lists with identifiers. Globals *global* and *local* in *identifier.c* point to the relevant lists with identifiers. An exception are builtin functions as defined in *function.c*. However you can specify identifiers with the same names as builtins: then your functions which will shadow the builtins.
An identifier is just a name (ie. a string). The value which belongs to a variable is stored separately in an object. This allows an identifier to point to any type of value. This is used in the *for .. in* statement. Using a uniform way to store values makes operations on variables easy. Because all values are objects they can also be used during expression evaluation (see *expression.c*). The generic functions to do unary and binary operations on objects can be found in *object.c*. Actually the *obj_...* functions are wrappers. For each type of variable a separate C file with the supported operations exists. See *number.c*, *string.c* and *list.c* for the details and note that not every object supports all operations. Again note the obj_... wrapper calls functions in these files.
Two special objects are *position* and *none*. The first one is used to store the location of function calls and loops in the source code. None is used as a return value when a function cannot return a value.
