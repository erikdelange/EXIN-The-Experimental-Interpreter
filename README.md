# EXIN - An Experimental Interpreter

**June 6, 2020 <br><br>
This repository has been superseded by [EXIN-AST](https://github.com/erikdelange/EXIN-AST-The-Experimental-Interpreter), and will not be developed any further. While still implementing the same language [EXIN-AST](https://github.com/erikdelange/EXIN-AST-The-Experimental-Interpreter) follows a more elegant design centered around an Abstract Syntax Tree.**
___
EXIN is an exercise in defining a programming language and writing an interpreter for it. Both are as simple as possible to make it easy to understand how the interpreter code is structured and to be able to add new features. The language resembles (as far as I know) Basic, C and Python. This is an example of EXIN code:
```
# Reverse string s
#
def reverse(s)
    str new
    int index = s.len

    while index
        index -= 1
        new += s[index]

    return new

# First a short demo
#
str s2, s1 = "abracadabra"

s2 = reverse(s1)

print -raw s1, " reversed is ", s2, 2 * "\n"

# Now let the user enter strings
#
do
    input "Enter a string to reverse (empty line to end): " s1
    if s1 == ""
        break
    print s1, "reversed is", reverse(s1)
while 1
```
The interpreter is written in C (version C99). The details of the language and the interpreter are explained in the documents listed below. EXIN is designed for fun and education (at least mine :) and you can do with it whatever you like.

- [EXIN language reference](EXIN%20language%20reference.md)
- [EXIN software architecture](EXIN%20software%20architecture.md)
- [EXIN syntax diagram](EXIN%20syntax%20diagram.pdf)
