#!/usr/bin/python

from BoostBuild import Tester
import os

t = Tester()

# This test does no modifications, so run in in the invocation dir

os.chdir(t.original_workdir)

expected_output="""Projects structure dump

    Location: project-test1
    Project id: /boost-build-test-project-1

        Project root: project-test1
        Parent project: project-test1
        Requirements: <threading>multi <include>/home/ghost/local/include
        Default build: debug
        Source location: project-test1
        Subincludes: dir dir2

    Location: project-test1/dir
    Project id: /boost-build-test-project-1/dir

        Project root: project-test1
        Parent project: project-test1
        Requirements: <threading>multi <include>/home/ghost/local/include
        Default build: release
        Source location: project-test1/dir/src
        Subincludes:

    Location: project-test1/dir2
    Project id: /cool-library

        Project root: project-test1/dir2
        Parent project: project-test1/dir2
        Requirements: <include>/home/ghost/build/boost-cvs
        Default build: debug
        Source location: project-test1/dir2
        Subincludes:

"""

t.run_build_system("-sBOOST_BUILD_PATH=../new -fproject-test1.jam", stdout=expected_output)
t.pass_test()
