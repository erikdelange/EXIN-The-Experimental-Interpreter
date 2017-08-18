# EXIN Language Reference

##### General
EXIN is a simple language and contains elements of Basic, C and Python.
##### Keywords
The follwing keywords are reserved and may not be used as variable names.
```
and      break    char     continue  def
do       else     float    if        import
input    int      list     or        pass
print    return   str      while
```
##### Code format
Code consist of lines of plain text. Lines contains statements but can also be empty. Statements do not span lines but are terminated by the newline character. Indentation is used to group statements in blocks for control structures (if-else, do-while, while-do). For example
``` python
while a != 10
    print a
    a += 1
```
 will not yield the same result as
``` python
while a != 10
    print a
a += 1
```
Only tabs must be used for indentation (although spaces will also work).

A hash sign (#) identifies the start of a comment. All text after a hash sign until the end of the line is discarded.
``` python
# This is a line containing only a comment

print "Hello"  # a comment at the end of a statement
```
Code execution starts at the top of the file.
##### Data types
The three primitive data types are *char*, *int* and *float*. They are used for storing characters, integers and floating point numbers and match the C data types char, long and double.

Two other data types are built on top of these primitive types: strings and lists. Lists store any data type, including other lists. Their data type is *list*. The elements in a list can be a mix of all data types. A special variant of the list is the string (data type *str*) which contains only characters.

EXIN is strongly typed and requires that every variable is declared before it can be used.
```
char c
int count
float sales_amount
str s1
list l_2
```
Variable names must begin with a letter and consist of letters, digits and underscores.

Variables receive an implicit default value when declared; 0 for the primitive types or else an empty list or string. It is also possible to assign a value during declaration. This value can be a constant or an expression. Multiple variables of the same type can be declared on a single line.
```
char a = 'A', b = '\n', c
int i = 10
float x = 3.14, y = 1E10
str s = "abcd", t = "\n", u = ""
list l = ['a', 2.1, "xyz"], m = []
```
##### Literals
A character constant is surrounded by single quotes, a string constant by double quotes. Both may contain escape sequences starting with a backslash. Number constant without decimal dot are considered integers, and with a dot floats. A floating point number can also be written in scientific notation.
``` python
>>> 1 / 2
= 0
>>> 1 / 2.0
= 0.5
>>> 1.0 / 2
= 0.5
>>> 1 / 2E0
= 0.5
>>> 1E0 / 2
= 0.5
```
##### Strings and lists
###### Indices and slices
Elements in lists and strings are be accessed via their index. Index numbers start at 0, or -1 when counting from the end of the list or variable.
``` python
>>> "abc"[0]
= a

>>> "abc"[-1]
= c
```
It is also possible to take slices of a list or string using indices. The absence of an index identifies either the start or the end of a list or string. Out of bound indices are adjusted to the nearest possible value.
``` python
>>> "abcdef"[:]
= abcdef

>>> "abcdef"[1:]
= bcdef

>>> "abcdef"[1:3]
= bc

>>> "abcdef"[:-5]
= a
```
The number of elements in a list of string is returned by the *len* method.
```
>>> "abcdef".len
= 6

>>> "abcdef"[1:3].len
= 2
```
###### Adding and removing values
Characters, numbers and strings can be added to a string via the *+* operator.
``` python
>>> "ab" + 'c'
= abc

>>> "xy" + 3.14
= xy3.14
```
Literal values can be appended to a list via the *+* operator if the literal is represented as a list constant.
``` python
>>> [3] + ["alfa"]
=[3,"alfa"]
```
The best way to append an element at the end of a list is via the *append* method.
``` python
>>> list m
>>> m.append(3.14)
>>> print m
[3.14]
```
Inserting an element somewhere in a list is done via *insert(before_index, element)*. Removing an element can only be done via its index. As always this can be an index from the beginning or the end of the list.
``` python
>>> list m
>>> m.insert(0, 3.14)  # insert at beginning of list
>>> print m
[3.14]
>>> m.remove(-1)  # remove the last item
>>> print m
[]
>>>
```
##### Operators
###### Arithmetic
The binary operators are +, -, \*, / and the modulo operator %. Modulo can only be used on integers. For usage in assignments the shorthands operators +=, -=, \*=, /= and \%= are available instead of (for example) n = n + 1. Using addition on lists or strings will result in concatenation. Multiplication by a number will result in repetition of the list or string.
``` python
>>> "abc" + "def"
= abcdef

>>> [1,2] + [3,4]
= [1,2,3,4]

>>> "xyz" * 2
= xyzxyz

>>> [1,2] * 2
= [1,2,1,2]
```  
###### Comparison
The comparison operators are ==, !=, <>, <, <=, >, >=. Note that equality comparison uses two equal characters where assignment only used one. Lists cannot be compared.
###### Logical
The logical operators are *and*, *or* and *!* (being not). True is represented by a non-zero integer, falso being zero.
###### Order of evaluation
Expression evaluation follows the following rules of precedence:
 *  first read variables (including subscripts and slices) and constants,
 * 	then execute function calls, list and string methods and evaluate parenthesized expressions,
 *	then the unary operators + - and !
 *	then multiplication and division (normal and modulo)
 *	then addition and subtraction
 *	then the comparisons < <= > and >=
 *	then the comparisons  == and !=
 * 	then logical *and*
 *	then logical *or*
 *	then assignment of values (normal and shorthand)
 * 	and last comma separated statements

##### Control structures
###### If .. else
The *if* keyword is followed by a conditional expression and if this evalutes to true the statement block following *if* is executed. Optionally an *else* block can be defined. If statements can be nested.
``` python
if i > 0
    print "i is greater then zero"
else
    if i == 0
        print "i equals zero"
    else
        print "i is less then zero"
```
###### Loops
EXIN has two types of loops: *while*, and *do .. while*. The while loop evaluates the conditional expression before the loop is entered, whereas for the do .. while loop this is only done after the loop has been executed. So the statement block of the last type of loop is executed at least once.
``` python
int n = 0
while n
    print "this is never executed"

int m
do
    print m
while (m += 1) < 10
```
According to the rules of precedence the parenthesized part of the conditional expression from the do-while loop is executed before the comparison is made, so loop counter m is incremented first.

Exit from a loop can be forced at any point by the *break* and *continue* statement. Using *break* the innermost loop is exited and the conditional expression is considered to be false. Via *continue* the rest of the statement block of the loop is skipped, causing the conditonal expression to be evaluated again.
``` python
int n
while 1
    if n == 10
        break
    n += 1
```
This loop is executed infinitly because 1 always evaluates to true. Eventually the *if* statement with *break* makes sure the loop is terminiated once n equals 10.
##### Function definition
Functions are defined via the *def* keyword followed by a function name and a pair of parenthesis containing the argument names separated by comma's. Even if a function has no arguments the parenthesis are mandatory. Arguments are passed by value. There is no type checking when the function is called, and more arguments can be sent to the function then are stated in the definition.
``` python
# Prepare Fibonacci sequence for n elements, return as list
#
def fibonacci(n)
    list fib
    int f0 = 0, f1 = 1, fn, i

    while i < n
        if i <= 1
            fn = i
        else
            fn = f0 + f1
            f0 = f1, f1 = fn
        fib.append(fn)
        i += 1

    return fib

int n = 10
print "Fibonacci sequence for ", n, " elements: ", fibonacci(n), "\n\n"
```
A function returns when it reaches the end of its statement block or when a *return* statement is encountered. Via the *return* statement a return value can be explicitly specified. Without this statement, of when using just *return* the return value is considered to be integer 0. The return value of a function can be used immediately, so a function can appear everywhere where a variable can be used. Any data type can be returned by a function, including lists and strings.
Variables are defined within the scope of a function. If defined outside of a function a variable is global. Functions are always defined globally.
##### Importing modules
Via the *import* statement program code from other files can be loaded. Any file which is imported is executed immediately. Any functions are added to the global list and any statement or declaration outside a function definition is executed.
``` python
str file = "file1.ext"

import "file2.ext", file
```
##### Input and output
Information can be send to the standard output via the *print* statement. Any number of expressions separated by comma's can follow *print*.
``` python
>>> str s = "Hello"
>>> print s, " there ", 1 + 2 - 3.14
Hello there -0.14
```
The *input* statement reads data from the standard input into a variable. Input must be ended by a newline. Optionally a string can be specified which is printed before the input is read. Multiple variables can be read using a single input statement.
``` python
input "Please enter your name: " name
input first_name, last_name
```
##### Various
The *pass* keyword is a no-operation statement.
##### Grammar in EBNF
For an explantion of the EBNF notation used here see [EBNF syntax.txt](EBNF%20syntax.txt).
For a graphical representation of the syntax see [EXIN syntax diagram](EXIN%20syntax%20diagram.pdf).

```
/*	EXIN grammar.
 *
 *	For an interactive graphical representation paste this grammar into http://bottlecaps.de/rr/ui
 */

program ::= (statement | NEWLINE)* EOF

/* statements */

statement ::= declaration_stmnt | import_stmt | print_stmnt | input_stmnt | return_stmnt | if_stmnt | while_stmnt | do_stmnt | break_stmnt | continue_stmnt | pass_stmnt | expression_stmnt

declaration_stmnt ::= variable_declaration | function_declaration

variable_declaration ::= type identifier ( '=' assignment_expr )? ( ',' identifier ( '=' assignment_expr )? )* NEWLINE

type ::= 'char' | 'int' | 'float' | 'str' | 'list'

function_declaration ::= 'def' identifier '(' (identifier ( ',' identifier )* )? ')' block

identifier ::= alphabetic ( alphabetic | digit | '_' )*

block ::= NEWLINE INDENT statement+ DEDENT

import_stmt ::= 'import' assignment_expr ( ',' assignment_expr )* NEWLINE  /* assignment must result in string */

print_stmnt ::= 'print' assignment_expr ( ',' assignment_expr )* NEWLINE

input_stmnt ::= 'input' string? identifier ( ',' string? identifier )* NEWLINE

return_stmnt ::= 'return' expression? NEWLINE

if_stmnt ::= 'if' expression block ( 'else' block )?

while_stmnt ::= 'while' expression block

do_stmnt ::= 'do' block 'while' expression NEWLINE

break_stmnt ::= 'break' NEWLINE

continue_stmnt ::= 'continue' NEWLINE

pass_stmnt ::= 'pass' NEWLINE

expression_stmnt ::= expression NEWLINE

/* expressions */

expression ::= assignment_expr ( ',' expression )*

assignment_expr ::= logical_or_expr ( ( '=' | '+=' | '-=' | '*=' | '\=' | '%=' ) assignment_expr )*

logical_or_expr ::= logical_and_expr ( 'or' logical_or_expr )*

logical_and_expr ::= equality_expr ( 'and' logical_and_expr )*

equality_expr ::= relational_expr ( ( '==' | '!=' | '<>' ) equality_expr )*

relational_expr ::= additive_expr ( ( '<'| '>' | '<=' | '>=' ) relational_expr )*

additive_expr ::= mult_expr ( ( '+' | '-' ) additive_expr )*

mult_expr ::= unary_expr ( ( '*' | '/' | '%' ) mult_expr )*

unary_expr ::= ( '+' | '-' | '!' )? primary_expr

primary_expr ::= function_call | variable | constant | '(' expression ')'

function_call ::= identifier '(' (assignment_expr ( ',' assignment_expr )* )? ')'

/* variables and constants */

variable ::= numeric_variable | sequence_variable

numeric_variable ::= char_variable | integer_variable | float_variable

sequence_variable ::= ( string_variable | list_variable ) ( subscript? ( '.' method )? )

method ::= list_insert | list_append | list_remove | list_len | string_len

list_insert ::= 'insert' '(' index ','  logical_or_expr ')'

list_append ::= 'append' '(' logical_or_expr ')'

list_remove ::= 'remove' '(' index ')'

list_len ::= 'len'

string_len ::= 'len'

char_variable ::= 'identifier of variable of type char'

integer_variable ::= 'identifier of variable of type int'

float_variable ::= 'identifier of variable of type float'

string_variable ::= 'identifier of variable of type str'

list_variable ::= 'identifier of variable of type list'

subscript ::= '[' ( index | slice ) ']'

index ::= logical_or_expr

slice ::= logical_or_expr? ':' logical_or_expr?

constant ::= numeric_const | character_const | string_const | list_const

numeric_const ::= number

character_const ::= "'" character "'"

string_const ::= string

list_const ::= '[' ( assignment_expr ( ',' assignment_expr )* )? ']'

/* low level definitions */

string ::= '"' character* '"'

character ::= 'any character except single quote' | escape

alphabetic ::= [a-zA-Z]

escape ::= '\' [bnfrtv\'"0]

number ::= integer | float

integer ::= ( '+' | '-' )? digit+

float ::= ( '+' | '-' )? digit+ '.' ( digit+ )? ( ('e' | 'E') ( '+' | '-' )? digit+ )?

digit ::= [0-9]
```
