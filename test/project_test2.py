#!/usr/bin/python

from BoostBuild import Tester
import os
from string import strip

t = Tester("--build-system=project-test2")
t.set_tree("project-test2")


t.run_build_system()
print t.stdout()

# Note that without this line the test would be brittle: it would silently
# pass if those files were accicendally present in the original 'project-test2'
# dir.
t.expect_addition("bin/foo1")
t.expect_addition("bin/foo2")
t.expect_addition("dir/bin/foo1")

if os.name == 'nt':
    props = '" <variant>debug <optimization>off <rtti>on  "'
else:
    props = ' <variant>debug <optimization>off <rtti>on  \n'


t.fail_test(strip(t.read("bin/foo1")) != strip(props))
t.fail_test(strip(t.read("bin/foo2")) != strip(props))   

# Now check that incompatible requirements for subproject are checked.

t.copy("dir/Jamfile2", "dir/Jamfile")

expected="""Requirements for project at 'dir' conflict with parent's.
Explanation:  link-incompatible properties <threading>multi and
<threading>single

"""
t.run_build_system(stdout=expected, status=None)


# Check incompatible requirements on main target
t.copy("dir/Jamfile3", "dir/Jamfile")

expected="""Cannot satisfy request to build dir/foo1 with properties  <variant>debug
<optimization>off <rtti>on
Explanation: link-incompatible properties <rtti>on and <rtti>off

"""

t.run_build_system(stdout=expected, status=None)

t.cleanup()

