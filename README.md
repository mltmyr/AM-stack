# AM-stack

### Purpose
Slice and generate trajectories for additive manufacturing with robot manipulator. Builds upon LibSlic3r ([github](https://github.com/slic3r/Slic3r/tree/master/xs/src/libslic3r)), a submodule of [Slic3r](https://manual.slic3r.org/) ([github](https://github.com/slic3r/Slic3r)).

### Dependencies
1. [Slic3r](https://github.com/slic3r/Slic3r)
2. [Boost](https://github.com/boostorg/boost)
3. [CMake(Build system)](https://cmake.org/download/)

### Setup
1. Clone this repository to a directory of your choice.
2. Clone Slic3r repository to a directory of your choice.
3. Download [precompiled boost library](https://sourceforge.net/projects/boost/files/boost-binaries/) and install/unpack to a directory of your choice.
4.
5. Generate project with CMake.
6. Open the `CMakeLists.txt` file.
7. In `CMakeLists.txt`, set `Slic3r_DIR` equal to the root directory of your Slic3r clone.
8. In `CMakeLists.txt`, set `Boost_INCLUDEDIR` equal to boost root and `Boost_LIBRARYDIR` equal to 'boost_root/stage/lib'.
9. Build with your choosen toolchain.

**Only tested with MVSC and CL compiler!! Changes to the CMakelists.txt may be required for other platforms.**