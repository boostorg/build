Development code for new build system. To test, execute:

  jam -sBOOST_BUILD_PATH=.:$BOOST_ROOT -sBOOST_BUILD_TEST=1 -sJAMFILE=test.jam

on unix, or 

  jam -sBOOST_BUILD_PATH=.;%BOOST_ROOT% -sBOOST_BUILD_TEST=1 -sJAMFILE=test.jam

on windows
