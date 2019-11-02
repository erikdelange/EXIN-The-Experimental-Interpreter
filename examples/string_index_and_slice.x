# Index and slice test on string objects
#
# The output should be three times the same

print "Index and slice operations on str constant \"abcd\""
print "[1] =", "abcd"[1]
print "[-2] =", "abcd"[-2]
print "[:] =", "abcd"[:]
print "[1:] =", "abcd"[1:]
print "[:3] =", "abcd"[:3]
print "[1:3] =", "abcd"[1:3]
print "[1:-1] =", "abcd"[1:-1]
print "[-1:3] =", "abcd"[-1:3]
print "[:][1] =", "abcd"[:][1]
print

print "Index and slice operations on str variable s which contains \"abcd\""
str s = "abcd"
print "[1] =", s[1]
print "[-2] =", s[-2]
print "[:] =", s[:]
print "[1:] =", s[1:]
print "[:3] =", s[:3]
print "[1:3] =", s[1:3]
print "[1:-1] =", s[1:-1]
print "[-1:3] =", s[-1:3]
print "[:][1] =", s[:][1]
print

print "Index and slice operations on function fstr() which returns \"abcd\""
print "fstr() = \"abcd\""
print "[1] =", fstr()[1]
print "[-2] =", fstr()[-2]
print "[:] =", fstr()[:]
print "[1:] =", fstr()[1:]
print "[:3] =", fstr()[:3]
print "[1:3] =", fstr()[1:3]
print "[1:-1] =", fstr()[1:-1]
print "[-1:3] =", fstr()[-1:3]
print "[:][1] =", fstr()[:][1]
print

def fstr()
    return "abcd"
