
import TestCmd
from tree import build_tree, trees_difference
import copy
import fnmatch
import os
import shutil
import string
import types
import time
import tempfile
import sys

# Compute an executable suffix for tests to use
exe_suffix = ''
dll_suffix = '.so'
if os.environ.get('OS','').lower().startswith('windows'):
    exe_suffix = '.exe'
    dll_suffix = '.dll'
#
# FIXME: this is copy-pasted from TestSCons.py
# Should be moved to TestCmd.py?
#
if os.name == 'posix':
    def _failed(self, status = 0):
        if self.status is None:
            return None
        if os.WIFSIGNALED(status):
            return None
        return _status(self) != status
    def _status(self):
        if os.WIFEXITED(self.status):
            return os.WEXITSTATUS(self.status)
        else:
            return None
elif os.name == 'nt':
    def _failed(self, status = 0):
        return not self.status is None and self.status != status
    def _status(self):
        return self.status

class Tester(TestCmd.TestCmd):
    """Class for testing Boost.Build.

    Optional argument `executable` indicates the name of the
    executable to invoke. Set this to "jam" to test Boost.Build v1
    behavior.
    """
    def __init__(self, arguments="", executable = 'bjam', match =
                 TestCmd.match_exact, boost_build_path = None,
                 **keywords):

        self.original_workdir = os.getcwd()
        self.last_build_time = 0

        jam_build_dir = ""
        if os.name == 'nt':
            jam_build_dir = "bin.ntx86"
        elif os.name == 'posix':
            if os.uname()[0].lower().startswith('cygwin'):
                jam_build_dir = "bin.cygwinx86"
            else:
                jam_build_dir = "bin.linuxx86"
        else:
            raise "Don't know directory where jam is build for this system"

        if boost_build_path is None:
            boost_build_path = os.path.join(self.original_workdir,
                                            "..", "new")

        verbosity = ' -d0 --quiet '
        if '--verbose' in sys.argv:
            keywords['verbose'] = 1
            verbosity = ' -d+2 '

        TestCmd.TestCmd.__init__(
            self
            , program=os.path.join(
                '..', 'jam_src', jam_build_dir, executable)
              +  ' --ignore-config -sBOOST_BUILD_PATH=' + boost_build_path
              + verbosity
              + arguments
            , match=match
            , workdir=''
            , **keywords)

        os.chdir(self.workdir)

    def cleanup(self):
        try:
            TestCmd.TestCmd.cleanup(self)
            os.chdir(self.original_workdir)
        except AttributeError:
            # Whe this is called during by TestCmd.TestCmd.__del__ we can have both
            # 'TestCmd' and 'os' unavailable in our scope. Do nothing in this case.
            pass
            
    #
    # Methods that change working directory's content
    #
    def set_tree(self, tree_location):
        # Seem like it's not possible to remove a directory which is
        # current.
        d = os.getcwd()
        os.chdir(os.path.dirname(self.workdir))
        shutil.rmtree(self.workdir, ignore_errors=0)

        if not os.path.isabs(tree_location):
                tree_location = os.path.join(self.original_workdir, tree_location)
        shutil.copytree(tree_location, self.workdir)

        os.chdir(d)

        def make_writable(unused, dir, entries):
            for e in entries:
                name = os.path.join(dir, e)
                os.chmod(name, os.stat(name)[0] | 0222)

        os.path.walk(".", make_writable, None)


    def write(self, file, content):
        self.wait_for_time_change()
        nfile = self.native_file_name(file)
        try:
            os.makedirs(os.path.dirname(nfile))
        except Exception, e:
            pass
        open(nfile, "wb").write(content)

    def rename(self, old, new):
        try:
            os.makedirs(os.path.dirname(new))
        except:
            pass
        
        try:
            os.remove(new)
        except:
            pass
        
        os.rename(old, new)
        self.touch(new);

    def copy(self, src, dst):
        self.wait_for_time_change()
        self.write(dst, self.read(src))

    def touch(self, names):
        self.wait_for_time_change()
        if not type(names) == types.ListType:
                names = [names]
        for name in names:
                os.utime(self.native_file_name(name), None)

    def rm(self, names):
        self.wait_for_time_change()
        if not type(names) == types.ListType:
            names = [names]

	# Avoid attempts to remove current dir
	os.chdir(self.original_workdir)
        for name in names:
            n = self.native_file_name(name)
            if os.path.isdir(n):
                shutil.rmtree(n, ignore_errors=0)
            else:
                os.unlink(n)

        # Create working dir root again, in case
        # we've removed it
        if not os.path.exists(self.workdir):
            os.mkdir(self.workdir)
        os.chdir(self.workdir)
                                                        
    #
    #   FIXME: Large portion copied from TestSCons.py, should be moved?
    #
    def run_build_system(
        self, extra_args='', subdir='', stdout = None, stderr = '',
        status = 0, match = None, **kw):

        self.previous_tree = build_tree(self.workdir)

        if match is None:
            match = self.match

        try:
            kw['program'] = self.program + ' ' + extra_args
            kw['chdir'] = subdir
            apply(TestCmd.TestCmd.run, [self], kw)
        except:
            print "STDOUT ============"
            print self.stdout()
            print "STDERR ============"
            print self.stderr()
            raise

        if status != None and _failed(self, status):
            expect = ''
            if status != 0:
                expect = " (expected %d)" % status

            print '"%s %s" returned %d%s' % (
                self.program, extra_args, _status(self), expect)

            print "STDOUT ============"
            print self.stdout()
            print "STDERR ============"
            print self.stderr()
            self.fail_test(1)

        if not stdout is None and not match(self.stdout(), stdout):
            print "Expected STDOUT =========="
            print stdout
            print "Actual STDOUT ============"
            print self.stdout()
            stderr = self.stderr()
            if stderr:
                print "STDERR ==================="
                print stderr
            self.maybe_do_diff(self.stdout(), stdout)
            self.fail_test(1)

        if not stderr is None and not match(self.stderr(), stderr):
            print "STDOUT ==================="
            print self.stdout()
            print "Expected STDERR =========="
            print stderr
            print "Actual STDERR ============"
            print self.stderr()
            self.maybe_do_diff(self.stderr(), stderr)
            self.fail_test(1)

        self.tree = build_tree(self.workdir)
        self.difference = trees_difference(self.previous_tree, self.tree)
        self.difference.ignore_directories()
        self.unexpected_difference = copy.deepcopy(self.difference)

        self.last_build_time = time.time()

    def read(self, name):
        return open(self.native_file_name(name), "r").read()

    def read_and_strip(self, name):
        lines = open(self.native_file_name(name), "r").readlines()
        result = string.join(map(string.rstrip, lines), "\n")
        if lines and lines[-1][-1] == '\n':
            return result + '\n'
        else:
            return result
    
    def fail_test(self, condition, *args):
	# If test failed, print the difference
        if condition and hasattr(self, 'difference'):
            self.difference.pprint()
        TestCmd.TestCmd.fail_test(self, condition, *args)
        
    # A number of methods below check expectations with actual difference
    # between directory trees before and after build.
    # All the 'expect*' methods require exact names to be passed.
    # All the 'ignore*' methods allow wildcards.

    # All names can be lists, which are taken to be directory components
    def expect_addition(self, names):
        if type(names) == types.StringType:
                names = [names]
        for name in names:
                try:
                        self.unexpected_difference.added_files.remove(name)
                except:
                        print "File %s not added as expected" % (name,)
                        self.fail_test(1)

    def ignore_addition(self, wildcard):
        ignore_elements(self.unexpected_difference.added_files, wildcard)

    def expect_removal(self, names):
        if type(names) == types.StringType:
                names = [names]
        for name in names:
                try:
                        self.unexpected_difference.removed_files.remove(name)
                except:
                        print "File %s not removed as expected" % (name,)
                        self.fail_test(1)

    def ignore_removal(self, wildcard):
        ignore_elements(self.unexpected_difference.removed_files, wildcard)

    def expect_modification(self, names):
        if type(names) == types.StringType:
                names = [names]
        for name in names:
                try:
                        self.unexpected_difference.modified_files.remove(name)
                except:
                        print "File %s not modified as expected" % (name,)
                        self.fail_test(1)

    def ignore_modification(self, wildcard):
        ignore_elements(self.unexpected_difference.modified_files, wildcard)

    def expect_touch(self, names):
        
        if type(names) == types.StringType:
            names = [names]
            
        d = self.unexpected_difference
        for name in names:

            # We need to check in both touched and modified files if
            # it's a Windows exe because they sometimes have slight
            # differences even with identical inputs
            if name.endswith('.exe'):
                filesets = [d.modified_files, d.touched_files]
            else:
                filesets = [d.touched_files]

            while filesets:
                try:
                    filesets[-1].remove(name)
                    break
                except ValueError:
                    filesets.pop()

            if not filesets:
                print "File %s not touched as expected" % (name,)
                self.fail_test(1)


    def ignore_touch(self, wildcard):
        ignore_elements(self.unexpected_difference.touched_files, wildcard)

    def ignore(self, wildcard):
        ignore_elements(self.unexpected_difference.added_files, wildcard)
        ignore_elements(self.unexpected_difference.removed_files, wildcard)
        ignore_elements(self.unexpected_difference.modified_files, wildcard)
        ignore_elements(self.unexpected_difference.touched_files, wildcard)

    def expect_nothing(self, names):
        if type(names) == types.StringType:
            names = [names]
        for name in names:
            if name in self.difference.added_files:
                print "File %s is added, but no action was expected" % (name,)
                self.fail_test(1)
            if name in self.difference.removed_files:
                print "File %s is removed, but no action was expected" % (name,)
                self.fail_test(1)
                pass
            if name in self.difference.modified_files:
                print "File %s is modified, but no action was expected" % (name,)
                self.fail_test(1)
            if name in self.difference.touched_files:
                print "File %s is touched, but no action was expected" % (name,)
                self.fail_test(1)

    def expect_nothing_more(self):
        if not self.unexpected_difference.empty():
           print "Expected nothing more, but got the following:"
           self.unexpected_difference.pprint()
           self.fail_test(1)       

    def expect_content(self, name, content, exact=0):
        if exact:
            actual = self.read(name)
        else:
            actual = string.replace(self.read_and_strip(name), "\\", "/")

        if actual != content:
            print "Expected:\n"
            print content
            print "Got:\n"
            print actual
            self.fail_test(1)

    def maybe_do_diff(self, actual, expected):
        if os.environ.has_key("DO_DIFF") and os.environ["DO_DIFF"] != '':
            
            e = tempfile.mktemp("expected")
            a = tempfile.mktemp("actual")
            open(e, "w").write(expected)
            open(a, "w").write(actual)
            print "DIFFERENCE"
            if os.system("diff -u " + e + " " + a):
                print "Unable to compute difference"               
            os.unlink(e)
            os.unlink(a)    
        else:
            print "Set environmental variable 'DO_DIFF' to examine difference." 

    # Helpers
    def mul(self, *arguments):
        if len(arguments) == 0:
                return None
        else:
                here = arguments[0]
                if type(here) == type(''):
                        here = [here]

                if len(arguments) > 1:
                        there = apply(self.mul, arguments[1:])
                        result = []
                        for i in here:
                                for j in there:
                                        result.append(i + j)
                        return result
                else:
                        return here



    # Internal methods
    def ignore_elements(list, wildcard):
        """Removes in-place, element of 'list' that match the given wildcard."""
        w = fnmatch.translate(wildcard)
        list[:] = filter(lambda x, w=w: not w(x), list)

    def native_file_name(self, name):
        elements = string.split(name, "/")
        return os.path.normpath(apply(os.path.join, [self.workdir]+elements))

    # Wait while time is no longer equal to the time last "run_build_system"
    # call finished.
    def wait_for_time_change(self):
        while int(time.time()) == int(self.last_build_time):
            time.sleep(0.1)

            
class List:

    def __init__(self, s=""):
        # Have to handle espaced spaces correctly
        s = string.replace(s, "\ ", '\001')
        self.l = []
        for e in string.split(s):
            self.l.append(string.replace(e, '\001', ' '))

    def __len__(self):
        return len(l)

    def __getitem__(self, key):
        return self.l[key]

    def __setitem__(self, key, value):
        self.l[key] = value

    def __delitem__(self, key):
        del self.l[key]

    def __coerce__(self, other):
        if isinstance(other, type("")):
            return (self,List(other))
        else:
            return None

    def __str__(self):
        return str(self.l)

    def __repr__(self):
        return ( self.__module__ + '.List('
                 + repr(string.join(self.l, ' '))
                 + ')')

    def __mul__(self, other):
        result = List()
        for f in self:
            for s in other:
                result.l.append(f + s)
        return result

    def __add__(self, other):
        result = List()
        result.l = self.l[:] + other.l[:]
        return result

# quickie tests. Should use doctest instead.
if __name__ == '__main__':
    assert str(List("foo bar") * "/baz") == "['foo/baz', 'bar/baz']"
    assert repr("foo/" * List("bar baz")) == "__main__.List('foo/bar foo/baz')"
    print 'tests passed'




