
import bjam
import re

# Decorator the specifies bjam-side prototype for a Python function
def bjam_signature(s):

    def wrap(f):       
        f.bjam_signature = s
        return f

    return wrap

class cached(object):

    def __init__(self, function):
        self.function = function
        self.cache = {}

    def __call__(self, *args):
        try:
            return self.cache[args]
        except KeyError:
            v = self.function(*args)
            self.cache[args] = v
            return v    

def unquote(s):
    if s and s[0] == '"' and s[-1] == '"':
        return s[1:-1]
    else:
        return s

_extract_jamfile_and_rule = re.compile("(Jamfile<.*>)%(.*)")

def call_jam_function(name, *args):

    m = _extract_jamfile_and_rule.match(name)
    if m:
        args = ("set-update-action-in-module", m.group(1), m.group(2)) + args
    else:
        args = ("set-update-action", name) + args

    return bjam.call(*args)
