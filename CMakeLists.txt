cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)
project(AMpwg)
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Boost REQUIRED COMPONENTS system thread filesystem )
# =================[Define paths]===============
# Set this to your Slic3r install directory, remember to build it with CMake
set(SLIC3R_DIR
    "/home/mathia/Programming/slicing/Slic3r"
)
set(SLIC3R_BUILDDIR
    ${SLIC3R_DIR}/build
)
set(SLIC3R_SRCDIR
    ${SLIC3R_DIR}/xs/src
)
set(SRCDIR
    ${CMAKE_SOURCE_DIR}/src
)
set(INCLUDEDIR
    ${CMAKE_SOURCE_DIR}/include
)
add_executable(slice_extractor
    ${SRCDIR}/slice_extractor.cpp
)
find_library(libslic3r
    NAMES liblibslic3r.a
    HINTS "${SLIC3R_BUILDDIR}"
)
find_library(libadmesh
    NAMES libadmesh.a
    HINTS "${SLIC3R_BUILDDIR}"
)
find_library(libBSpline
    NAMES libBSpline.a
    HINTS "${SLIC3r_BUILDDIR}"
)
find_library(libclipper
    NAMES libclipper.a
    HINTS "${SLIC3R_BUILDDIR}"
)
find_library(libexpat
    NAMES libexpat.a
    HINTS "${SLIC3R_BUILDDIR}"
)
find_library(libpolypartition
    NAMES libpolypartition.a
    HINTS "${SLIC3R_BUILDDIR}"
)
find_library(libpoly2tri
    NAMES libpoly2tri.a
    HINTS "${SLIC3R_BUILDDIR}"
)
find_library(libZipArchive
    NAMES libZipArchive.a
    HINTS "${SLIC3R_BUILDDIR}"
)
target_include_directories(slice_extractor
    PUBLIC
    ${INCLUDEDIR}
    ${SLIC3R_SRCDIR}/libslic3r
    ${SLIC3R_SRCDIR}
)
target_link_libraries(slice_extractor
    ${libslic3r}
    ${libadmesh}
    ${libBSpline}
    ${libclipper}
    ${libexpat}
    ${libpolypartition}
    ${libpoly2tri}
    ${libZipArchive}
    ${Boost_LIBRARIES}
)