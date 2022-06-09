Parameter Selection
===================

In this section we give some basic advice on how to parameterize your code.
As a general disclaimer, this does not guarantee any specific performance from
the code, but means that the parameters are reasonable.

The following is based on rough back-of-the-envelope calculation, and
simplifies a lot of things.

.. note:: If you see an unfamiliar term used, try to visit the
          :ref:`slide_api/definitions:Definitions` section and you should find
          a description.

We will look at how to specify two settings:

1. The *coding rate*
2. The *coding window*

Coding rate ``(n,k)``
---------------------

The coding rate is often denoted ``(n,k)`` where ``n`` is the total number
of symbols to send out of which ``k`` is the original data and ``r = n - k``
is the repair symbols. The ``coding rate`` is computed as ``k/n``.

As a rule of thumb, we need to specify the ``coding rate`` lower
than one minus the loss rate.

Example 1: rate selection
.........................

* Worst-case packet loss rate: 50%
* Then the coding rate should also be lower than ``(1-0.5) = 0.5``
  e.g. ``n=2`` and ``k=1`` our rate is then ``k/n = 1/2`` which is just enough
  to cope with the packet loss (in the worst case). In practice, you would
  probably want to lower the rate.

Example 2: rate selection
.........................

* Worst-case packet loss rate: 25%
* Then the coding rate should also be lower than ``(1-0.25) = 0.75`` e.g.
  ``n=3`` and ``k=2`` our rate is then ``k/n = 2/3 = 0.66`` which should be
  enough to cope with the packet loss.

If we want to minimize delay we should try to specify ``n`` and ``k`` as low as
possible. Since this means we will generate repair traffic more often.

Selecting ``(n,k)``
...................

In general, we should try to minimize ``(n,k)`` but still within a comfortable
margin of one minus the packet loss rate. 
With a smaller ``(n,k)`` parameter, we can interleave repair packets more often,
and therefore the decoder will more often have the chance to decode and release
a symbol.

If we select a coding rate of ``(n=3,k=2)``, this means that after every
``k=2`` packets we send ``r=1`` repair packet.

This has the same coding rate as a ``(n=6,k=4)``, but here we send two
repair packets after every four packets. We therefore wait more before
generating repair.

Coding window
-------------

Essentially, the coding window can be determined based on two different
considerations:

1. In a delay-constrained scenario, it does not make sense to
   keep including source symbols that are "too old".

2. If we are not constrained by delay, the computational requirements
   and the memory consumption of the algorithm become the limiting
   factors.

In the latter case, the right parameter combination heavily depends on the
target platform (e.g. high-end server or limited embedded device).
Depending on the use-case, benchmarking may be a good way to find a
suitable set of parameters for the given platform.

For the delay-constrained use-case, it is usually possible to determine the
coding window based on the delay requirement.

Encoding window
...............

Say we have an application that generates a fixed stream of 1 MBps
and has a latency requirement of 60 ms. If we choose our symbol
size to 1400 bytes, then we find that our coding window can be:


.. math::

    w = (1000000 * 60 / 1000) / 1400 = 42.8


Decoding window
...............

The decoder receives symbols encoded with the encoder's window.
But, it may itself choose to have a larger decoding window. This might be
advantageous, because even though a symbol is now considered
"too" old, it may still be useful for decoding symbols that are still
within the deadline.

As a general rule of thumb, specifying the decoding window as two
times the encoding window works well in most cases.

.. warning:: If the decoding window is smaller than the encoding window the
             decoder will not be able to use any repair symbols.
