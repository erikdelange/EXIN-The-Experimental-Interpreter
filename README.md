# EXIN - The Expermimental Interpreter

EXIN is an exercise in defining a progamming language and writing an interpreter for it. Both are meant to be as simple as possible to be able to understand how the interpreter is structured and to be able to add new features. The language resembles Basic, C and Python. This is an example of EXIN code:
```
# Sting reverse function
#
def reverse(s)
	str new
	int index = s.len

	while index
		index -= 1
		new += s[index]

	return new


str s

# First a short demo
s = reverse("abracadabra")
print s, "\n\n"
s = reverse(s)  # turn s back in the original string
print s, "\n\n"

# Let the user enter strings
do
	input "Please enter a string: " s
	if s == "End" or s == ""
		break
	print reverse(s), "\n"
while 1
```
The interpreter is written in C. The details of the language and the interpreter are explained in the documents listed below. EXIN is meant for educational purposes and you can do with it whatever you like.

- [EXIN language reference](EXIN%20language%20reference.md)
- [EXIN software architecture](EXIN%20software%20architecture.md)
- [EXIN syntax diagram](EXIN%20syntax%20diagram.xhtml)   

Download the syntax file before viewing (xhtml cannot be shown in Github.)
