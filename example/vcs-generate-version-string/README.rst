Generate a Version String from VCS
==================================

This example shows how to generate a version string directly from the
VCS system used to check out Boost.Build using the ``vcs`` module.

This Boost.Build project generates a program to print out the version
string of the current working directory.  It should be run with
``--verbose-test`` so the result is easy to see.

.. code::

   b2 --verbose-test

Note that the ``version_string.cpp`` file can be found in the ``bin``
directory tree.
