# reverse.xi


# Reverse a string

def reverse(s)
    str new
    int index = s.len

    while index
        index -= 1
        new += s[index]

    return new

str s

# reverse string s
s = reverse("abracadabra")
print s, "\n"

# and reverse it back to the original string
s = reverse(s)
print s, "\n"

# now let the user enter strings
do
    input "Please enter a string (return or End to exit): " s
    if s == "End" or s == ""
        break
    print reverse(s), "\n"
while 1
