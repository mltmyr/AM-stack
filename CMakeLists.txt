cmake_minimum_required(VERSION 3.9.0 FATAL_ERROR)

project(AMwpg)



option(SLIC3R_STATIC "Use static boost library" ON)

# only on newer GCCs: -ftemplate-backtrace-limit=0
add_compile_options(-DNO_PERL -DM_PI=3.14159265358979323846 -DHAS_BOOL -DNOGDI -DBOOST_ASIO_DISABLE_KQUEUE)

if (MSVC)
    add_compile_options(-W3)
    add_compile_options(-bigobj) 
else()
    add_compile_options(-Wall)
endif()

# mingw needs to be told to deal with large objects
if(MINGW)
    add_compile_options(-Wa,-mbig-obj) 
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

IF(CMAKE_HOST_APPLE)
    add_compile_options(-stdlib=libc++ -DBOOST_THREAD_DONT_USE_CHRONO -DBOOST_NO_CXX11_RVALUE_REFERENCES -DBOOST_THREAD_USES_MOVE)
    set(CMAKE_EXE_LINKER_FLAGS "-framework IOKit -framework CoreFoundation -lc++")
ELSE(CMAKE_HOST_APPLE)
#    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -L.")
ENDIF(CMAKE_HOST_APPLE)

if(SLIC3R_STATIC)
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_STATIC_RUNTIME ON)
else(SLIC3R_STATIC)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif(SLIC3R_STATIC)

if(MSVC)
    if(SLIC3R_STATIC)
        set(LIBSLIC3R_STATIC_DEBUG_OPTIONS "/MTd")
        set(LIBSLIC3R_STATIC_RELEASE_OPTIONS "/MT")
        add_compile_options(
            "$<$<CONFIG:DEBUG>:${LIBSLIC3R_STATIC_DEBUG_OPTIONS}>"
            "$<$<CONFIG:RELEASE>:${LIBSLIC3R_STATIC_RELEASE_OPTIONS}>"
        )
    else(SLIC3R_STATIC)
    
    endif(SLIC3R_STATIC)
endif(MSVC)

#====================[Threads dependency]====================
find_package(Threads REQUIRED)

#====================[Boost dependency]====================
set(BOOST_INCLUDEDIR "D:/boost/boost_1_67_0")
set(BOOST_LIBRARYDIR "D:/boost/boost_1_67_0/stage/lib")

#set(BOOST_INCLUDEDIR "D:/boost/boost")
#set(BOOST_LIBRARYDIR "D:/boost/boost/stage/lib")

set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost REQUIRED COMPONENTS system thread filesystem)

#====================[LibSlic3r dependency]====================
set(Slic3r_DIR "D:/slic3r")

#=====[admesh]=====
set(admesh_DIR "${Slic3r_DIR}/xs/src/admesh")
set(admesh_INCLUDE_DIRS
    ${admesh_DIR}
)
add_library(admesh STATIC
    ${admesh_DIR}/stl_io.c
    ${admesh_DIR}/stlinit.c
    ${admesh_DIR}/util.c
    ${admesh_DIR}/shared.c
    ${admesh_DIR}/normals.c
    ${admesh_DIR}/connect.c
)
target_include_directories(admesh PUBLIC ${admesh_INCLUDE_DIRS})
set_property(TARGET admesh PROPERTY C_STANDARD 99)
target_compile_options(admesh PUBLIC -w)

#=====[BSpline]=====
set(BSpline_DIR "${Slic3r_DIR}/xs/src/BSpline")
set(BSpline_INCLUDE_DIRS
    ${BSpline_DIR}
)
add_library(BSpline STATIC
    ${BSpline_DIR}/BSpline.cpp
)
target_include_directories(BSpline PUBLIC ${BSpline_INCLUDE_DIRS})
target_compile_options(BSpline PUBLIC -w)

#=====[clipper]=====
set(clipper_DIR "${Slic3r_DIR}/xs/src")
set(clipper_INCLUDE_DIRS
    ${clipper_DIR}
)
add_library(clipper STATIC
    ${clipper_DIR}/clipper.cpp
)
target_compile_features(clipper PUBLIC cxx_std_11)
target_include_directories(clipper PUBLIC ${clipper_INCLUDE_DIRS})
target_compile_options(clipper PUBLIC -w)

