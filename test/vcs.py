#!/usr/bin/python

import os

import BoostBuild

t = BoostBuild.Tester(use_test_config=False)

t.write("jamroot.jam", """
import vcs ;

vcs.fetch git : https://github.com/boostorg/build.git : wd-git ;

vcs.checkout wd-git : 2016.03 ;

echo "type:" [ vcs.type wd-git ] ;
echo "root-url:" [ vcs.root-url wd-git ] ;
echo "head-ref:" [ vcs.ref wd-git ] ;
echo "symbolic-ref:" [ vcs.ref wd-git : 2016.03 ] ;
echo "generate-version-string:" [ vcs.generate-version-string wd-git ] ;

vcs.fetch svn : https://github.com/boostorg/build.git : wd-svn ;

vcs.checkout wd-svn : tags/2016.03 ;

echo "type:" [ vcs.type wd-svn ] ;
echo "root-url:" [ vcs.root-url wd-svn ] ;
echo "head-ref:" [ vcs.ref wd-svn ] ;
echo "symbolic-ref:" [ vcs.ref wd-svn : tags/2016.03 ] ;
echo "generate-version-string:" [ vcs.generate-version-string wd-svn ] ;
""")

t.run_build_system()

t.expect_output_lines("type: git")
t.expect_output_lines("root-url: https://github.com/boostorg/build.git")
t.expect_output_lines("head-ref: e83838da44f46bbe1f9e07c61dd8d96d13be55df")
t.expect_output_lines("symbolic-ref: e83838da44f46bbe1f9e07c61dd8d96d13be55df")
t.expect_output_lines("generate-version-string: 2016.03")

t.expect_output_lines("type: svn")
t.expect_output_lines("root-url: https://github.com/boostorg/build.git")
t.expect_output_lines("head-ref: tags/2016.03@12703")
t.expect_output_lines("symbolic-ref: tags/2016.03@12703")
t.expect_output_lines("generate-version-string: 2016.03")
