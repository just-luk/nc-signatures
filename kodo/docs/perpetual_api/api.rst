=============
Perpetual API
=============
..
   Temporary fix for issue in wurfapi which makes arguments
   named offset point to kodo::perpetual::offset:

.. _kodo::perpetual::offset:

Overview of the public perpetual API.

The ``kodo::perpetual`` namespace of the kodo library implements perpetual
network codes.

Perpetual RLNC is a sparse and structured code where the non-zero coding
coefficients are localized to a specific part of the coding vector.

This approach allows for structured decoding, which can yield a substantially
higher throughput than random sparse codes, especially for large generation
sizes.

.. toctree::
   :maxdepth: 2

   perpetual_width
   perpetual_encoder
   perpetual_decoder
   perpetual_generator_random_uniform
   perpetual_offset_random_uniform