#=====[expat]=====
set(expat_DIR "${Slic3r_DIR}/xs/src/expat")
set(expat_INCLUDE_DIRS
    ${expat_DIR}
)
add_library(expat STATIC
    ${expat_DIR}/xmlparse.c
    ${expat_DIR}/xmlrole.c
    ${expat_DIR}/xmltok.c
)
target_compile_features(expat PUBLIC cxx_std_11)
target_include_directories(expat PUBLIC ${expat_INCLUDE_DIRS})
target_compile_options(expat PUBLIC -w)

#=====[polypartition]=====
set(polypartition_DIR "${Slic3r_DIR}/xs/src")
set(polypartition_INCLUDE_DIRS
    ${polypartition_DIR}
)
add_library(polypartition STATIC
    ${polypartition_DIR}/polypartition.cpp
)
target_include_directories(polypartition PUBLIC ${polypartition_INCLUDE_DIRS})
target_compile_options(polypartition PUBLIC -w)

#=====[poly2tri]=====
set(poly2tri_DIR "${Slic3r_DIR}/xs/src/poly2tri")
set(poly2tri_INCLUDE_DIRS
    ${poly2tri_DIR}
    ${poly2tri_DIR}/sweep
    ${poly2tri_DIR}/common
)
add_library(poly2tri STATIC
    ${poly2tri_DIR}/common/shapes.cc
    ${poly2tri_DIR}/sweep/advancing_front.cc
    ${poly2tri_DIR}/sweep/cdt.cc
    ${poly2tri_DIR}/sweep/sweep_context.cc
    ${poly2tri_DIR}/sweep/sweep.cc
)
target_include_directories(poly2tri PUBLIC ${poly2tri_INCLUDE_DIRS})
target_compile_options(poly2tri PUBLIC -w)

#=====[ZipArchive]=====
set(ZipArchive_DIR "${Slic3r_DIR}/xs/src/Zip")
set(ZipArchive_INCLUDE_DIRS
    ${ZipArchive_DIR}
    ${Slic3r_DIR}/xs/src
)
add_library(ZipArchive STATIC
    ${ZipArchive_DIR}/ZipArchive.cpp
)
target_compile_features(ZipArchive PUBLIC cxx_std_11)
target_include_directories(ZipArchive PUBLIC ${ZipArchive_INCLUDE_DIRS})
target_compile_options(ZipArchive PUBLIC -w)

