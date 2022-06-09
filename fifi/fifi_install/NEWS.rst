News for fifi
=============

This file lists the major changes between versions. For a more detailed list
of every change, see the Git log.

Latest
------
* Minor: Added CMake configs for the missing examples.

35.1.3
------
* Patch: Changed title of metrics to "Field".

35.1.2
------
* Patch: Changed title of metrics to "Field Metrics".

35.1.1
------
* Patch: Fix CMake dependency issue where abacus is private, but should be
  public.

35.1.0
------
* Patch: Fix ARM thumb mode
* Minor: Updated license to Fifi Evaluation License 1.3 and
  Fifi Research License 1.2.

35.0.0
------
* Minor: Updated waf.g
* Major: Added Abacus as dependency
* Major: Counters are now called metrics.
* Major: math::to_json() is now math::metrics_to_json().

34.1.0
------
* Minor: Added SIMD example.
* Minor: Added fifi::to_string().
* Minor: Added fifi::to_field().

34.0.0
------
* Major: Change cmake object library approach.

33.0.0
------
* Major: Change cmake build to be object library based.

32.1.0
------
* Minor: Added install step to CMake.

32.0.0
------
* Major: Change `fields` namespace to `field`.

31.1.0
------
* Minor: Use constexpr for all online calculations.

31.0.2
------
* Patch: Silence all warnings if compiling without asserts

31.0.1
------
* Patch: Silence warnings if compiling without asserts

31.0.0
------
* Major: Changes type from uint32_t -> std::size_t for index and size related
  variables.

30.5.3
------
* Patch: Use `std::size_t` instead of `uint32_t` for `simd_allocator`.

30.5.2
------
* Patch: Added missing include in `src/fifi/online/invert.hpp`.

30.5.1
------
* Patch: Added add_subdirectory guard to CMakeLists.txt.

30.5.0
------
* Minor: Changed resolve step in CMake file.
* Minor: Lowered CMake version requirement.

30.4.0
------
* Minor: Add functionality to set the backend on field specific math objects.
* Minor: Added type information to field specific math objects. Making them
  easier to use in generic (templated) code.

30.3.2
------
* Patch: Use variable for resolve step in cmake.

30.3.1
------
* Patch: Fix resolve step in cmake.

30.3.0
------
* Minor: Improve cmake build file.
* Minor: Upgrade to gauge 12

30.2.0
------
* Minor: Optimize the initialization of the math objects.

30.1.0
------
* Minor: Added cmake build file.

30.0.0
------
* Major: Restructured the entire library and moved the main User API to the
  ``fifi::math`` class (see the documentation for a complete description).
  The implementation details for specific finite fields and their available
  accelerations are moved to the appropriate subfolders to reduce clutter.
  The restructuring also yields some performance gains due to the unrolling
  of important functions and the addition of the new optimization flags.
* Major: Added the bourne dependency (version 7).

29.3.0
------
* Minor: Added sphinx documentation.

29.2.1
------
* Patch: Fixed compatibility with Visual Studio 2017 by using the
  immintrin.h header file (which is not included implicitly).

29.2.0
------
* Minor: Added pre-calculated multiplication and division tables for the
  binary4 and binary8 fields to allow fast initialization.
* Minor: Added some basic examples.

29.1.0
------
* Patch: Add virtual destructor to ``api::field_interface``.
* Minor: Updated the license to STEINWURF EVALUATION LICENSE 1.0.

29.0.0
------
* Major: Upgrade to storage 4.

28.0.0
------
* Major: Constants are automatically packed in the region arithmetic operations,
  therefore the user does not need to provide packed values.
* Major: Removed the obsolete ``min_alignment()`` and ``best_alignment()``
  functions and the ``region_alignment_info`` helper.
* Minor: Added a new API in ``src/fifi/api`` that wraps the existing field
  implementations with a common dynamic interface. This allows the user to
  select a field at runtime.
* Minor: Changed the arithmetic benchmark to use the dynamic field interface.

27.0.0
------
* Major: Upgrade to allocate 3

26.0.1
------
* Patch: Fixed double build issue due to the fifi_no_dispatch target.

26.0.0
------
* Major: Upgrade to waf-tools 4
* Major: Upgrade to allocate 2
* Major: Upgrade to cpuid 5
* Major: Upgrade to storage 3
* Minor: Upgrade to gauge 11
* Minor: Upgrade to stub 6
* Minor: Upgrade to gtest 4

