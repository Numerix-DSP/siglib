# SigLib Python example program
# Copyright (c) 2023 Delta Numerix

import siglib_wrap

# Here you can display the attributes of the library but this is not advised because of the length of the output !
#print ('')
#print ('Library attributes:')              # Print the attributes of the library
#for a in dir(siglib_wrap):
#    print (a)
#print ('')

a = [1.2, 3.4, 5.6, 7.8, 9.0]
# define destination array to put results into
b = [1.2, 3.4, 5.6, 7.8, 9.0]

print ('')
print ('PI = ')                             # Print a constant defined in SIGLIB
print (siglib_wrap.SIGLIB_PI)
print ('')

print ('')
print ('SigLib Version = ')
print (siglib_wrap.SUF_SiglibVersion())
print ('')

p_a = siglib_wrap.doubleArray (5)
p_b = siglib_wrap.doubleArray (5)

for index, item in enumerate(a):            # Copy data to array p_a
    p_a[index] = item

print ('a = ')                              # Print contents of a
for i in range(0,5):
    print(p_a[i])
print ('')

siglib_wrap.SDA_Multiply (p_a, siglib_wrap.SIGLIB_TWO_PI, p_b, 5)

for i in range(0,5):                        # Copy data to b
    b[i] = p_b[i]

print ('Scaled a = ')
print (b)
print ('')
