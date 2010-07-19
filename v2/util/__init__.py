
def bjam_signature(s):

    def wrap(f):       
        f.bjam_signature = s
        return f

    return wrap
