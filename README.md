# AM-stack

### Purpose
Slice and generate trajectories for additive manufacturing with robot manipulator. Builds upon LibSlic3r ([github](https://github.com/slic3r/Slic3r/tree/master/xs/src/libslic3r)), a submodule of [Slic3r](https://manual.slic3r.org/) ([github](https://github.com/slic3r/Slic3r)).

### Dependencies
1. [Slic3r](https://github.com/slic3r/Slic3r) and its dependencies
2. [CMake(Build system)](https://cmake.org/download/)
3. At least C++11

### Installation
1. Download [Slic3r](https://github.com/slic3r/Slic3r).
2. Build [Slic3r using CMake](https://github.com/slic3r/Slic3r/wiki/Running-Slic3r-from-git-on-GNU-Linux#build-slic3r-c) (or your preferred OS).
3. Point `CMakeLists.txt` to your Slic3r directory.
4. Run the following commands:
```
mkdir build
cd build
cmake ..
make
```

### Usage
Run `./slice_extractor -i MESHFILE.stl -o RESULTS.json` or if you just want a list of points in csv format `./slice_extractor -i MESHFILE.STL -o RESULTS.csv --only-points`.
Check the input flag help: `./slice_extractor -h` for more usage
