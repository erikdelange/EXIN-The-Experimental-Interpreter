list l = [0,1,2,3]

for i in l
    print "i = ", i, "\n"

for j in l
	if j == 1
	    continue  # skip 1
    print "j = ", j, "\n"


for k in l
	if k == 2
		break  # stop before 2
    print "k = ", k, "\n"

for c in "abcdef"
    print c, "\n"

for i in "ab"[1:]
	print i, "\n"

print c.type, "\n"
print l.type, "\n"
print i.type, "\n"
print (1.2).type, "\n"
print "abc".type, "\n"

for element in [1, 2.0, "abc", 'c']
    print element, " ", element.type, "\n"
