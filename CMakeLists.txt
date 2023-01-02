cmake_minimum_required(VERSION 3.19)
project(QuadTree C)

set(CMAKE_C_STANDARD 99)

include_directories(include)
add_executable(QuadTree src/main.c src/vector.c src/graph.c include/vector.h src/quadtree.c src/partitions.c include/quadtree.h include/queue.h src/queue.c include/graph.h include/partitions.h)
target_link_libraries(QuadTree m)
