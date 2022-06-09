Sliding Window Encode and Decode
================================

In this example shows how to use the API of ``kodo::slide`` to encode and decode
using a sliding window codec.

The following classes will be used:

* The :ref:`kodo::slide::encoder` and :ref:`kodo::slide::decoder` classes exposes
  a low-level API of a sliding window code giving you full control
  over the operations.

* The :ref:`kodo::slide::generator::random_uniform` class exposes
  the coefficient generator for the sliding window codec.

* The :ref:`kodo::slide::stream` is a simple adapter over a
  ``std::deque`` but with an API which matches the one used in
  ``kodo::slide``.

* The next helper we will use is the :ref:`kodo::slide::rate_controller`.
  This object will allow us to specify in which pattern to generate
  repair/redundancy. See also :ref:`slide_api/parameter_selection:Parameter Selection`.

In this example we will walk through the basic functionality and
discuss how to use it.

.. note:: If you see an unfamiliar term used, try to visit the
          :ref:`slide_api/definitions:Definitions` section and you should find a description.

.. literalinclude:: ../../examples/slide/encode_decode_simple.cpp
    :language: c++
    :linenos:
    :lines: 20-
