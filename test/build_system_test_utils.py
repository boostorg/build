# Copyright (C) 2001 Vladimir Prus. Permission to copy, use, modify, sell and
# distribute this software is granted, provided this copyright notice appears
# in all copies and modified versions are clearly marked as such. This software
# is provided "as is" without express or implied warranty, and with no claim as
# to is suitability for any purpose.

import os;
import stat;
import zipfile;

def list2map(list):
    r = {}
    for l in list:
        r[l[0]] = l[1]
    return r

def unpack_zip(archive, dir):
    z = zipfile.ZipFile(archive)
    for i in z.namelist():
        target_path = os.path.join(dir, os.path.normpath(i))

        if os.path.basename(i) == '':
            os.mkdir(target_path)
        else:
            f = open(target_path, "wb")
            f.write(z.read(i))


# I couldn't find anything similar in the os module.
def rm_recursively(name):
    s = os.stat(name)
    if stat.S_ISDIR(s[stat.ST_MODE]):
        try:
            os.rmdir(name);
        except OSError:
            l = os.listdir(name)
            for i in l:
                rm_recursively(os.path.join(name, i))
            os.rmdir(name)
    else:
        os.remove(name)

def inherited(child, parent):
    if parent in child.__bases__:
        return 1
    else:
        for i in child.__bases__:
            if inherited(i, parent):
                return 1
        else:
            return 0

def lookup(the_class, name):
    # Let's do it the right way -- BFS etc.
    classes = [the_class]
    while classes:
        # See if the name is here
        for c in classes:
            if c.__dict__.has_key(name):
                return c.__dict__[name]
        # Move one layer higher
        cc = classes[:]
        classes = []
        for c in cc:
            classes.extend(c.__bases__)
    raise AttributeError

