cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Fagso)

# Include Eigen matrix libraries
include_directories(eigen)
include_directories(fagso)

add_subdirectory(fagso)
