#!/usr/bin/python
import os

# clear environment for testing
#
for s in (
    'BOOST_ROOT','BOOST_BUILD_PATH','JAM_TOOLSET','BCCROOT',
    'MSVCDir','MSVC','MSVCNT','MINGW','watcom'
    ):
    
    try:
        del os.environ[s]
    except:
        pass

import startup_v1
import startup_v2
import project_test1
import project_test3
import project_test4
