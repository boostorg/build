#!/usr/bin/python

# Test the 'vcs-generate-version-string' example.

import BoostBuild

t = BoostBuild.Tester(use_test_config=False)

t.set_tree("../example/vcs-generate-version-string")

t.run_build_system()

#t.expect_output_lines("2016.03")

t.cleanup()
