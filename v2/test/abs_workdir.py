# Niklaus Giger, 2005-03-15
# Testing whether we may run a test in a absolute directories
# There are no tests for temporary directories as this is implictly tested in a lot of other cases

from BoostBuild import Tester
import os, string

t = Tester(
      executable="jam"
    , workdir = os.getcwd()
    , pass_toolset=0
    )

jamfile="""
actions print_pwd { pwd ; }
print_pwd pwd ;
Always pwd ;
"""

t.write("Jamfile", jamfile)
t.write("project-root.jam", " ")

t.run_build_system(status=0, extra_args = "pwd")

if 'TMP' in os.environ:
  tmp_dir =os.environ.get('TMP')
else:
  tmp_dir ="/tmp"

if string.rfind(t.stdout(), tmp_dir) != -1:
    t.fail_test(1)

if string.rfind(t.stdout(), 'build/v2/test') == -1:
    t.fail_test(1)

t.run_build_system(status=1, extra_args = "pwd", subdir ="/must/fail/with/absolute/path",
                   stderr=None)
t.cleanup

