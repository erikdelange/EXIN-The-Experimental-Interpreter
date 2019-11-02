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
