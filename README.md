Thor
======

Cmake
======

Exporting targets from a build system, to see where the stuff is installed:  
add_executable(generator generator.c)
export(TARGETS generator FILE myproj-exports.cmake)