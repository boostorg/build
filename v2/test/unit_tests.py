#!/usr/bin/python

from BoostBuild import Tester

t = Tester()
t.run_build_system(extra_args="--debug --build-system=test")

t.cleanup()