# Index en slice test on list-object
#
# The output should be three times the same

str newline = "\n"

print "Index and slice operations on list constant [\'a\',10,2.1,\"bcd\"]", newline
print "[1]    "
print ['a',10,2.1,"bcd"][1], newline
print "[-2]   "
print ['a',10,2.1,"bcd"][-2], newline
print "[:]    "
print ['a',10,2.1,"bcd"][:], newline
print "[1:]   "
print ['a',10,2.1,"bcd"][1:], newline
print "[:3]   "
print ['a',10,2.1,"bcd"][:3], newline
print "[1:3]  "
print ['a',10,2.1,"bcd"][1:3], newline
print "[1:-1] "
print ['a',10,2.1,"bcd"][1:-1], newline
print "[-1:3] "
print ['a',10,2.1,"bcd"][-1:3], newline
print "[:][1] "
print ['a',10,2.1,"bcd"][:][1], newline
print newline

print "Index and slice operations on list variable l which contains ['a',10,2.1,\"bcd\"]", newline
list l = ['a',10,2.1,"bcd"]
print "[1]    "
print l[1], newline
print "[-2]   "
print l[-2], newline
print "[:]    "
print l[:], newline
print "[1:]   "
print l[1:], newline
print "[:3]   "
print l[:3], newline
print "[1:3]  "
print l[1:3], newline
print "[1:-1] "
print l[1:-1], newline
print "[-1:3] "
print l[-1:3], newline
print "[:][1] "
print l[:][1], newline
print newline

print "Index and slice operations on function flist() which returns list [\'a\',10,2.1,\"bcd\"]", newline
print "flist() = ", flist(), newline
print "[1]    "
print flist()[1], newline
print "[-2]   "
print flist()[-2], newline
print "[:]    "
print flist()[:], newline
print "[1:]   "
print flist()[1:], newline
print "[:3]   "
print flist()[:3], newline
print "[1:3]  "
print flist()[1:3], newline
print "[1:-1] "
print flist()[1:-1], newline
print "[-1:3] "
print flist()[-1:3], newline
print "[:][1] "
print flist()[:][1], newline
print newline

def flist()
    return ['a',10,2.1,"bcd"]
