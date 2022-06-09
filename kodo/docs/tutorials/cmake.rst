Using CMake
-----------

Kodo ships with support for the CMake buildsystem. This section describes
different ways you can use Kodo with CMake.

To demonstrate the CMake support we will build the following small example:

``main.cpp``:

.. literalinclude:: ../../examples/cmake_path/main.cpp
    :language: c++

Using existing Kodo checkout
............................

In this example we show how to use an already cloned version of Kodo. We assume
the clone is located at relative path from our current folder
``../../../kodo``.

In this case we can use the following ``CMakeLists.txt`` file to build and link
with the Kodo library:

.. literalinclude:: ../../examples/cmake_path/CMakeLists.txt


We can now build using CMake from the command-line by invoking::

    mkdir build
    cd build
    cmake ..
    cmake --build .


Automatically fetch the Kodo source code
........................................

Using CMake's ``FetchContent`` we can also automatically download and use
the Kodo library. The following ``CMakeLists.txt`` shows how this is achieved:

.. literalinclude:: ../../examples/cmake_fetchcontent/CMakeLists.txt

Remember to update the ``GIT_TAG`` to specify the version of Kodo you need.

Again we simply build using (notice how Kodo is now automatically cloned and
made available)::

    mkdir build
    cd build
    cmake ..
    cmake --build .


Cross-compilation using CMake
.............................

Cross compiling Kodo with CMake can be achieved using CMake's toolchain files.

Example: Raspberry Pi
,,,,,,,,,,,,,,,,,,,,,,

We may build a Raspberry Pi 3/4 compatible binary using the following steps.

1. Fetch a suitable `cross compilation toolchain
   <https://github.com/abhiTronix/raspberry-pi-cross-compilers>`_ we'll use
   ``cross-gcc-10.3.0-pi_3+.tar.gz``.

   You can check which Target OS you have on your Raspberry Pi by running (on
   the device)::

       lsb_release --all

2. Create a CMake toolchain file to use our cross compiler. Here we've extracted
   the toolchain in `/tmp` so our CMake toolchain file contains the following.

   ``arm-linux-gnueabihf-toolchain.cmake``::

        set(CMAKE_SYSTEM_NAME Linux)
        set(CMAKE_SYSTEM_PROCESSOR "arm")
        set(CMAKE_C_COMPILER   /tmp/cross-gcc-10.3.0-pi_3+/cross-pi-gcc-10.3.0-2/bin/arm-linux-gnueabihf-gcc)
        set(CMAKE_CXX_COMPILER /tmp/cross-gcc-10.3.0-pi_3+/cross-pi-gcc-10.3.0-2/bin/arm-linux-gnueabihf-g++)
        SET(CMAKE_FIND_ROOT_PATH  /tmp/cross-gcc-10.3.0-pi_3+/cross-pi-gcc-10.3.0-2/arm-linux-gnueabihf)

        set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
        set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
        set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
        set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

3. Use configure and compiler::

    mkdir build
    cd build
    cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm-linux-gnueabihf-toolchain.cmake
    cmake --build .