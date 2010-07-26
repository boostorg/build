
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
