#!/usr/bin/python
# Output the specified range of numbers with their
# bits reversed as a lookup table suitable for C
print "const uint8_t reverse_bits[]={ \\"
for n in range(32):
    print int('{:08b}'.format(n)[::-1], 2),",\\"
print "};"
