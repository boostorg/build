#!/usr/bin/python

from BoostBuild import Tester, List

t = Tester()

t.set_tree("dependency-test")
t.run_build_system()
# Do not bother checking for created files now.


t.cleanup()
