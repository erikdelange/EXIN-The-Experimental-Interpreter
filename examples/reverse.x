# reverse.x


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
str s

s = reverse("abracadabra")

print s, "\n\n"  # this will print 'arbadacarba'

s = reverse(s)   # turn s back in the original string

print s, 2 * "\n"

# Now let the user enter strings
#
do
    input "Enter a string to reverse (empty line to end): " s
    if s == ""
        break
    print reverse(s), "\n"
while 1
