
This example shows how you can use Python modules from Boost.Build.

In order to do this, you need to build bjam with Python support, by running:

   ./build.sh --with-python=/usr
   
in jam directory. (Replace /usr with the root of your Python installation.)

The integration between Python and bjam is very basic now, but enough to
be useful.