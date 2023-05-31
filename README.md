# game
A file based strategy game 

Outside libraries used:
- Google Test:
  Reason for using Google Test is that it offers great possibilities
  while being simple to use and powerfull. 
  It is also a very popular framework for testing in C++.
  Google Test was downloaded and installed using CMake.
- boost/process.hpp:
  Used for running the processes of players in mediator.
  Easy and interplatform way of solving this problem.
  Installed by downloading from Boost.org and packaging with bcp.
  BCP installation process (from downloaded directory using sudo):
  
    * ./bootstrap.sh
    * ./b2 tools/bcp
    * ./b2 install tools/bcp
    
  And running:
    bcp process.hpp <directory_with_source>
  
