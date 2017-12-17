# EXIN Software Architecture

The expermimental interpreter (EXIN) is intended to be simple to make it easy to understand its workings. Starting point for the interpreter is the EXIN language grammar which is described in [EXIN EBNF Syntax Diagram](./EXIN EBNF syntax diagram.xhmtl). The interpreter works according to the rules depicted in this diagram.
##### Interpreter code
The interpreter itself is written in C. To build the interpreter I used the GCC C compiler which is automatically installed with the CodeLite IDE for Windows. When setting up a project in CodeLite add DEBUG to the preprocessor constant if you want the interpreter to be able to print debug information. Every source code file has an accompanying header file (e.g. *object.c*, *object.h*). All source and header files reside in the same directory. At the beginning of most files an extensive explanation of its function is available.
##### Using the interpreter
When starting the interpreter without any argument - and if it has been compiled with the DEBUG option - the following message is printed.
```
> .\exin
EXIN version 1.0
usage: EXIN.exe [options] [file]
options
-d[detail] = show debug info
   detail = 1..9 (little - much)
-i = interpret line as statement
-e = evaluate line as expression
 ```
This indicates the three ways it can be used.
1. By specifying a file name this file is loaded and the code is executed.
2. By specifying the -i flag a line with a statement can be entered which will be interpreted after newline. Note that this only allows you to use single line statements and no loops or if-else. This is mainly useful when debugging.
3. By specifying the -e flag a line with an expression can be entered and the result will be printed after newline. Again this is useful when debugging.

##### Code structure
The core function of EXIN is *parser()* which can be found in *parser.c*. The parser decodes the last read token from the program code and starts executing it. A token is a group of characters which have a special meaning in the language. For example the *while* statement or floating point constant *5.1E3*. The scanner translates groups of characters in the program code into tokens so the parser can digest them. The scanner is accessed via struct *scanner* in file *scanner.c* which contains both the scanners variables and all functions which may be called by other functions. Using a struct like this is a very crude way to mimic what in other languages would be an object.
``` C
typedef struct scanner {
	token_t	token;
	token_t peeked;
	bool_t atbol;
	char *string;
	void (*init)(void);
	token_t (*next)(void);
	token_t (*peek)(void);
} Scanner;

Scanner scanner = {
	UNKNOWN,
	0,
	true,
	&buffer[0],
	token_init,
	token_next,
	token_peek
};

token = scanner.next();
printf("%s", token.string);
```
EXIN program code is stored modules and accessed via the *module* struct. At least one module exists but via the *import* statement more can be loaded. See *module.c* and struct *module* for the details.
The scanner uses a reader (in file *reader.c*) to read individual characters from the program code. Similar to the *scanner* struct a single *reader* struct containing variables and function calls is used. The reader is able move the read pointer to other places in the same module or between modules when it needs to do so because of function calls or loops.

![Exin-software-structure.png](https://github.com/erikdelange/EXIN-The-Experimental-Interpreter/Exin-software-structure.png)

###### Not efficient
When reading code the interpreter evaluates the characters which are read over and over. So long variables names are searched in the list with identifiers every time again. This can be time-consuming. Some interpreters first translate these names and/or keywords in shorter (e.g. one-byte versions) before starting interpretation which speeds up things. However the aim for EXIN was simplicity and not speed.
##### Variables
Function names and variables are stored in lists with identifiers. The two global variables *global* and *local* in *identifier.c* point to the relevant lists with identifiers.
An identifier is just a name, the value which belongs to a variable is stored separately in an object. This allows an identifier to point to any type of value. Using a uniform way to store values also makes operations on variables easy. Because all values are objects they are also used during expression evaluation (see *expression.c*). The generic functions to do unary and binary operations on objects can be found in *object.c*. The *obj_...* functions are actually wrappers. For each type of variable a separate file with the supported operations is created. See *number.c*, *string.c* and *list.c* for the detals and note that not all objects support the same operations. The obj_... wrapper calls functions in these files.
Two special objects are *position* and *none*. The first one is used to store the location of function calls and loops in the source code. None is used as a return value when a function cannot return a value.
