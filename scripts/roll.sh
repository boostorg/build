#!/bin/bash

# Clean up

rm -rf examples
# This one is not fully finished
rm -rf examples-v2/versioned

find . -maxdepth 1 -type f | grep -v "roll.sh" | xargs rm -rf
cvs up boost_build_v2.html
cvs up c++boost.gif
cvs up index_v2.html
cvs up hacking.txt

mv index_v2.html index.html

# Make packages
find . -name CVS | xargs rm -rf
rm roll.sh
cd .. && zip -r boost-build.zip boost-build && tar --bzip2 -cf boost-build.tar.bz2 boost-build
cd boost-build

# Upload docs to sourceforge
perl -pi -e 's%<!-- sf logo -->%<a href="http://sourceforge.net"><img src="http://sourceforge.net/sflogo.php?group_id=7586&amp;type=1" width="88" height="31" border="0" alt="SourceForge.net Logo" align="right"/></a>%' boost_build_v2.html index.html doc/*.html
scp -r doc *.html hacking.txt vladimir_prus@shell.sourceforge.net:/home/groups/b/bo/boost/htdocs/boost-build2
