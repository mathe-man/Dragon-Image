cmake_minimum_required(VERSION 3.15)

# Solution name
project(Dragon Image)

# Define SDL3 installation folder
set(SDL3_ROOT "C:/Libs/SDL3")
set(SDL3_INCLUDE_DIR "${SDL3_ROOT}/include")
set(SDL3_LIB_DIR "${SDL3_ROOT}/lib/x64")

# Add SDL3
include_directories(${SDL3_INCLUDE_DIR})
link_directories(${SDL3_LIB_DIR})

# Enable header include from other projects in the solution
include_directories(${CMAKE_SOURCE_DIR})

# Add projects (static libs)
add_subdirectory(DrimLib)
add_subdirectory(DrimEditor)
add_subdirectory(DrimDebugger)

# Add app (executable project)
add_subdirectory(DrimApp)




#    Add a new extern lib:
# Define is installation folder then include and link it, 
# take exemple on SDL3 here.

#    Add a new project lib:
# Add the project folder up-there, this project must have a CMakeLists.txt inside,
# take exemple on DrimEditor and is CMakeLists.txt file.
