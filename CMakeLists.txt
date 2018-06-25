cmake_minimum_required (VERSION 2.6) 
 
set (PROJECT polygons) 
 
project (${PROJECT}) 

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=c++17 -g -lm")
endif (CMAKE_COMPILER_IS_GNUCXX)

set (HEADERS
    src/geometry/point.hpp
    src/algebra/vector.hpp
    src/algebra/matrix.hpp
    src/error_handling/my_exception.hpp
    src/geometry/line_segment.hpp
    src/geometry/orthobrick.hpp
    src/geometry/plane.hpp
    src/geometry/polygon.hpp
    src/geometry/polygonal_polyhedron.hpp
    src/geometry/polygonal_cylinder.hpp
    src/settings/settings_parser.hpp
    src/io/csg_printer_polygons.hpp
    src/systems_architecture/polyhedronal_system.hpp
    src/systems_architecture/ternary_polygonal_system.hpp
    src/percolation/percolation_checker.hpp
)

set (SOURCES
    src/geometry/point.cpp
    src/algebra/vector.cpp
    src/algebra/matrix.cpp
    src/error_handling/my_exception.cpp
    src/geometry/line_segment.cpp
    src/geometry/orthobrick.cpp
    src/geometry/plane.cpp
    src/geometry/polygon.cpp
    src/geometry/polygonal_polyhedron.cpp
    src/geometry/polygonal_cylinder.cpp
    src/settings/settings_parser.cpp
    src/io/csg_printer_polygons.cpp
    src/systems_architecture/polyhedronal_system.cpp
    src/systems_architecture/ternary_polygonal_system.cpp
    src/percolation/percolation_checker.cpp
)

add_executable (MC_exfoliation ${HEADERS} ${SOURCES} MC_exfoliation.cpp)
add_executable (mixing ${HEADERS} ${SOURCES} mixing.cpp)
#add_executable (tester ${HEADERS} ${SOURCES} test.cpp)
