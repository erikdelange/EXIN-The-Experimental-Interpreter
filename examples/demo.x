# demo.x
#

str newline = "\n"

# -----------------------------------------------------------
# Create a list with two dimensions - so a list within a list
#
print "Create list which includes another list", newline

list outer, inner
int i = 0, j

while i != 3
    j = 0
    inner = []
    while j < 4
        inner.append((i + 1) * 10 + j)
        j += 1
    outer.append(inner)
    print "inner = ", inner, newline
    i = i + 1

print "outer = ", outer, newline * 2

print "Print list which includes another list", newline

i = 0
do
    j = 0
    while j < outer[i].len
        print outer[i][j], " "
        j += 1
    i += 1
while i < outer.len

print "\n\n"


# ------------------------------------------
# Function with arguments and a return value
#

# Multiply two arguments - also works for strings and list
#
print "Multiply two values", newline

def multiply(x, y)
    print x, " * ", y, " = "
    return x * y

float a = multiply(1.1, 2.1)
print a, '\n'

int b = multiply(2, 3)
print b, '\n'

str s = multiply("abc", 4)
print s, '\n'

list t = multiply([1.2, "ab", 'c'], 2)
print t, newline * 2


# ------------------------------
# Add numbers, strings and lists
#
print "Add two values", newline

def add(x, y)
    print x, " + ", y, " = "
    return x + y

print add(1.1, 2.2), newline		# two floats
print add("abc", "def"), newline	# two strings
print add([1,2], [3,4]), newline	# two lists
print add("abc", 'd'), newline		# string and char
print add("abc", 123), newline		# string and int
print add("abc", 3.14), newline		# string and float


# --------------
# Nested if else
#
print newline, "if " + "then else", newline  # string concatenation via +

# Test if value is less then, equal if greater then zero
#
def test_for_zero(v)
    if v < 0
        print "v < 0, v = ", v, "\n"
    else
        if v == 0
            print "v = 0", "\n"
        else
            print "v > 0, v = ", v, "\n"

test_for_zero(-2)
test_for_zero(0)
test_for_zero(2)


# ----------
# While loop
#
print newline, "while loop", newline

def while_loop(v)
    while (v += 1) != 5
        print "v = ", v, " "

while_loop(0)
print '\n', '\n'


# -------------
# Do while loop
#
print "do while loop", newline

def do_loop(v)
    do
        print "v = ", v += 1, " "
    while v < 5

do_loop(0)
print '\n', '\n'


# -------------------
# Cascaded assignment
#
print "Cascaded assignment (a = b = c = d = 3 * 3): "

int c
int d

a = b = c = d = 3 * 3

if a == b and c == d and d == 3 * 3
    print a, " = " , b, " = ", c, " = ", d, newline * 2


# ------------------
# Recursive function
#
print "Recursive function: "

def func(local)
    if local > 0
        func(local - 1)
    print local, " "

func(9)
print newline * 2


# -----------------------------------------------------
# Function without parameter which returns a new string
#
print "Function which returns string f1: "

def f1()
    return "f1"

print f1(), newline * 2


# -----------------------------------------------
# Function which returns a new list of i elements
#
print "Function which returns a new list of i elements:\n"

def create_list(i)
    list l

    while i
        l.insert(0, i)  # insert before element nr 0
        i -= 1

    return l

print "create_list(6)      = ", create_list(6), "\n"
print "create_list(6)[1]   = ", create_list(6)[1], "\n"
print "create_list(6)[-1]  = ", create_list(6)[-1], "\n"  # -1 is first element from end
print "create_list(6)[:2]  = ", create_list(6)[:2], "\n"
print "create_list(6)[2:]  = ", create_list(6)[2:], "\n"
print "create_list(6)[2:4] = ", create_list(6)[2:4], "\n"
print "create_list(6)[2:-2]= ", create_list(6)[2:-2], "\n\n"


# ---------------------------------------------------------
# Prepare Fibonacci sequence for n elements, return as list
#
print "Fibonacci sequence"

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
print "for ", n, " elements: ", fibonacci(n), "\n\n"


# -----------------------------------
# Direct use of function return value
#
def power(g, e)
    float x, r
    int negative = 0

    if e == 0
        return 1

    if e < 0
        negative = 1
        e = 0 - e

    x = g
    while  e > 1
        x *= g
        e -= 1

    if negative
        x = 1 / x

    return x

print "Direct use of function return value: "
print "2^3 + 1.1 = ", power(2, 3) + 1.1, newline, newline


# ----------------------------------------------------------------------
print "Scientific notation of float constants: 2.1E2 = ", 2.1E2, newline


# -----------------------------------------------------------
# Return from EXIN with a calculated value (script exit code must be 7)
#
print "\nThis script will now exit with a return value of ", 1 + 2 * 3
return 1 + 2 * 3
