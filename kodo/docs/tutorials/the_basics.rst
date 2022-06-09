The Basics
----------

In the following, we will walk through the process of creating an encoder,
decoder, and generator for coding a single buffer of data.

To do so we need to select three key parameters configuring the erasure
correcting code:

* The number of ``symbols``,
* The ``symbol_bytes``, and
* The finite field.

In general if a block of data is to be encoded, it's split into a number of
symbols of a specific size. If you multiply the number of
symbols with the symbol size, you get the total amount of data in bytes that
will be encoded or decoded per generation.

.. note:: In network applications, a single symbol typically corresponds to
          a single packet (for example, a UDP datagram).

Let us briefly outline the impact of changing the three parameters.

Number of Symbols
.................

Denotes the number of symbols in a block/generation. Increasing this
number will have the following effects:

* The computational complexity will increase, and can therefore slow
  down the encoding/decoding.
* For some variants of RLNC, the per-packet overhead will increase due to added
  coding coefficients.
* The per-symbol decoding delay will become larger: when we increase the
  number of encoded symbols, the decoder has to receive more symbols before
  decoding a full block.
* The protocol complexity can be decreased. If the number of symbols
  is increased so that all the data can fit in a single generation, then
  the protocol will only have to handle a single generation. But if multiple
  generations are needed, the receivers will have to tell from which
  generations the server should send data, and hence increasing the complexity
  of the protocol.
* The need for a high field size decreases (which is an advantage
  since, in short, a higher field size leads to higher complexity).
  The reason for this is that when the decoder is only missing a few
  symbols, the chance for it to receive a *useful* encoded symbol
  decreases. This reduction depends on the field size (higher is
  better). You pay this price at each generation, but if a generation
  contains many symbols this issue becomes smaller. Furthermore with
  many symbols, the generations will be bigger, and hence fewer
  generations are needed.

Symbol Bytes
............

Denotes the size of each symbol in bytes. The choice of symbol size
typically depends on the application. For network applications we may
choose the symbol size according to the network MTU (Maximum Transfer
Unit) so that datagrams do not get fragmented as they traverse the
network. In those cases, symbol sizes are typically around 1300-1400
bytes. On the other hand, for storage applications the symbol size is
typically much larger, e.g., in the order of several megabytes.

Finite Field
............

The finite field determines the core mathematics. Most erasure
correcting codes are based on :ref:`finite fields<faq:What is a finite field?>`.

* Increasing the field size will increase the probability of successful
  decoding.
* However it will typically also lead to increased computational complexity
  which results in slower applications.

We're now ready to look at the code. We will go through the code in a
step-by-step manner to finally end up with something that resembles the
following:

.. literalinclude:: ../../examples/tutorial/block_basic.cpp
    :language: c++
    :linenos:
    :lines: 20-

Initially we define the three parameters that we mentioned above.
For the finite field parameter, you can choose from the following fields
``binary``, ``binary4``, ``binary8``, or ``binary16``.

.. literalinclude:: ../../examples/tutorial/block_basic.cpp
    :language: c++
    :linenos:
    :start-after: //! [0]
    :end-before: //! [1]

Once the key parameters are selected, we are ready to create and configure an
encoder, decoder and generator. These will be needed to perform the actual
coding.

.. literalinclude:: ../../examples/tutorial/block_basic.cpp
    :language: c++
    :linenos:
    :start-after: //! [1]
    :end-before: //! [2]

Before the encoding and decoding of data can begin, we need to create
a few buffers.

.. literalinclude:: ../../examples/tutorial/block_basic.cpp
    :language: c++
    :linenos:
    :start-after: //! [2]
    :end-before: //! [3]

The first buffer is the ``symbol`` buffer. Once we start coding, this
buffer will contain a single encoded symbol which we can "transmit" to
the decoder. Besides the encoded symbol data, a coefficients buffer is also
needed. 
This contains the coefficients describing how the symbol was encoded.

The size of this data depends on the chosen finite field. The needed size of
the buffers are provided by ``encoder.symbol_bytes()`` and
``generator.max_coefficients_bytes()``, respectively.

The third buffer, ``data_in``, contains the data we wish to
encode. As mentioned earlier the size of this buffer is the number of
symbols multiplied by the symbol size. For convenience we can use the
``block_bytes`` function to get this value. In this case we are not
encoding real data, so we just fill the ``data_in`` buffer with some
randomly generated bytes. After this, we can call the ``set_symbols_storage``
function on the encoder to specify which buffer it should encode.

We also define the ``data_out`` buffer for the decoder and call the
``set_symbols_storage`` function. The decoder will decode the symbols into
this user-defined buffer.

.. note:: The ``data_in`` and ``data_out`` buffers should not be destroyed
          or moved after they are associated with the encoder and decoder.
          If these buffers are compromised, then the coding operations will
          be corrupted.

Finally, we have everything ready to start the coding. This is done in
a loop until the decoding has successfully completed.

.. literalinclude:: ../../examples/tutorial/block_basic.cpp
    :language: c++
    :start-after: //! [3]
    :end-before: //! [4]
    :linenos:

We use a variable ``encoded_count`` to keep track of the number of
symbols we've encoded. When we finish, this number should match the number of
``symbols``, as all data is passed safely on to the decoder.


The loop stops when the decoder's ``is_complete`` function returns true. This
happens when all symbols have been decoded.  The encoder encodes into
the symbol buffer based on the coefficients generated by the generator. 
The symbol and coefficients buffer is passed to the decoder which decodes
the encoded data and thereby increases its rank. When the decoding process is
completed, the decoded data will be available in the ``data_out`` buffer.

Finally, just to make sure, we check that the decoder correctly decoded the
symbols. This means that the content of ``data_in`` and ``data_out`` should now
be equal.

.. literalinclude:: ../../examples/tutorial/block_basic.cpp
    :language: c++
    :linenos:
    :start-after: //! [4]
    :end-before: //! [5]
