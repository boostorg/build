#!/usr/bin/python

# Test that unused sources are at least reported.

from BoostBuild import Tester
from string import find
t = Tester()

t.set_tree("unused")

t.run_build_system()
# The second invocation should do nothing, and produce
# no warning.
t.run_build_system()
t.fail_test(t.stdout() != '')

t.run_build_system("-sGENERATE_ONLY_UNUSABLE=1")
t.fail_test(find(t.stdout(), "warning: Unused source b in main target ./a") == -1)

t.cleanup()

