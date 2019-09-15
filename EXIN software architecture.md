# EXIN Software Architecture

The interpreters design is kept simple so its workings can be easily understood. The interpreter implements the language grammar as described in [EXIN EBNF Syntax Diagram](EXIN%20syntax%20diagram.pdf).

##### Running the interpreter
When starting the interpreter with the -h argument - and if it has been compiled with the DEBUG macro (-D DEBUG) - the following message is printed.
```
> .\exin -h
EXIN version 1.10
usage: exin [options] module
module: name of file containing code to execute
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
By specifying a module it is loaded and executed. The module name must include its extension (if any), the interpreter does not guess.
##### Notes on coding
###### Include files
If a source file requires a header (*.h*) file, this has the same name (*module.c, module.h*). Every header file has a guard to prevent double inclusion.  Every source or header file only includes the headers it needs, there is no general 'include all' header.
###### Assertions
Certain bugs such as NULL pointers can terminate the interpreter immediately and leave you clueless where things went wrong. To catch these bugs in many functions the validity of input and ouput values is checked using the assert() macro. In the release version assertions are removed by defining preprocessor macro NDEBUG.
###### Directory structure
All source and header files reside in the same directory.
###### Function prototypes
Unless required for code readability function definitions are sequenced in such a way in a source file that the use of function prototypes is not needed.
###### Static functions
In principle functions and global variables are declared as static. This means they can only be accessed from within the source file where they are defined. This ensures that making a function or variable available to other source files is a concious decision. In this way I try to avoid spaghetti code.
###### Struct holding functions
Another way I used to structure the code is using structs containing function pointers and some data. From within a certain source file only these pointers are exposed to other source files, i.e. can be called. Using a struct like this is a very crude way to mimic what in other languages would be an object. For example see the scanner which reads tokens (meaningfull sequences of characters) from EXIN modules. It is accessed via globally defined struct *scanner* in file *scanner.c* which contains both the scanners' variables and all exposed functions.

``` C
/* definition of the object */
typedef struct scanner {
	token_t token;
	token_t peeked;  	/* private */
	bool at_bol;
	char string[BUFSIZE + 1];

	token_t (*next)(void);
	token_t (*peek)(void);
	void (*init)(struct scanner *);
	void (*save)(struct scanner *);
	void (*jump)(struct scanner *);

} Scanner;

/* create an instance of the object */
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

/* access methods and variables */
token = scanner.next();
printf("%s", token.string);
```
##### Versions
The interpreter is written in C99. For development I used MinGW's GCC C compiler (then version 6.3.0) and the CodeLite IDE.
##### Debugging
There are two ways to debug the interpreter;
1. the most low level approach is debugging the actual C code
2. have the interpreter print debugging messages when running

###### Debugging messages
The interpreter can produce extensive debugging output. For this add DEBUG to the preprocessor macros. Search for `debug_printf()` in the code to see where the messages are generated. For example, when running the following program with -d7 as debug level ...
``` python
int n = 3
print n
```
... this debug output is printed.
```
------: Start execution
token : DEFINT
token : IDENTIFIER n
alloc : 00760f48 int
bind  : n, 00760f48
token : EQUAL
token : INTEGER LITERAL 3
alloc : 00760f68 int
token : NEWLINE
free  : 00760f68 int
token : PRINT
token : IDENTIFIER n
token : NEWLINE
print : 3
token : ENDMARKER
token : ENDMARKER
```
Printing debugging messages can obscure a programs actual output. Therefor this is printed using a green background (provided preprocessor macro VT100 is defined). Unfortunately this is not visible here as MarkDown does not allow manually setting colors in code blocks. The background color is set using ANSI/VT100 codes. On Windows these are not automatically recognized by PowerShell and cmd.exe, so the debugging output will just show the actual escape sequences. To enable ANSI/VT100 codes in these shells execute the following command.
``` shell
reg add HKCU\Console /v VirtualTerminalLevel /t REG_DWORD /d 1
```
Restart the shell afterwards. In Windows' bash shell the colors work out of the box. If the shell you are using does not support VT100 code remove *VT100* from the preprocessor macros and recompile.
##### Code structure
The core function of the interpreter is *parser()* in file *parser.c*. The parser decodes the most recently read token from the program code and starts executing it. A token is a group of characters which have a special meaning in the language. For example the *while* statement or floating point constant *5.1E3*. The scanner translates groups of characters in the program code into tokens which the parser can digest.
EXIN program code is stored in modules which are loaded via the *import* statement. See *module.c* and struct *module* for details.
The scanner uses the reader (struct *reader* in file *reader.c*) to read individual characters from the program code. Similar to the *scanner* struct only a single *reader* struct containing variables and function calls is used. The reader is able move the read pointer to other places in the same module when executing loops, or even between modules in case of function calls.

![EXIN-software-structure.png](https://github.com/erikdelange/EXIN-The-Experimental-Interpreter/blob/master/EXIN-software-structure.png)

###### Efficiency
When reading code the interpreter evaluates the characters which are read over and over. So long variables names are searched in the identifier lists every time again. This is not very efficient. Some interpreters first translate names and/or keywords in shorter (e.g. one- or two-byte) versions before starting interpretation to speeds up things. However the aim for this interpreter was simplicity and not speed.
##### Variables
Function names and variables are stored in lists with identifiers. Globals *global* and *local* in *identifier.c* point to the relevant lists with identifiers. An exception are builtin functions as defined in *function.c*. However you can specify identifiers with the same names as builtins: then your functions which will shadow the builtins.
An identifier is just a name (ie. a string). The value which belongs to a variable is stored separately in an object. This allows an identifier to point to any type of value. This is used in the *for .. in* statement. Using a uniform way to store values makes operations on variables easy. Because all values are objects they can also be used during expression evaluation (see *expression.c*). The generic functions to do unary and binary operations on objects can be found in *object.c*. Actually the *obj_...* functions are wrappers. For each type of variable a separate C file with the supported operations exists. See *number.c*, *string.c* and *list.c* for the details and note that not every object supports all operations. Again note the obj_... wrapper calls functions in these files.
Two special objects are *position* and *none*. The first one is used to store the location of function calls and loops in the source code. *None* is used as a return value when a function cannot return a value.
