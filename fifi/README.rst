|Linux make-specs| |Windows make-specs| |MacOS make-specs| |Linux CMake| |Windows CMake| |MacOS CMake| |Raspberry Pi| |Valgrind| |No Assertions| |Clang Format| |Cppcheck|

.. |Linux make-specs| image:: https://github.com/steinwurf/fifi/actions/workflows/linux_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/fifi/actions/workflows/linux_mkspecs.yml

.. |Windows make-specs| image:: https://github.com/steinwurf/fifi/actions/workflows/windows_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/fifi/actions/workflows/windows_mkspecs.yml

.. |MacOS make-specs| image:: https://github.com/steinwurf/fifi/actions/workflows/macos_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/fifi/actions/workflows/macos_mkspecs.yml

.. |Linux CMake| image:: https://github.com/steinwurf/fifi/actions/workflows/linux_cmake.yml/badge.svg
   :target: https://github.com/steinwurf/fifi/actions/workflows/linux_cmake.yml

.. |Windows CMake| image:: https://github.com/steinwurf/fifi/actions/workflows/windows_cmake.yml/badge.svg
   :target: https://github.com/steinwurf/fifi/actions/workflows/windows_cmake.yml

.. |MacOS CMake| image:: https://github.com/steinwurf/fifi/actions/workflows/macos_cmake.yml/badge.svg
   :target: https://github.com/steinwurf/fifi/actions/workflows/macos_cmake.yml

.. |Raspberry Pi| image:: https://github.com/steinwurf/fifi/actions/workflows/raspberry_pi.yml/badge.svg
   :target: https://github.com/steinwurf/fifi/actions/workflows/raspberry_pi.yml

.. |Clang Format| image:: https://github.com/steinwurf/fifi/actions/workflows/clang-format.yml/badge.svg
   :target: https://github.com/steinwurf/fifi/actions/workflows/clang-format.yml

.. |No Assertions| image:: https://github.com/steinwurf/fifi/actions/workflows/nodebug.yml/badge.svg
   :target: https://github.com/steinwurf/fifi/actions/workflows/nodebug.yml

.. |Valgrind| image:: https://github.com/steinwurf/fifi/actions/workflows/valgrind.yml/badge.svg
   :target: https://github.com/steinwurf/fifi/actions/workflows/valgrind.yml

.. |Cppcheck| image:: https://github.com/steinwurf/fifi/actions/workflows/cppcheck.yml/badge.svg
   :target: https://github.com/steinwurf/fifi/actions/workflows/cppcheck.yml

.. image:: ./fifi.svg
   :height: 200px
   :align: center

Fifi is a library for finite field arithmetics. It provides an optimized
implementation for operations in various finite fields, such as GF(2),
GF(2⁴), GF(2⁸) and GF(2¹⁶). To provide high performance, it
utilizes a wide range of hardware optimizations for the different fields,
especially the SIMD instruction sets of modern CPUs.

The Fifi repository: https://github.com/steinwurf/fifi

The ``examples`` folder contains some programs that demonstrate the usage
of the available API.

.. image:: http://buildbot.steinwurf.dk/svgstatus?project=fifi
    :target: http://buildbot.steinwurf.dk/stats?projects=fifi

If you have any questions or suggestions about this library, please contact
us at our developer mailing list (hosted at Google Groups):

* http://groups.google.com/group/steinwurf-dev

Use as Dependency in CMake
--------------------------

To depend on this project when using the CMake build system, add the following
in your CMake build script:

::

   add_subdirectory("/path/to/fifi" fifi)
   target_link_libraries(<my_target> steinwurf::fifi)

Where ``<my_target>`` is replaced by your target.

License
-------

A valid license is required if you wish to use this library. Please
request a license by **filling out the license request** form_.

Fifi is available under a research- and education-friendly license,
see the details in the `LICENSE.rst file
<https://github.com/steinwurf/fifi/blob/master/LICENSE.rst>`_.

For commercial projects, you can use the same license request form.

.. _form: http://steinwurf.com/license/

Documentation
-------------

Please read our general documentation here to get started:
http://docs.steinwurf.com

The Fifi documentation is located here:
https://fifi.steinwurf.com/


