#!/bin/bash

# Grab jam_src
cp -r ../jam_src .

# Clean up
rm -rf examples
# This one is not fully finished
rm -rf examples-v2/versioned
# This one is too low-level and misleading
rm -rf examples-v2/make

find . -maxdepth 1 -type f | egrep -v "roll.sh|build-system.jam|boost_build_v2.html|c++boost.gif|index_v2.html|hacking.txt" | xargs rm -f

# Prepare some more files.
mv index_v2.html index.html
echo -e "boost-build kernel ;\n" > boost-build.jam

# Make packages
find . -name CVS | xargs rm -rf
rm roll.sh
chmod a+x jam_src/build.bat
cd .. && zip -r boost-build.zip boost-build && tar --bzip2 -cf boost-build.tar.bz2 boost-build
cd boost-build

# Upload docs to sourceforge
perl -pi -e 's%<!-- sf logo -->%<a href="http://sourceforge.net"><img src="http://sourceforge.net/sflogo.php?group_id=7586&amp;type=1" width="88" height="31" border="0" alt="SourceForge.net Logo" align="right"/></a>%' boost_build_v2.html index.html doc/*.html
scp -r doc examples-v2 *.html hacking.txt vladimir_prus@shell.sourceforge.net:/home/groups/b/bo/boost/htdocs/boost-build2
