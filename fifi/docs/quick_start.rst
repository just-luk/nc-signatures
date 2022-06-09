Quick Start
===========

If you already installed a C++14 compiler, git and python on your system,
then you can clone this repository to a suitable folder::

    git clone git@github.com:steinwurf/fifi.git

After cloning the repository you will have the latest development snapshot.
Unless you need to work on the latest and greatest we recommend that you
switch to a released version of the library.

.. parsed-literal::

  cd fifi
  git checkout |release|

Configure and build the project::

    python waf configure
    python waf build
    python waf install --destdir=fifi_install

The final ``install`` step will create a folder containing all the nessecary
files needed to use the library (e.g. static library, headers etc.). You
can change the output folder by passing a different path to ``--destdir``.
You can also copy/move this install folder anywhere you like.

Example
.......

To create your first "hello world" program using fifi you can try the following:

.. literalinclude:: ../examples/readme/readme.cpp
   :language: c++
   :lines: 6-

To compile with g++ (if you installed to the ``fifi_install`` local folder)::

    g++ main.cpp -std=c++14 -I fifi_install/include -L fifi_install/lib -lfifi

See the full API provided by the :ref:`fifi::math` object to see all
available operations.