25.0.1
------
* Patch: Fixed warnings in ``prime2325_prefix_length``.

25.0.0
------
* Major: Depend on allocate 1.
* Major: Removed dependency on sak.
* Minor: Added ``ceil_division`` from sak.
* Minor: Added ``easy_bind`` and ``optional_bind`` from sak.

24.0.0
------
* Major: Upgrade to storage 2

23.0.0
------
* Major: Upgrade to storage 1

22.0.0
------
* Major: Changed the region arithmetic API to operate on uint8_t* buffers
  instead of value_type* buffers. The size of these buffers is specified in
  bytes (previously the buffer length was measured in value_type elements).
  This change should make it easier to use raw buffers without any casts.
* Major: The buffer granularity is now measured in bytes (instead of
  value_type elements).
* Major: The ``get_value``, ``set_value``, ``set_values`` and ``swap_values``
  in fifi_utils.hpp also operate on uint8_t* buffers (instead of value_type*
  buffers).

21.0.0
------
* Major: Upgrade to waf-tools 3
* Major: Upgrade to cpuid 4
* Major: Upgrade to sak 15
* Minor: Upgrade to gauge 10
* Minor: Upgrade to stub 4
* Minor: Upgrade to gtest 3

20.1.0
------
* Patch: XCode 6.3+ supports the ``vtbl2_u8`` intrinsic on ARM64 and the
  ``vtbl1q_u8`` intrinsic is no longer available. Therefore the ARM64-specific
  code was removed and ``vtbl2_u8`` is used for all ARM targets.
* Patch: Replaced ``typedef`` with ``using``-statements.
* Minor: Added buildbot.py for coverage report settings.
* Patch: Changed all global inline template functions to static functions.
  The msvc linker incorrectly collapsed template functions that were
  instantiated with different types in multiple translation units. This led
  to unexpected behavior in the unit tests. The msvc linker cannot collapse
  static template functions, which are local to the translation units.

20.0.0
------
* Minor: Added free functions for invoking all region arithmetic operations.
  The corresponding type traits (e.g. ``has_region_add`` for ``region_add``)
  can be used to check if the underlying type provides the operation.
* Major: Added the block arithmetics API to calculate linear combinations
  in a single operation. This approach provides substantial performance
  improvements for some applications. The optimized ``vector_dot_product``
  function is available for the binary8 field with SSSE3 and AVX2
  intrinsics. For other cases, the ``vector_dot_product`` function defined in
  ``src/fifi/block_arithmetic.hpp`` will perform the computation using a
  standard loop.
* Patch: Use the more compact gauge macros in the arithmetic benchmarks.
* Minor: Modified options in the arithmetic benchmarks to correspond to the
  configuration values.

19.0.0
------
* Minor: Added NEON intrinsics for the binary field to speed up arithmetics
  on ARM CPUs (see the ``neon_binary`` class).
* Major: The ``simd_simple_online`` stack is now the default implementation
  for the binary field on ARM CPUs. The compiler-optimized ``simple_online``
  stack remains to be the default implementation on other CPUs.

18.1.0
------
* Minor: Added support for 64-bit ARM targets. The ``vtbl2_u8`` intrinsic is
  not available on ARM64, so it was replaced with the more efficient
  ``vtbl1q_u8`` intrinsic.

18.0.0
------
* Minor: Added ``set_values`` function, which makes it easier to initialize
  a memory buffer with finite field elements. The function was added to
  fifi_utils.hpp.
* Major: Remove `fifi_includes` waf target.
* Patch: Fix use flags in all waf targets.

17.1.0
------
* Minor: Update local dependency gauge to version 9.
* Patch: Fix version define.

17.0.0
------
* Minor: Added version define.
* Major: Update sak to version 14
* Major: Update gauge to version 8

16.0.0
------
* Major: Modified the ``simd_region_dispatchers`` layer to handle value_types
  that are incompatible with the field_type of the currently compiled stack.
* Minor: Added the ``sse42_prime2325`` optimization class that provides SIMD
  acceleration for the 2^32-5 prime field using SSE4.2 intrinsics. The
  ``simd_optimal_prime`` stack uses this optimization class if the CPU
  supports the SSE4.2 instruction set.
