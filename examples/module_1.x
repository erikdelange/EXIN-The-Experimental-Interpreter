# module_1.x
#
# example of import a module which in itself also imports modules

print "now in module_1.x\n"

str s = "module_2.x"

import s

print "now in module_1.x\n"

m2(10)
m2(3.14)
m2(name)
m3()
m4()

import "module_2.x"  # has not effect

print "now exiting module_1.x\n"
