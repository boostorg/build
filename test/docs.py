#!/usr/bin/env python3

# Copyright 2023 Nikita Kniazev
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

import re
import sys
from collections import defaultdict
from pathlib import Path


def read_text(path, root):
    try:
        with path.open(encoding='utf-8') as f:
            return f.read()
    except UnicodeDecodeError as e:
        with path.open() as f:
            data = f.read()
        i = data.rfind(b'\n', 0, e.start) + 1
        j = data.find(b'\n', e.end)
        e.reason += '\nOn line: {}\nIn file {}'.format(data[i:j],path.relative_to(root))
        raise


def main():
    for path in Path(__file__).absolute().parents:
        if (path / 'Jamroot.jam').is_file():
            root = path
            break
    else:
        print("Could not locate Jamroot.jam")
        exit(1)

    already_included = defaultdict(set)
    for doc in (root / 'doc/src').rglob('*.adoc'):
        if not doc.is_file():
            continue
        for match in re.finditer(r'include::([^\[]+)\[tag=([^\]]+)', read_text(doc, root)):
            path = (doc.parent / match.group(1)).resolve().relative_to(root).as_posix()
            already_included[str(path)].add(match.group(2))

    if '-v' in sys.argv:
        for incl, tags in sorted(already_included.items()):
            print('* {}: {}'.format(incl,tags))

    fail = False
    #for path in (root / 'src').rglob('*.[jch]*'):
    for path in (root / 'src').rglob('*.jam'):
        if not path.is_file():
            continue

        tags = set(re.findall(r'tag::([^\[]+)', read_text(path, root)))
        if not tags:
            continue

        path = path.relative_to(root).as_posix()
        already_included_tags = already_included.get(str(path))
        if already_included_tags is None:
            print('{} has documentation but is not included anywhere, uses tags: {}'.format(path,", ".join(tags)))
            fail = True
            continue

        tags -= already_included_tags
        if tags:
            print('{} has unused in documentation tags: {}'.format(path,", ".join(tags)))
            fail = True

    if not fail and __name__ == '__main__':
        print('Everything seems to be OK', file=sys.stderr)

    exit(fail)


main()
