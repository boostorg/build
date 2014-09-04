#  (C) Copyright David Abrahams 2001. Permission to copy, use, modify, sell and
#  distribute this software is granted provided this copyright notice appears in
#  all copies. This software is provided "as is" without express or implied
#  warranty, and with no claim as to its suitability for any purpose.

import re

from b2.util import bjam_signature


def transform (list, pattern, indices = [1]):
    """ Matches all elements of 'list' agains the 'pattern' 
        and returns a list of the elements indicated by indices of 
        all successfull matches. If 'indices' is omitted returns
        a list of first paranthethised groups of all successfull
        matches.
    """
    result = []

    for e in list:
        m = re.match (pattern, e)

        if m:
            for i in indices:
                result.append (m.group (i))

    return result


@bjam_signature((['s', 'match', 'replacement']))
def replace(s, match, replacement):
    return s.replace(match, replacement)


@bjam_signature((['items', '*'], ['match'], ['replacement']))
def replace_list(items, match, replacement):
    return [replace(item, match, replacement) for item in items]
