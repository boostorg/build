#!/usr/bin/python

from BoostBuild import Tester, List
import os
from string import strip

t = Tester()

t.set_tree('module-actions')

# We expect t5 and t7's output to be dumped to stdout
t.run_build_system(
    stdout =
r'''A.act t1:   X1-t1     
B.act t1:   X1-t1   X2-B   
act t1:   X1-t1   X2-global   X3-global 
A.act t2:   X1-A   X2-t2   
B.act t2:     X2-t2   
act t2:   X1-global   X2-t2   X3-global 
A.act t3:   X1-A     X3-t3 
B.act t3:     X2-B   X3-t3 
act t3:   X1-global   X2-global   X3-t3 
'''
)

t.expect_nothing_more()
t.cleanup()
print 'tesing complete'