#=====[LibSlic3r base]=====
set(LibSlic3r_DIR "${Slic3r_DIR}/xs/src/libslic3r")
set(LibSlic3r_INCLUDE_DIRS
    ${LibSlic3r_DIR}
    ${LibSlic3r_DIR}/Fill
    ${LibSlic3r_DIR}/GCode
    ${LibSlic3r_DIR}/IO
)
add_library(LibSlic3r STATIC
    ${LibSlic3r_DIR}/BoundingBox.cpp
    ${LibSlic3r_DIR}/BridgeDetector.cpp
    ${LibSlic3r_DIR}/ClipperUtils.cpp
    ${LibSlic3r_DIR}/ConditionalGCode.cpp
    ${LibSlic3r_DIR}/Config.cpp
    ${LibSlic3r_DIR}/ConfigBase.cpp
    ${LibSlic3r_DIR}/ExPolygon.cpp
    ${LibSlic3r_DIR}/ExPolygonCollection.cpp
    ${LibSlic3r_DIR}/Extruder.cpp
    ${LibSlic3r_DIR}/ExtrusionEntity.cpp
    ${LibSlic3r_DIR}/ExtrusionEntityCollection.cpp
    ${LibSlic3r_DIR}/Flow.cpp
    ${LibSlic3r_DIR}/GCode.cpp
    ${LibSlic3r_DIR}/GCodeReader.cpp
    ${LibSlic3r_DIR}/GCodeSender.cpp
    ${LibSlic3r_DIR}/GCodeTimeEstimator.cpp
    ${LibSlic3r_DIR}/GCodeWriter.cpp
    ${LibSlic3r_DIR}/Geometry.cpp
    ${LibSlic3r_DIR}/IO.cpp
    ${LibSlic3r_DIR}/Layer.cpp
    ${LibSlic3r_DIR}/LayerHeightSpline.cpp
    ${LibSlic3r_DIR}/LayerRegion.cpp
    ${LibSlic3r_DIR}/LayerRegionFill.cpp
    ${LibSlic3r_DIR}/Line.cpp
    ${LibSlic3r_DIR}/Log.cpp
    ${LibSlic3r_DIR}/Model.cpp
    ${LibSlic3r_DIR}/MotionPlanner.cpp
    ${LibSlic3r_DIR}/MultiPoint.cpp
    ${LibSlic3r_DIR}/PerimeterGenerator.cpp
    ${LibSlic3r_DIR}/PlaceholderParser.cpp
    ${LibSlic3r_DIR}/Point.cpp
    ${LibSlic3r_DIR}/Polygon.cpp
    ${LibSlic3r_DIR}/Polyline.cpp
    ${LibSlic3r_DIR}/PolylineCollection.cpp
    ${LibSlic3r_DIR}/Print.cpp
    ${LibSlic3r_DIR}/PrintConfig.cpp
    ${LibSlic3r_DIR}/PrintGCode.cpp
    ${LibSlic3r_DIR}/PrintObject.cpp
    ${LibSlic3r_DIR}/PrintRegion.cpp
    ${LibSlic3r_DIR}/SLAPrint.cpp
    ${LibSlic3r_DIR}/SVG.cpp
    ${LibSlic3r_DIR}/SimplePrint.cpp
    ${LibSlic3r_DIR}/SlicingAdaptive.cpp
    ${LibSlic3r_DIR}/SupportMaterial.cpp
    ${LibSlic3r_DIR}/Surface.cpp
    ${LibSlic3r_DIR}/SurfaceCollection.cpp
    ${LibSlic3r_DIR}/TransformationMatrix.cpp
    ${LibSlic3r_DIR}/TriangleMesh.cpp
    ${LibSlic3r_DIR}/utils.cpp
    
    ${LibSlic3r_DIR}/Fill/Fill.cpp
    ${LibSlic3r_DIR}/Fill/Fill3DHoneycomb.cpp
    ${LibSlic3r_DIR}/Fill/FillConcentric.cpp
    ${LibSlic3r_DIR}/Fill/FillGyroid.cpp
    ${LibSlic3r_DIR}/Fill/FillHoneycomb.cpp
    ${LibSlic3r_DIR}/Fill/FillPlanePath.cpp
    ${LibSlic3r_DIR}/Fill/FillRectilinear.cpp
    
    ${LibSlic3r_DIR}/GCode/CoolingBuffer.cpp
    ${LibSlic3r_DIR}/GCode/SpiralVase.cpp
    
    ${LibSlic3r_DIR}/IO/AMF.cpp
    ${LibSlic3r_DIR}/IO/TMF.cpp
)

target_compile_features(LibSlic3r PUBLIC cxx_std_14)
if(MSVC)
    if(SLIC3R_STATIC)
        #target_link_libraries(LibSlic3r -static)
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
        target_link_options(LibSlic3r PUBLIC /INCREMENTAL:NO /NODEFAULTLIB:MSVCRT)
    endif(SLIC3R_STATIC)
endif(MSVC)

target_include_directories(LibSlic3r PUBLIC ${LibSlic3r_INCLUDE_DIRS} PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(LibSlic3r
    admesh
    BSpline
    clipper
    expat
    polypartition
    poly2tri
    ZipArchive
    ${Boost_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
)

#====================[Create Executable Targets]====================
set(executableName am_wpg)

set(includeFolderName ".")
set(sourceFolderName ".")
set(sourceFiles
    ${sourceFolderName}/main.cpp
)

add_executable(${executableName} ${sourceFiles})
target_compile_features(${executableName} PUBLIC cxx_std_14)
target_include_directories(${executableName} 
    PUBLIC ${includeFolderName}
    PUBLIC ${LibSlic3rIncludeFiles})

target_link_libraries(${executableName} LibSlic3r)

#==============================[(CMake Debug) List CMake variables]==============================
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

