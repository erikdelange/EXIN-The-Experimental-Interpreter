# Index en slice test on list-object
#
# The output should be three times the same

print "Index and slice operations on list constant [\'a\',10,2.1,\"bcd\"]"
print "[1] =", ['a',10,2.1,"bcd"][1]
print "[-2] =", ['a',10,2.1,"bcd"][-2]
print "[:] =", ['a',10,2.1,"bcd"][:]
print "[1:] =", ['a',10,2.1,"bcd"][1:]
print "[:3] =", ['a',10,2.1,"bcd"][:3]
print "[1:3] =", ['a',10,2.1,"bcd"][1:3]
print "[1:-1] =", ['a',10,2.1,"bcd"][1:-1]
print "[-1:3] =", ['a',10,2.1,"bcd"][-1:3]
print "[:][1] =", ['a',10,2.1,"bcd"][:][1]
print

print "Index and slice operations on list variable l which contains ['a',10,2.1,\"bcd\"]"
list l = ['a',10,2.1,"bcd"]
print "[1] =", l[1]
print "[-2] =", l[-2]
print "[:] =", l[:]
print "[1:] =", l[1:]
print "[:3] =", l[:3]
print "[1:3] =", l[1:3]
print "[1:-1] =", l[1:-1]
print "[-1:3] =", l[-1:3]
print "[:][1] =", l[:][1]
print

print "Index and slice operations on function flist() which returns list [\'a\',10,2.1,\"bcd\"]"
print "flist() =", flist()
print "[1] =", flist()[1]
print "[-2] =", flist()[-2]
print "[:] =", flist()[:]
print "[1:] =", flist()[1:]
print "[:3] =", flist()[:3]
print "[1:3] =", flist()[1:3]
print "[1:-1] =", flist()[1:-1]
print "[-1:3] =", flist()[-1:3]
print "[:][1] =", flist()[:][1]
print

def flist()
    return ['a',10,2.1,"bcd"]
