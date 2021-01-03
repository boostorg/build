#!/usr/bin/python

# Test the 'vcs' example.

import BoostBuild

t = BoostBuild.Tester(use_test_config=False)

t.set_tree("../example/vcs")

t.run_build_system()

#t.expect_output_lines("----------")

t.cleanup()
