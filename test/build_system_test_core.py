# Copyright (C) 2001 Vladimir Prus. Permission to copy, use, modify, sell and
# distribute this software is granted, provided this copyright notice appears
# in all copies and modified versions are clearly marked as such. This software
# is provided "as is" without express or implied warranty, and with no claim as
# to is suitability for any purpose.

from __future__ import nested_scopes;

import unittest;
import tempfile;
import os;
import sys;
import copy;
import types;

import tree;

from build_system_test_utils import *;

class Build_system_tester(unittest.TestCase):

    def setUp(self):
        try:
            self.__class__.archive
        except AttributeError:
            print "Class %s must have defined 'archive' member." % (self.__class__,)
            sys.exit(1)

        try:
            self.__class__.runner
        except AttributeError:
            print "Class %s must have defined 'runner' member." % (self.__class__,)
            sys.exit(1)

        self.arena = tempfile.mktemp()
        os.mkdir(self.arena)
        unpack_zip(self.__class__.archive, self.arena)
        self.tree = tree.build_tree(self.arena)

    def tearDown(self):
        rm_recursively(self.arena)

    def set_content(self, name, new_content):
        path = os.path.join(self.arena, name)
        f = open(path, "wb")
        f.write(new_content)

    def touch(self, name):
        real_name = os.path.join(self.arena, name)
        os.utime(real_name, None)

    def rebuild(self, subdir="", extra_options="", commands_only=0):
        cwd = os.getcwd()
        os.chdir(os.path.join(self.arena, subdir))
        try:
            # This is to prevent interpreting runner as method
            runner = lookup(self.__class__, 'runner')
            status, self.commands = runner(extra_options, commands_only)
            assert status == 0 or status == None
        finally:
            os.chdir(cwd)

        new_tree = tree.build_tree(self.arena)
        self.diff = tree.trees_difference(self.tree, new_tree)
        self.unexpected_diff = copy.deepcopy(self.diff)
        self.tree = new_tree

    def run(self, exe):
        cwd = os.getcwd()
        os.chdir(os.path.join(self.arena, os.path.dirname(exe)))
        f = os.popen(os.path.basename(exe))
        self.output = f.readlines()
        self.status = f.close()
        os.chdir(cwd)

    def expect_addition(self, names, dir=""):
        full_names = self.get_full_names(names, dir)
        for n in full_names:
            self.expect_addition_aux(n)

    def ignore_addition(self, wildcard):
        compiled_regexp = re.compile(fnmatch.translate(wildcard))
        for list in [self.unexpected_diff.added_files, self.unexpected_diff.removed_files,\
                     self.unexpected_diff.modified_files, self.unexpected_diff.touched_files]:
            list[:] = filter(lambda x: not self.path_match(x, compiled_regexp), list)

    def expect_removal(self, names, dir=""):
        full_names = get_full_names(names, dir)
        def not_present(name):
            return self.unexpected_diff.removed_files.count(name) == 0
        self.failUnless(filter(not_present, full_names) == [])
        def should_be_removed(name):
            return exists(full_names, lambda x: name[:len(x)] == x)
        self.unexpected_diff.removed_files = filter(should_be_removed, self.unexpected_diff.removed_files)

    def expect_touch(self, names, dir=""):
        names = self.get_full_names(names, dir)
        for n in names:
            self.failUnless(self.unexpected_diff.touched_files.count(n) != 0)
            self.unexpected_diff.touched_files.remove(n)

    def expect_nothing_more(self):
        self.failUnless(len(self.unexpected_diff.added_files) == 0)
        self.failUnless(len(self.unexpected_diff.removed_files) == 0)
        self.failUnless(len(self.unexpected_diff.touched_files) == 0)
        self.failUnless(len(self.unexpected_diff.modified_files) == 0)

    def get_full_names(self, names, dir):
        if type(names) == types.StringType:
            names = [ names ]
        if dir:
            if dir[-1:] != "/":
                dir = dir + "/"
        full_names = [dir+x for x in names]
        return full_names

    def path_match(self, path, compiled_regexp):
        """ Returns true if any component of path matched regular expression re."""
        components = split_path(path)
        for i in components:
            if compiled_regexp.match(i) is not None:
                return 1
        else:
            return 0

    def expect_addition_aux(self, name):
        self.failUnless(self.unexpected_diff.added_files.count(name) != 0)
        self.unexpected_diff.added_files.remove(name)
        while 1:
            name = os.path.split(name)[0]
            if not name:
                break
            name2 = name + "/"
            if self.unexpected_diff.added_files.count(name2):
                self.unexpected_diff.added_files.remove(name2)
