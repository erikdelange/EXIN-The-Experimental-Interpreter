# power.x

main()


def main()
    auto(10, 2)
    user_input()


# manual entry of base and exponent
#
def user_input()
    char more
    float base
    int exponent

    do
        input "Base : " base, "Exponent : " exponent
        print "Power(", base, ", ", exponent, ") = ", power(base, exponent), "\n"
        input "Continue? (y/n) " more
    while more != 'n'


# raise all numbers between 0 and i to the power of e
#
def auto(i, e)
    int b
    while b <= i
        print b, "^", e, " = ", power(b,e), "\n"
        b += 1
    print "\n"


# base raised to the exponent power
#
# b = base
# e = exponent (must be an integer)
#
def power(b, e)
    float x, r
    int is_negative = 0

    if e == 0
        return 1

    if e < 0
        is_negative = 1
        e = 0 - e

    x = b
    while e > 1
        x *= b
        e -= 1

    if is_negative
        x = 1 / x

    return x
