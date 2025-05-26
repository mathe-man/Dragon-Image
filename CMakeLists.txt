cmake_minimum_required(VERSION 3.16)
project(Solution)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SDL3 local
set(SDL3_PATH "C:/Libs/SDL3")
include_directories("${SDL3_PATH}/include")
link_directories("${SDL3_PATH}/lib/x64")

# SDL3_image local
set(SDL3_image_PATH "C:/Libs/SDL3_image")
include_directories("${SDL3_image_PATH}/include")
link_directories("${SDL3_image_PATH}/lib/x64")

# SDL3_ttf local
set(SDL3_ttf_PATH "C:/Libs/SDL3_ttf")
include_directories("${SDL3_ttf_PATH}/include")
link_directories("${SDL3_ttf_PATH}/lib/x64")



# Fonction to add a folder as a lib
function(auto_add_library libname path)
    file(GLOB_RECURSE SOURCES "${path}/*.cpp" "${path}/*.c")
    file(GLOB_RECURSE HEADERS "${path}/*.h" "${path}/*.hpp")
    add_library(${libname} STATIC ${SOURCES} ${HEADERS})
    target_include_directories(${libname} PUBLIC ${path})
endfunction()

# Add libs
auto_add_library(ProjectLib "${CMAKE_SOURCE_DIR}/DrimLib")
auto_add_library(ProjectEditor "${CMAKE_SOURCE_DIR}/DrimEditor")
auto_add_library(ProjectDebugger "${CMAKE_SOURCE_DIR}/DrimDebugger")

# Add includes
target_include_directories(ProjectDebugger PRIVATE ${CMAKE_SOURCE_DIR}/DrimEditor)
target_include_directories(ProjectDebugger PRIVATE ${CMAKE_SOURCE_DIR}/DrimLib)
target_include_directories(ProjectDebugger PRIVATE ${CMAKE_SOURCE_DIR}/DrimLib)



message(STATUS "Files in ProjectEditor:")
file(GLOB_RECURSE DEBUG_SOURCES "${CMAKE_SOURCE_DIR}/DrimEditor/*.cpp")
foreach(file ${DEBUG_SOURCES})
    message(STATUS " - ${file}")
endforeach()


# Executable
file(GLOB_RECURSE EXE_SOURCES "DrimApp/*.cpp")

# Create main executable file
add_executable(ProjectApp ${EXE_SOURCES})
target_include_directories(ProjectApp PUBLIC ${CMAKE_SOURCE_DIR}/DrimApp)
target_include_directories(ProjectApp PRIVATE ${CMAKE_SOURCE_DIR}/DrimDebugger)


# Link external libs
target_link_libraries(ProjectEditor PRIVATE SDL3 SDL3_image SDL3_ttf)
target_link_libraries(ProjectApp PRIVATE ProjectLib ProjectEditor ProjectDebugger)
