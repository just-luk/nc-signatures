
Coefficients' Memory Layout
===========================

The codec expect the coding coefficients to have a specific memory layout.
The reason for this memory layout is to achieve two goals:

1. The bit position of a coefficient corresponding to a specific symbol
   index in the ``window`` should be constant within memory.

2. It should be possible to resize the coding ``window`` without remapping
   coefficients.

We will use LSB 0 bit numbering (see `Bit numbering (bit endianness)`_).

Example: Binary field
.....................

In the binary field each coefficient is a single bit.

Initial example, if we have three symbols in the ``window`` with the first
symbol being index zero (we call this the ``window_lower_bound``).

In this case we would have three coefficients at bit index 0, 1 and 2 below
represented by capital ``C``::

      7   6   5   4   3   2   1   0
    +-------------------------------+
    | 0   0   0   0   0   C   C   C |
    +-------------------------------+
                                  ^
                                  |
     least significant bit +------+

As the ``window`` moves to encode a different set of symbols, so will the
coefficients offset within the byte e.g. lets see the coefficients for
encoding symbol 2,3 and 4::

      7   6   5   4   3   2   1   0
    +-------------------------------+
    | 0   0   0   C   C   C   0   0 |
    +-------------------------------+
                                  ^
                                  |
     least significant bit +------+

Notice how the coefficient for symbol 2 remains at the same position with in
the byte.

Once the ``window_lower_bound`` reaches 6, coefficients will "spill" and we
need to use an additional byte::

      7   6   5   4   3   2   1   0   7   6   5   4   3   2   1   0
    +-------------------------------+-------------------------------+
    | C   C   0   0   0   0   0   0 | 0   0   0   0   0   0   0   C |
    +-------------------------------+-------------------------------+
                                  ^                               ^
                                  |                               |
         least significant +------+      least significant +------+
         bit in first byte               in second byte

Effectively bit index 0 in the second byte corresponds to the coefficient
encoding the symbol with index 8.

Once the coding ``window`` leaves the first byte i.e. ``window_lower_bound
>= 8`` we can omit the fist byte since it only contains leading zeros.

It is important that all bits not in the ``window`` are zero'ed.

Example: Binary8 field
......................

In the binary8 field each coefficient is eight bits (or one byte).

As in the binary example, lets imagine we have three symbols in the
``window``. Since we are in binary8 we simply need set three bytes::

                      0                 8                 16
      +-----------------+-----------------+-----------------+
      | C C C C C C C C | C C C C C C C C | C C C C C C C C |
      +-----------------+-----------------+-----------------+
               ^                 ^                 ^
               |                 |                 |
    first byte +     second byte +      third byte +

    increasing addresses ------->

The coding coefficient corresponding to symbol zero is the first byte, the
second byte corresponds to symbol with index one and so forth.
As the ``window`` moves we interpret the first byte as the one corresponding
to the symbol at the ``window_lower_bound``.

Bit numbering (bit endianness)
------------------------------
In the following we will use LSB 0 bit numbering. LSB 0 is where the bit
numbering starts at zero for the least significant bit (LSB). As an example
say we have a single byte (8 bits)::

     least significant bit +------+
                                  |
                                  v
    +-------------------------------+
    | 0   1   0   1   1   1   0   0 |
    +-------------------------------+
      ^
      |
      +-----------+ most significant bit

Lets number the bits inside byte given earlier according to the LSB 0
bit numbering::

     7   6   5   4   3   2   1   0
   +-------------------------------+
   | 0   1   0   1   1   1   0   0 |
   +-------------------------------+

In the following we will omit all but the LSB index::

                                 0
   +-------------------------------+
   | 0   1   0   1   1   1   0   0 |
   +-------------------------------+

For a stream of bytes we number assume little endian byte order least
significant byte first::

                                 0                               8
   +-------------------------------+-------------------------------+
   | 0   0   0   0   0   0   0   0 | 0   0   0   0   0   0   0   0 |
   +-------------------------------+-------------------------------+
                                 ^                               ^
                                 |                               |
        least significant +------+      least significant +------+
        bit in first byte               in second byte

                      increasing addresses ------->
