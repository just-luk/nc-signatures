Quick Start
===========

If you already installed a C++14 compiler, git and python on your system,
then you can clone this repository to a suitable folder::

    git clone git@github.com:steinwurf/kodo.git

After cloning the repository you will have the latest development snapshot.
Unless you need to work on the latest and greatest we recommend that you
switch to a released version of the library.

.. parsed-literal::

  cd kodo
  git checkout tag

Replace ``tag`` with the specific version you want to use.

Build using Waf
---------------

Configure and build the project::

    python waf configure
    python waf build
    python waf install --destdir=kodo_install

The final ``install`` step will create a folder containing all the nessecary
files needed to use the library (e.g. static library, headers etc.). You
can change the output folder by passing a different path to ``--destdir``.
You can also copy/move this install folder anywhere you like.

Build using CMake
-----------------
As an alternative to Waf, we provide a minimal CMake build file. This is mainly
to allow integration with external tools and codebase.

.. note:: The provided CMake build file will not build unit-tests, nor examples.

To build and install the library using CMake, use the following commands::

    mkdir cmake_build
    cd cmake_build/
    cmake ..
    make
    make install DESTDIR=./kodo_install

Feel free to change the folder names as needed.

Use as Dependency in CMake
..........................

If you want to used Kodo as a dependency within an existing CMake project you
can find more information here: :ref:`tutorials/cmake:Using CMake`.
