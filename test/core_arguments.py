#!/usr/bin/python

# Copyright 2001 Dave Abrahams
# Copyright 2011 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

import BoostBuild


def test(t, type, input, output):
    code = ["include echo_args.jam ; echo_%s" % type]
    if input: code.append(input)
    code.append(";")
    t.write("file.jam", " ".join(code))
    t.run_build_system("-ffile.jam", status=1)
    t.expect_output_line(output);


def test_args(t, *args, **kwargs):
    test(t, "args", *args, **kwargs)


def test_varargs(t, *args, **kwargs):
    test(t, "varargs", *args, **kwargs)


t = BoostBuild.Tester(pass_toolset=0, pass_d0=False)

t.write("echo_args.jam", """
rule echo_args ( a b ? c ? : d + : e * )
{
    ECHO a= $(a) b= $(b) c= $(c) ":" d= $(d) ":" e= $(e) ;
}

rule echo_varargs ( a b ? c ? : d + : e * : * )
{
    ECHO a= $(a) b= $(b) c= $(c) ":" d= $(d) ":" e= $(e)
      ": rest= "$(4[1]) $(4[2])
      ": "$(5[1]) $(5[2])
      ": "$(6[1]) $(6[2])
      ": "$(7[1]) $(7[2])
      ": "$(8[1]) $(8[2])
      ": "$(9[1]) $(9[2]) ;
}
""")

test_args(t, "", "* missing argument a")
test_args(t, "1 2 : 3 : 4 : 5", "* extra argument 5")
test_args(t, "a b c1 c2 : d", "* extra argument c2")

# Check modifier '?'
test_args(t, "1 2 3 : 4", "a= 1 b= 2 c= 3 : d= 4 : e=")
test_args(t, "1 2 : 3", "a= 1 b= 2 c= : d= 3 : e=")
test_args(t, "1 2 : 3", "a= 1 b= 2 c= : d= 3 : e=")
test_args(t, "1 : 2", "a= 1 b= c= : d= 2 : e=")

# Check modifier '+'
test_args(t, "1", "* missing argument d")
test_args(t, "1 : 2 3", "a= 1 b= c= : d= 2 3 : e=")
test_args(t, "1 : 2 3 4", "a= 1 b= c= : d= 2 3 4 : e=")

# Check modifier '*'
test_args(t, "1 : 2 : 3", "a= 1 b= c= : d= 2 : e= 3")
test_args(t, "1 : 2 : 3 4", "a= 1 b= c= : d= 2 : e= 3 4")
test_args(t, "1 : 2 : 3 4 5", "a= 1 b= c= : d= 2 : e= 3 4 5")

# Check varargs
test_varargs(t, "1 : 2 : 3 4 5", "a= 1 b= c= : d= 2 : e= 3 4 5")
test_varargs(t, "1 : 2 : 3 4 5 : 6", "a= 1 b= c= : d= 2 : e= 3 4 5 : rest= 6")
test_varargs(t, "1 : 2 : 3 4 5 : 6 7",
    "a= 1 b= c= : d= 2 : e= 3 4 5 : rest= 6 7")
test_varargs(t, "1 : 2 : 3 4 5 : 6 7 : 8",
    "a= 1 b= c= : d= 2 : e= 3 4 5 : rest= 6 7 : 8")
test_varargs(t, "1 : 2 : 3 4 5 : 6 7 : 8 : 9",
    "a= 1 b= c= : d= 2 : e= 3 4 5 : rest= 6 7 : 8 : 9")

t.cleanup()
