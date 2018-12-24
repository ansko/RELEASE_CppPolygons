cmake_minimum_required (VERSION 2.6) 
 
set (PROJECT polygons) 
 
project (${PROJECT}) 

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++17 -g -lm  -O3")
endif (CMAKE_COMPILER_IS_GNUCXX)

set (COMMON_HEADERS
    src/geometry/point.hpp
    src/algebra/vector.hpp
    src/algebra/matrix.hpp
    src/error_handling/my_exception.hpp
    src/geometry/line_segment.hpp
    src/geometry/orthobrick.hpp
    src/geometry/plane.hpp
    src/geometry/polygon.hpp
    src/geometry/sphere.hpp
    src/geometry/polygonal_polyhedron.hpp
    src/geometry/polygonal_cylinder.hpp
    src/settings/settings_parser.hpp
    src/io/csg_printer_polygons.hpp
    src/systems_architecture/polyhedronal_system.hpp
)
set (COMMON_SOURCES
    src/geometry/point.cpp
    src/algebra/vector.cpp
    src/algebra/matrix.cpp
    src/error_handling/my_exception.cpp
    src/geometry/line_segment.cpp
    src/geometry/orthobrick.cpp
    src/geometry/plane.cpp
    src/geometry/polygon.cpp
    src/geometry/sphere.cpp
    src/geometry/polygonal_polyhedron.cpp
    src/geometry/polygonal_cylinder.cpp
    src/settings/settings_parser.cpp
    src/io/csg_printer_polygons.cpp
    src/systems_architecture/polyhedronal_system.cpp
)

set (TERNARY_HEADERS
    src/systems_architecture/ternary_polygonal_system.hpp
    src/percolation/percolation_checker.hpp
)
set (TERNARY_SOURCES
    src/systems_architecture/ternary_polygonal_system.cpp
    src/percolation/percolation_checker.cpp
)

set (BINARY_HEADERS
    src/systems_architecture/binary_polygonal_system.hpp
)
set (BINARY_SOURCES
    src/systems_architecture/binary_polygonal_system.cpp
)


add_executable (ternary_mc
    ${COMMON_HEADERS} ${TERNARY_HEADERS}
    ${COMMON_SOURCES} ${TERNARY_SOURCES}
    ternary_mc.cpp)
add_executable (ternary_mix
    ${COMMON_HEADERS} ${TERNARY_HEADERS}
    ${COMMON_SOURCES} ${TERNARY_SOURCES}
    ternary_mix.cpp)

add_executable (binary_mc
    ${COMMON_HEADERS} ${BINARY_HEADERS}
    ${COMMON_SOURCES} ${BINARY_SOURCES}
    binary_mc.cpp)

add_executable (spheres_mc
    ${COMMON_HEADERS} ${BINARY_HEADERS}
    ${COMMON_SOURCES} ${BINARY_SOURCES}
    make_spheres.cpp)
