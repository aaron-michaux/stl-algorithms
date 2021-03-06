
#define CATCH_CONFIG_PREFIX_ALL
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "algorithm.hxx"

int main(int argc, char** argv)
{
   Catch::Session session; // There must be exactly one instance

   // Let Catch (using Clara) parse the command line
   auto return_code = session.applyCommandLine(argc, argv);
   if(return_code != EXIT_SUCCESS) return return_code; // Command line error

   return session.run();
}
