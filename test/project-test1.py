#!/usr/bin/python

from BoostBuild import Tester
import os

t = Tester()

# This test does no modifications, so run in in the invocation dir

os.chdir(t.original_workdir)

expected_output="""Project Roots:
 
'project-test1/dir2':
 
  Module for project-root is 'project-root<project-test1/dir2>'
 
Projects:
 
'/cool-library':
 
* Project root: project-test1/dir2
* Parent project: (none)
* Requirements: <include>/home/ghost/build/boost-cvs
* Default build: debug
* Source location: project-test1/dir2
* Subincludes:
 
'project-test1':
 
  Module for project-root is 'project-root<project-test1>'
 
Projects:
 
'/boost-build-test-project-1/dir':
 
* Project root: project-test1
* Parent project: project-test1
* Requirements: <threading>multi <include>/home/ghost/local/include
* Default build: release
* Source location: project-test1/dir/src
* Subincludes:
 
'/boost-build-test-project-1':
 
* Project root: project-test1
* Parent project: (none)
* Requirements: <threading>multi <include>/home/ghost/local/include
* Default build: debug
* Source location: project-test1
* Subincludes: dir dir2
 
"""

t.run_build_system("--build-system=project-test1", stdout=expected_output)
t.pass_test()