* Major: The ``simd_optimal_prime`` stack is now the default implementation
  for the prime2325 field.

15.0.0
------
* Major: Update sak to version 13
* Major: Update stub to version 2
* Patch: Added missing static assert for simple_online algorithm

14.1.0
------
* Minor: Added support for Emscripten compiler.

14.0.0
------
* Major: The default alignment and granularity properties of the region
  arithmetic operations were renamed to ``min_alignment`` and
  ``min_granularity``. In addition, the finite field stacks expose the
  ``best_alignment`` and ``best_granularity`` properties. Applications can
  achieve optimal performance by allocating memory buffers according to these
  requirements.
* Major: Replaced the templated-based dispatch layers with a single dispatch
  layer called ``simd_region_dispatchers``. This layer can instantiate several
  pre-defined SIMD optimization classes if the CPU supports the corresponding
  instruction set. This solution avoids the execution of potentially
  unsupported/illegal instructions in the constructors of the SIMD optimization
  classes if the application is running on an older CPU.
* Major: Upgrade to tables 5

13.0.0
------
* Major: Updated to sak version 12.x.y

12.0.0
------
* Major: Updated the way optimization dispatching is done to only
  happen once at run-time.
* Major: Updated to sak version 11.x.y
* Minor: Added NEON intrinsics for binary4 and binary8 to speed up arithmetics
  on recent ARM CPUs.
* Minor: Added AVX2 intrinsics for binary4 and binary8.
* Minor: Update to waf 1.8.0-pre1
* Minor: Made python files comply with pep8

11.0.0
------
* Major: Changed Fifi into a static library. Essentially this change
  was motivated by the desire to introduce SIMD accelerated arithmetics,
  but keep the library as easy to use as possible. SIMD introduces a number
  of CPU-dependent algorithms, this increases both compile-time and run-time
  complexity. These complexities can be handled internally in Fifi by using
  a static library, which means that they are hidden from its users.
* Major: Redid the Fifi architecture to use mix-in layers. This design
  technique allows a highly flexible design where most functionality
  can be customized to specific needs.
* Major: Moved optimizations from ``arithmetics.hpp`` to separate layers.
* Major: Split ``field_types.hpp`` into separate files for each field.
* Minor: Added unit tests for all classes.
* Minor: Introduced the binary4 finite field, i.e. GF(2^4).
* Minor: Introduced SIMD accelerated full table arithmetics for binary4 and
  binary8 (using SSSE3 intrinsics).

10.0.0
------
* Major: Updated to gauge version 7.x.y
* Minor: Updated to waf 1.7.10

9.1.0
-----
* Minor: Updated to waf-tools version 2.x.y
* Bug: Include ``<cstdlib>`` instead of ``<cmath>`` to use ``std::abs(int)``

9.0.0
-----
* Updated/added new functions to ``fifi_utils.hpp`` for converting between
  lengths, sizes and elements.
* Bumped sak to version 10
* Added project generator tool which allows Visual Studio solutions to be
  generated.

8.0.0
-----
* Updating waf to version 1.7.9 and start using the ``wurf_install_path`` tool.
* Rename ``default_field_impl.hpp`` to ``default_field.hpp``

7.0.0
-----
* Updating to use new waf-tools

6.0.0
-----
* Bump sak to version 7

5.0.0
-----
* Upgrade to sak version 6.x.y

4.0.1
-----
* Removing benchmarks from the automatic unit test runner.

4.0.0
-----
* Using ``.hpp`` extension instead of ``.h``

3.0.0
-----
* Switched c++11 compiler flag on per default
* Adding optimal prime field
* Moving benchmarks to the gauge tool
* Moved fifi headers to the sub-directory src
* Upgraded to waf 1.7.2 and using waf unit tests tool (for automatically
  running unit tests during a build).
* Switched to sak version 4.x.y

2.0.0
-----
* Updated the Waf build system to handle dependencies better. The new tools
  used are not compatible with the old versions so we have to bump the major
  version.

1.0.2
-----
* Added benchmarks
* Added dependency on sak providing timer functionality
  used in benchmarks
* Bumped dependency version for gtest to 1.0.3-gtest_1_6_0
* Bumped dependency version for boost to 1.1.2-boost_1_48_0
* Bumped dependency version for sak to 1.1.1
* Fixed win32 link and cxx flags
