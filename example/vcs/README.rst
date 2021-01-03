Boost.Build vcs Module Demonstration
====================================

This Boost.Build project provides a demonstration of the functionality
available from the ``vcs`` module.

It first prints the properties of the VCS system used to maintain this
repository, then fetches and checks out the Boost.Build source code
form GitHub using both Git and Subversion and prints the properties of
each of those repositories.

This will create the following directory tree by checking out the
Boost.Build source code at the indicated versions.

::

   repositories
   repositories/git
   repositories/gitboost-build-boost-1.55.0
   repositories/gitboost-build-master
   repositories/gitboost-build-origin-master
   repositories/gitboost-build-refs-heads-master
   repositories/gitboost-build-refs-tags-boost-1.55.0
   repositories/gitboost-build-tags-boost-1.55.0
   repositories/svn
   repositories/svn/boost-build-boost-tags-1.55.0
   repositories/svn/boost-build-trunk
