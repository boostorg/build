#!/usr/bin/python

# Copyright 2002-2005 Dave Abrahams.
# Copyright 2002-2006 Vladimir Prus.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#         http://www.boost.org/LICENSE_1_0.txt)

import os, sys, string
import BoostBuild
from BoostBuild import get_toolset

# clear environment for testing
#
for s in (
    'BOOST_ROOT','BOOST_BUILD_PATH','JAM_TOOLSET','BCCROOT',
    'MSVCDir','MSVC','MSVCNT','MINGW','watcom'
    ):

    try:
        del os.environ[s]
    except:
        pass

BoostBuild.set_defer_annotations(1)

def run_tests(critical_tests, other_tests):
    """Runs first critical tests and then other_tests.

       Stops on first error, and write the name of failed test to
       test_results.txt. Critical tests are run in the specified order,
       other tests are run starting with the one that failed the last time.
    """
    last_failed = last_failed_test()
    other_tests = reorder_tests(other_tests, last_failed)
    all_tests = critical_tests + other_tests

    invocation_dir = os.getcwd()

    pass_count = 0
    failures_count = 0
    for i in all_tests:
        print ("%-25s : " %(i)),
        try:
            __import__(i)
        except SystemExit:
            print "FAILED"
            if failures_count == 0:
                f = open(os.path.join(invocation_dir, 'test_results.txt'), 'w')
                f.write(i)
                f.close()
            failures_count = failures_count + 1
            # Restore the current directory, which might be changed by the
            # test
            os.chdir(invocation_dir)
            BoostBuild.flush_annotations();
            continue
        print "PASSED"
        BoostBuild.flush_annotations();
        pass_count = pass_count + 1
        sys.stdout.flush()  # makes testing under emacs more entertaining.

    # Erase the file on success
    if failures_count == 0:
        open('test_results.txt', 'w')

    print """
    === Test summary ===
    PASS: %d
    FAIL: %d
    """ % (pass_count, failures_count)


def last_failed_test():
    "Returns the name of last failed test or None"
    try:
        f = open("test_results.txt")
        s = string.strip(f.read())
        return s
    except:
        return None

def reorder_tests(tests, first_test):
    try:
        n = tests.index(first_test)
        return [first_test] + tests[:n] + tests[n+1:]
    except ValueError:
        return tests


critical_tests = ["unit_tests", "module_actions", "startup_v1", "startup_v2"]

critical_tests += ["core_d12", "core_typecheck", "core_delete_module",
                   "core_varnames", "core_import_module"]

tests = [ "rebuilds",
          "timedata",
          "project_test3",
          "project_test4",
          "generators_test",
          "dependency_test",
          "path_features",
          "relative_sources",
          "no_type",
          "chain",
          "default_build",
          "default_toolset",
          "use_requirements",
          "conditionals",
          "conditionals2",
          "conditionals3",
          "conditionals_multiple",
          "configuration",
          "indirect_conditional",
          "stage",
          "prebuilt",
          "project_dependencies",
          "build_dir",
          "searched_lib",
          "make_rule",
          "alias",
          "alternatives",
          "default_features",
          "print",
          "ndebug",
          "explicit",
          "absolute_sources",
          "dependency_property",
          "custom_generator",
          "bad_dirname",
          "c_file",
          "inline",
          "property_expansion",
          "loop",
          "tag",
          "suffix",
          "inherit_toolset",
          "skipping",
          "project_glob",
          "project_root_constants",
          "double_loading",
          "dll_path",
          "regression",
          "composite",
          "library_chain",
          "unit_test",
          "standalone",
          "expansion",
          "wrapper",
          "duplicate",
          "library_property",
          "load_order",
          "wrong_project",
          "using",
          "source_locations",
          "out_of_tree",
          "notfile",
          "project_root_rule",
          "resolution",
          "build_file",
          "build_no",
          "disambiguation",
          "clean",
          "lib_source_property",
          "implicit_dependency",
          "example_libraries",
          "example_make",
          "remove_requirement",
          "free_features_request",
          "file_name_handling",
          "sort_rule",
          "ordered_include",
          "test_result_dumping",
          "inherited_dependency"
          ]

if os.name == 'posix':
    tests.append("symlink")
    # On windows, library order is not important, so skip this test
    # Besides, it fails ;-)
    # Further, the test relies on the fact that on Linux, one
    # can build a shared library with unresolved symbols. This is
    # not true on Windows (even with cygwin gcc).
    if string.find(os.uname()[0], "CYGWIN") == -1:
        tests.append("library_order")

if string.find(get_toolset(), 'gcc') == 0:
    tests.append("gcc_runtime")

if string.find(get_toolset(), 'gcc') == 0 or string.find(get_toolset(), 'msvc') == 0:
    tests.append("pch")

if "--extras" in sys.argv:
    tests.append("boostbook")
    tests.append("qt4")
    tests.append("example_qt4")
    # Requires ./whatever.py to work, so is
    # not guaranted to work everywhere.
    tests.append("example_customization")
    # Requires gettext tools.
    tests.append("example_gettext")

else:
    print 'Note: skipping extra tests'

run_tests(critical_tests, tests)
