Definitions
===========

To understand the API of kodo::slide's ECC implementation a bit of terminology
is needed:

* The ``coding rate``: Most erasure coding algorithms allow you to select a
  coding rate. The rate is typically specified by two numbers ``n`` and ``k``
  where the coding rate is ``k/n`` (``n`` divided by ``k``). They specify
  that after sending ``n`` symbols, ``k`` of those will be original source
  symbols. Often ``r`` is defined as ``r = n - k``, which denotes the number of
  repair symbols we are sending.
  See :ref:`slide_api/parameter_selection:Parameter Selection`.

* The ``stream``: This is the set of symbols currently available at the encoder
  for encoding or at the decoder for decoding. Using the API it is possible to
  either push new symbols to the front of the stream or pop symbols from the
  back of the stream.

* The ``window``: This is the set of symbols included in a specific encoding.
  The ``window`` is a subset of the ``stream``. The API allows full control over
  both the ``stream`` and ``window``.

The following diagram gives an overview of how these three concepts relate::

    Data Symbols:
     +---+  +---+  +---+  +---+  +---+  +---+  +---+  +---+
     | 0 |  | 1 |  | 2 |  | 3 |  | 4 |  | 5 |  | 6 |  | 7 |      ...
     +---+  +---+  +---+  +---+  +---+  +---+  +---+  +---+
            |            stream             |
            +-------------------------------+
                    |    window      |
                    +----------------+                           ...

In the above example the ``stream`` includes five symbols, 1 through 5. The
``window`` which is a subset of the stream includes symbol 2 through 4.
To recap:

* Symbols 1 through to 5 are currently available in memory (in the ``stream``).
* Symbols 2 through to 4 are in the ``window``, i.e., they will be included in
  a coded symbol produced by the encoder. At the decoder the next coded symbol
  read is expected to be a linear combination of the symbols in the ``window``.
