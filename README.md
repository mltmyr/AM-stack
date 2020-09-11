# AM-stack

### Purpose
Slice and generate trajectories for additive manufacturing with robot manipulator. Builds upon LibSlic3r ([github](https://github.com/slic3r/Slic3r/tree/master/xs/src/libslic3r)), a submodule of [Slic3r](https://manual.slic3r.org/) ([github](https://github.com/slic3r/Slic3r)).

### Dependencies
1. [Slic3r](https://github.com/slic3r/Slic3r)
2. [Boost](https://github.com/boostorg/boost)
3. [CMake(Build system)](https://cmake.org/download/)

### Setup
* Clone this repository to a directory of your choice.
* Clone Slic3r repository to a directory of your choice.
* Download [CMake](https://cmake.org/download/) (version 3.18 or newer)
* Download [precompiled boost library](https://sourceforge.net/projects/boost/files/boost-binaries/) (version 1.74 or newer) and install/unpack to a directory of your choice.
* Generate project with CMake.
* Open the `CMakeLists.txt` file.
* In `CMakeLists.txt`, set `Slic3r_DIR` equal to the root directory of your Slic3r clone.
* In `CMakeLists.txt`, set `Boost_DIR` equal to the folder containing the `BoostConfig.cmake` (located in `lib64-msvc-14.1/cmake/Boost-1.74` or similar subfolder of the boost library).
* Generate build with CMake. (Use CMake-gui if you are not super confident about the command-line invocation of cmake).
* Build with platform appropriate tool. MVSC on windows. Usually makefile on linux.

**Only tested with MVSC and CL compiler!! Changes to the CMakelists.txt may be required for other platforms.**