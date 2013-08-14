BTRFdom
=======

Rappelz BTRF Document Object Model

Dependencies
------------

To compile BTRFdom, you must have at least [cmake 2.8](http://www.cmake.org/cmake/resources/software.html) and a C++11 capable compiler. (MS Visual C++ 2010 and [MinGW](http://sourceforge.net/projects/mingw/files/latest/download?source=files) support needed features of C++11 at this time). No additional libraries are required.

How to compile
--------------

To compile the project, create a build directory and open a command prompt with the build directory as current directory. Then type `cmake <root_of_source_repo>`. If you have created the build directory in the root directory of the source repo, then type `cmake ..` or use the absolute path.

For MinGW, cmake need that g++ and other binaries to be in the `PATH` environment variable. To do that, type this command in cmd: `set PATH=%PATH%;<path_to_binaries>` for example, if your MinGW installation is `C:\MinGW` then type `set PATH=%PATH%;C:\MinGW\bin`

Notes
-----

A python client example is available in dll-client-python.