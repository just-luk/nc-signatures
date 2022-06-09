News for kodo
=============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* tbd

13.1.0
------
* Minor: Added on_symbol_pivot() callback-setter for kodo::slide::decoder.

13.0.0
------
* Major: Added ranbo dependency. Ranbo is now used for all random number
  generation in kodo. This means seeds from previous versions of kodo are
  incompatible with the current version.
* Major: Removed boost dependency.

12.0.1
------
* Patch: `kodo::slide::reset_callback`'s index is now an `uint64_t`.

12.0.0
------
* Major: Removed kodo::slide::encoder::window_symbol_bytes. The encoder now
  expects a buffer of size max_symbol_bytes. The number of bytes written
  then returned by kodo::slide::encoder::encode_symbol and
  kodo::slide::encoder::encode_systematic_symbol.
* Minor: kodo:slide::encoder and kodo::slide::decoder reset now takes an
  optional callback function which is to be called for each claimed symbol.
* Minor: Added benchmark for kodo::slide encoder and decoder.

11.0.2
------
* Patch: Fix bug in kodo::slide decoder where a backwards substitution were not
  performed when receiving a systematic symbol. This would be problematic in
  scenarios with packet reordering.

11.0.1
------
* Patch: Fixed bug in kodo::slide::decoder where the "top part" of systematic
  symbols were not properly zeroed, causing decoding failure.

11.0.0
------
* Major: Removed stream_symbol_bytes function from kodo::slide. Instead use
  window_symbol_bytes.
* Minor: Changed index types to uint64_t to void issues when running on 32 bit
  machines

10.1.0
------
* Minor: Expose `is_symbol_pivot` in `kodo::slide::decoder`.

10.0.0
------
* Major: Renamed `coefficients_at` -> `coefficients_data`.
* Major: Renamed `symbol_at` -> `symbol_data`.
* Minor: Added on_symbol_decoded callback support for block and fulcrum code.
* Major: Moved `kodo::slide::symbol_decoded_callback` function definition to
  `kodo::symbol_decoded_callback`.
* Minor: Renamed `kodo::detail::bit_count::count` ->
         `kodo::detail::bit_count::value`
* Minor: Renamed `kodo::detail::symbol_count::count` ->
         `kodo::detail::symbol_count::value`
* Minor: Added `kodo::slide::range`.
* Major: Rename `push_front` to `push` and conversely `pop_back` to `pop` on
  `kodo::slide::stream`.
* Major: Rename `push_front_symbol` to `push_symbol` and conversely
  `pop_back_symbol` to `pop_symbol` on `kodo::slide::encoder` and
  `kodo::slide::decoder`.
* Major: Removed `set_window` on `kodo::slide::encoder`.
* Major: Removed `set_window` on `kodo::slide::decoder`.

9.1.0
-----
* Minor: Removed lower limit for block bytes in perpetual encoder.
* Minor: Added examples that show how to set up a UDP sender/receiver using an encoder/decoder.

9.0.0
-----
* Major: Upgrade to fifi version 35.

8.1.0
-----
* Patch: Fixed kodo::perpetual logging so that calling log function no longer
  causes an assert.
* Minor: Moved kodo::perpetual mapping threshold functionality to separate
  layer.
* Patch: Fixed issue with kodo::perpetual where using any width but _32, would
  cause the decoding to fail.
* Minor: Changed how the codec stacks are wrapped.
* Minor: Updated waf.

8.0.0
-----
* Major: Removed save restore functionality of `kodo::perpetual`.
* Major: Re-wrote `kodo::perpetual`.
* Minor: Added example which shows how different codecs and configurations
  affect the linear dependency.
* Minor: Added kodo::fulcrum benchmarks.
* Minor: Benchmarks use Google Benchmark instead of Gauge.
* Minor: Added kodo::block benchmarks.
* Minor: Added `kodo::fulcrum` API.
* Minor: Added `set_log_name` and `log_name`.
* Minor: Added `kodo::block::decoder::coefficients_at`.
* Major: Changed `log_callback` to now also include a log name.
* Major: Changed `kodo::block::decoder::symbol_at` to now return a
  `const uint8_t*`, instead of `uint8_t*`.
* Patch: `encode_symbol` and `encode_systematic_symbol` functions are now marked
  `const` as they should be.

7.0.1
-----
* Patch: Fixed bug caused by not zeroing excess symbol data when working with
  systematic symbols in sliding window.

7.0.0
-----
* Minor: Added tunable block generator.
* Minor: Added tutorial.
* Major: Removed `set_window` on `kodo::slide::generator::uniform_random`.
* Major: Changed `generate` and `coefficients_bytes` on
  `kodo::slide::generator::uniform_random`. They now require two additional
  arguments, `window_lower_bound` and `window_symbols`.
* Minor: Added `kodo::slide::decoder::symbol_at` which will return the data
  pointer to the data stored at the given index.

6.0.1
-----
* Patch: Fix assert so that kodo::slide::encoder can now produce systematic
  symbols outside the window (but inside the stream).

6.0.0
-----
* Minor: Moved `kodo-slide` to `kodo::slide`.
* Minor: Moved perpetual details to a perpetual details folder.
* Minor: Moved block details to a block details folder.
* Minor: Added `is_symbol_set` to the `block::encoder`.
* Minor: The `block::encoder` now supports setting symbols out of order.
  Note this is not supported by the generators.
* Major: Added recycle dependency.
* Major: Renamed `generator_bytes` to `coefficients_bytes`.
* Major: Renamed `max_generator_bytes` to `max_coefficients_bytes`.
* Major: Removed `coefficients_bytes` from encoder and decoders,
  now it's only available for the generators.
* Major: `decode_systematic_symbol` now takes a const pointer to the symbol.
* Major: `bitset::storage_size` -> `bitset::storage_bytes`.

5.3.1
-----
* Patch: Fix issue where parity_2d generator expected a none zeroed buffer.

5.3.0
-----
* Minor: Added coefficients_bytes function to block encoder.

5.2.1
-----
* Patch: Fix possible infinite loop in
  kodo::block::generator::uniform_random::generate_recode.

5.2.0
-----
* Minor: Added `kodo::block::generator::uniform_random::generate_recode`.
* Minor: Added `kodo::block::decoder::recode_symbol`.

5.1.0
-----
* Minor: Update Evaluation License to version 1.3.

5.0.0
-----
* Major: Change cmake build to be object library based.

4.0.0
-----
* Major: Changed API of `parity_2d` to support disabling of row and/or column
  repair.

3.0.0
-----
* Major: Changed the name of the SMPTE-2022-1 FEC generator (`smpte_2022`) to `parity_2d`.

2.3.0
-----
* Minor: Added install step to CMake.

2.2.0
-----
* Minor: Added SMPTE-2022-1 FEC generator.

2.0.2
-----
* Patch: Remove unused lingering `max_block_bytes` function.

2.0.1
-----
* Patch: Fix issue with endian dependency.

2.0.0
-----
* Major: Upgrade to fifi version 32.
* Minor: Added Cauchy Reed Solomon Generator.

1.1.0
-----
* Patch: Fix nodebug warnings.
* Minor: Added block code.

1.0.0
-----
* Major: Initial release.
