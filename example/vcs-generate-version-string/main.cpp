// A program to print the version string.
#include <iostream>

extern const char * version_string;

int
main ()
{
   std::cout << version_string << "\n";

   return 0;
}
