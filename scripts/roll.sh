#!/bin/bash

set -e
# Do some renames/rearrangments

cp -r ../v2 ../boost-build
# Grab jam_src
cp -r ../jam_src ../boost-build
cd ../boost-build

# This one is not fully finished
rm -rf example/versioned
# This one is too low-level and misleading
rm -rf example/make

# Remove unnecessary top-level files
find . -maxdepth 1 -type f | egrep -v "roll.sh|bootstrap.jam|build-system.jam|boost_build_v2.html|boost.png|index.html|hacking.txt|site-config.jam|user-config.jam" | xargs rm -f

# Prepare some more files.
echo -e "boost-build kernel ;\n" > boost-build.jam

# Build the documentation
touch doc/project-root.jam
export BOOST_ROOT=/home/ghost/Work/boost-rc
cd doc
/home/ghost/Work/boost/tools/build/jam_src/bin.linuxx86/bjam --v2
/home/ghost/Work/boost/tools/build/jam_src/bin.linuxx86/bjam --v2 pdf
cp `find bin -name "*.pdf"` ../..
rm -rf bin
cd ..

# Get the boost logo.
wget http://boost.org/boost-build2/boost.png

# Adjust the links, so they work with the standalone package
perl -pi -e 's%../../../boost.png%boost.png%' index.html
perl -pi -e 's%../../../doc/html/bbv2.html%doc/html/index.html%' index.html
perl -pi -e 's%../../../doc/html/bbv2.installation.html%doc/html/bbv2.installation.html%' index.html

# Make packages
find . -name CVS | xargs rm -rf
rm roll.sh
chmod a+x jam_src/build.bat
date >> timestamp.txt
cd .. && zip -r boost-build.zip boost-build && tar --bzip2 -cf boost-build.tar.bz2 boost-build
cd boost-build

chmod -R u+w *
# Upload docs to sourceforge
perl -pi -e 's%<!-- sf logo -->%<a href="http://sourceforge.net"><img src="http://sourceforge.net/sflogo.php?group_id=7586&amp;type=1" width="88" height="31" border="0" alt="SourceForge.net Logo" align="right"/></a>%' index.html doc/*.html
scp -r  doc example *.html hacking.txt vladimir_prus@shell.sourceforge.net:/home/groups/b/bo/boost/htdocs/boost-build2
scp ../userman.pdf vladimir_prus@shell.sourceforge.net:/home/groups/b/bo/boost/htdocs/boost-build2/doc