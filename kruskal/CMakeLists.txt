cmake_minimum_required(VERSION 3.21)
project(kruskal)

set(CMAKE_CXX_STANDARD 17)

add_executable(kruskal main.cpp UnionFind.h Node.h Edge.h Graph.h gnuplot-iostream.h)

#set(Boost_INCLUDE_DIR C:/msys64/mingw64/include)
#set(Boost_LIBRARY_DIR C:/msys64/mingw64/lib)

find_package(Boost REQUIRED COMPONENTS iostreams system filesystem)
target_include_directories(kruskal PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(kruskal PUBLIC ${Boost_LIBRARIES})