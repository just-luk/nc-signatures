Systematic Coding
-----------------

A simple, yet clever technique called systematic encoding can be used
to improve the performance of network coding. The way it works is to
initially send everything uncoded, and then start the encoding.

The benefit of this is that as the receivers initially have no data, all
data will be useful for them. So if the symbols are safely received by
the decoder, it can get the data "for free" without the need for decoding.

The following sample code is also based on the basic example.

.. literalinclude:: ../../examples/tutorial/block_systematic_coding.cpp
    :language: c++
    :linenos:
    :lines: 20-

What's changed in this example here is the use of ``encode_systematic_symbol`` and
``decode_systematic_symbol``.

.. literalinclude:: ../../examples/tutorial/block_systematic_coding.cpp
    :language: c++
    :start-after: //! [0]
    :end-before: //! [1]
    :linenos:

In this example we initially use the ``encode_systematic_symbol`` function to
send each symbol uncoded. Note this function doesn't use a coefficient vector
to generate the symbol, but rather it needs the index of the symbol.
This index is likewise used by the decoder when calling
``decode_systematic_symbol``. This means that in a network scenario the index
needs to be communicated as well as the symbol buffer.

The output of the program hasn't changed:

.. code-block:: none

    Encoded count = 27
    Dropped count = 11
    
For most use cases, the systematic phase is beneficial. 

However it should be avoided if

* The state of receivers is unknown. If that's the case, using the
  systematic approach might result in sending redundant data, as the
  receivers might already have some of the data.
* The setup has multiple sources. If this is the case, the sources
  should not send the same data, as this can be redundant for the
  receivers.
