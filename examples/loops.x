# loops.x
#
# loops in various forms

int i

i = 100000

while (i -= 1)
    pass

i = 100000

do
    pass
while (i -= 1)

def loop_w(i)
    while i
        print i -= 1, " "

loop_w(20)
print "\n"

def loop_d(i)
    do
        print i -= 1, " "
    while i

loop_d(20)
print "\n"

print "done\n"
