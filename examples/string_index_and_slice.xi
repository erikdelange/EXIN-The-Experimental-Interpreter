# Index en slice test onstring-object
#
# The output should be three times the same


str newline = "\n"

print "Index and slice operations on str constant \"abcd\"", newline
print "[1]    "
print "abcd"[1], newline
print "[-2]   "
print "abcd"[-2], newline
print "[:]    "
print "abcd"[:], newline
print "[1:]   "
print "abcd"[1:], newline
print "[:3]   "
print "abcd"[:3], newline
print "[1:3]  "
print "abcd"[1:3], newline
print "[1:-1] "
print "abcd"[1:-1], newline
print "[-1:3] "
print "abcd"[-1:3], newline
print "[:][1] "
print "abcd"[:][1], newline
print newline

print "Index and slice operations on str variable s which contains \"abcd\"", newline
str s = "abcd"
print "[1]    "
print s[1], newline
print "[-2]   "
print s[-2], newline
print "[:]    "
print s[:], newline
print "[1:]   "
print s[1:], newline
print "[:3]   "
print s[:3], newline
print "[1:3]  "
print s[1:3], newline
print "[1:-1] "
print s[1:-1], newline
print "[-1:3] "
print s[-1:3], newline
print "[:][1] "
print s[:][1], newline
print newline

print "Index and slice operations on function fstr() which returns \"abcd\"", newline
print "fstr() = \"abcd\"", newline
print "[1]    "
print fstr()[1], newline
print "[-2]   "
print fstr()[-2], newline
print "[:]    "
print fstr()[:], newline
print "[1:]   "
print fstr()[1:], newline
print "[:3]   "
print fstr()[:3], newline
print "[1:3]  "
print fstr()[1:3], newline
print "[1:-1] "
print fstr()[1:-1], newline
print "[-1:3] "
print fstr()[-1:3], newline
print "[:][1] "
print fstr()[:][1], newline
print newline

def fstr()
    return "abcd"
