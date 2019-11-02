# module_1.x
#
# example of import of a module which itself also imports modules

print "** now in module_1.x **"

str s = "module_2.x"

import s

print "back in module_1.x after import of", s

fm2(10)
fm2(3.14)
fm2(name)
fm3()
fm4()

import "module_2.x"  # has not effect

print "** leaving module_1.x **"
