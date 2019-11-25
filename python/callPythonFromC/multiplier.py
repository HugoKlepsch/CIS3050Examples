
#
# Python module -- no mainline
# Access this function by importing the module and calling it
#

def multiply(a, b):
    print "Will compute", a, "time", b
    c = 0
    for i in range(0, a):
        c = c + b
    return c

if __name__ == "__main__": print "Multiply module - not a full program"

