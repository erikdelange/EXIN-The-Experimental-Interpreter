# split.x

# Split a line in separate words and return as a list of words
#
def split(s)
    list words
    int wordlen

	print "split: sentence = ", s, "\n"

    while (s = strip(s)) != ""
        wordlen = wordlength(s)
        words.append(s[:wordlen])
        s = s[wordlen:]
    return words


# Remove leading spaces and tabs from a string and return as a new string
#
def strip(s)
    int i = 0

	print "strip: string before = |", s, "| (len=", s.len, ")\n"

    while i < s.len
        if !(s[i] == ' ' or s[i] == '\t')
            break
        i += 1

    print "strip: string after  = |", s[i:], "| (len=", s[i:].len, ")\n"

	return s[i:]


# Count the number of characters in the first word of string s
#
def wordlength(s)
    int i = 0

    while i < s.len
        if s[i] == ' ' or s[i] == '\t'
            break
        else
            i = i + 1

    return i


print split(" this is   a     sentence  "), "\n"

str s

do
    input "Enter a sentence: " s
    if s == ""
        break
    print split(s), "\n"
while 1
