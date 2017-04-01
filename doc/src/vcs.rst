Boost.Build ``vcs`` Module
==========================

.. contents::

Overview
--------

The Boost.Build ``vcs`` module exposes a limited subset of version
control system functionality to Boost.Build projects for a set of
supported version control system back ends.  Currently, Boost.Build
``vcs`` supports Subversion and Git.  Other systems should be
straightforward to implement.

Usage
-----

An example Boost.Build project illustrating the vcs interface is shown
below.

.. code::

   import vcs ;

   import assert ;

   # print the type of version control system and the generated
   # version string for this project
   echo [ vcs.type . ]
   echo [ vcs.generate-version-string . ] ;

   # fetch and checkout the 1.0 reference of a project kept in the Git
   # version control system
   vcs.get git : https://example.com/git/path/to/project/root : /path/to/desired/root ;
   vcs.checkout : /path/to/desired/root : 1.0 ;

   # verify that the URL and reference matches the desired
   assert.equal [ vcs.root-url /path/to/desired/root ] : https://example.com/git/path/to/desired/root ;
   assert.equal [ vcs.ref /path/to/desired/root ] : [ vcs.ref /path/to/desired/root : 1.0 ] ;

Also, see the `example <../../example/vcs>`_ for an exhaustive example.

The example below shows how to use this to create a
``version_string.cpp`` file containing the version string.  The
``print`` module provides a mechanism to ensure that the generated
file is only modified when the version string actually changes.

.. include:: ../../example/vcs-generate-version-string/jamroot.jam
   :code:

.. include:: ../../example/vcs-generate-version-string/main.cpp
   :code:

Also, see the `example <../../example/vcs-generate-version-string>`_ for
an complete example.

Reference
---------

``type ( directory )``

   Returns the type of version control system for the indicated
   directory, or the empty string if none was detected.

``generate-version-string ( directory )``

   Returns a string uniquely describing the state of the repository at
   the given directory.

   - When on a tag, all version control systems will return the tag
     name

   - Otherwise

      - Git: <nearest-tag-name>-<branch-name>-<commits-since-nearest-tag>-g<commit-id>

      - Subversion: -<URL>--s<REV>

   The ``generate-version-string`` rule can be used to generate a version
   string for a program dynamically.

``fetch ( vcs : root-url : directory )``

   Fetches from the URL to the root of the vcs project to the
   indicated directory using vcs.

``checkout ( directory : symbolic-ref )``

   Checks out the indicated symbolic reference from the repository
   located at the indicated directory.

``root-url ( directory )``

   Returns the URL to the root of the vcs project located at the
   indicated directory.

``ref ( directory : symbolic-ref ? )``

   Returns a unique identifier representing the current state of the
   vcs project located at directory.  If the symbolic reference is
   given, the rule returns the reference of that symbolic reference,
   not the current state of the project.

Backends Reference
------------------

``generate-version-string ( directory )``

   Returns the version string as defined for the backend.  Note that
   each backend is required to return the exact tag name if the
   directory is on a tag.  Otherwise, the format is free-form, but it
   is recommended that it be as close to the Git format for ``git
   describe`` as possible for maximum information.

``fetch ( root-url : directory )``

   Fetches the from the URL to the root of the vcs project to the
   indicated directory using the backend.

``checkout ( directory : symbolic-ref )``

   Checks out the indicated symbolic reference from the repository
   located at the indicated directory.

``root-url ( directory )``

   Returns the URL to the root of the vcs project located at the
   indicated directory.

``ref ( directory : symbolic-ref ? )``

   Returns a unique identifier representing the current state of the
   vcs project located at directory.  If the symbolic reference is
   given, the rule returns the reference of that symbolic reference,
   not the current state of the project.

``is-repository ( directory )``

   Returns true if the directory is controlled by the backend version
   control system.  This can be as complex or as simple as required.

``executable-exists ( )``

   Returns true if the executable required to support the backend
   exists on the system.

Design
------

The Boost.Build ``vcs`` module depends on separate backends to
implement the interface.  The backend file should be named
``vcs-BACKEND.jam`` where BACKEND is the name of the backend and
should contain implementations for each of the functions defined
below.

Currently, there are two supported backends:

- Git
- Subversion

Note that the only rule that requires that that the type of version
control system is specified is the ``fetch`` rule.  The rest detect
the version control system from querying the given directory.

Implementation
--------------

Hopefully, knowing the implementation will not be required to use this
module, but they are included here for reference.

``vcs`` Interface
~~~~~~~~~~~~~~~~~

- `vcs <../../src/tools/vcs.jam>`_

Backends
~~~~~~~~

- `vcs-git <../../src/tools/vcs-git.jam>`_
- `vcs-svn <../../src/tools/vcs-svn.jam>`_
